# EXPERIMENTAL OFFLINE NavalRamming v7. Originals in reference/ are read-only.
# This patches a TObj state structure and must be tested with a NEW offline map.
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$enc = [System.Text.Encoding]::GetEncoding(28591)

function LoadRef([string]$rel) {
    $filename = Join-Path $root $rel
    if (-not (Test-Path -LiteralPath $filename)) { throw "Missing reference: $filename" }
    return [System.IO.File]::ReadAllText($filename, $enc)
}
function NL([string]$v) {
    $crlf = [string][char]13 + [string][char]10
    if ($v.Contains($crlf)) { return $crlf }
    return [string][char]10
}
function InsertAfter([string]$source, [string]$anchor, [string]$insertion, [string]$name) {
    $count = [regex]::Matches($source, [regex]::Escape($anchor)).Count
    if ($count -ne 1) { throw "Unexpected patch anchor for $name : $count matches. Aborting." }
    return $source.Replace($anchor, $anchor + $insertion)
}
function InsertBefore([string]$source, [string]$anchor, [string]$insertion, [string]$name) {
    $count = [regex]::Matches($source, [regex]::Escape($anchor)).Count
    if ($count -ne 1) { throw "Unexpected patch anchor for $name : $count matches. Aborting." }
    return $source.Replace($anchor, $insertion + $anchor)
}
function FormatLines([string]$v, [string]$newline) { return (($v -split '\r?\n') -join $newline) }

# 1. Explicit per-ship serialized state; NEVER reuse the game's existing unrelated TObj fields.
$globalOriginal = LoadRef 'reference\scripts\dmscript.global'
$globalNl = NL $globalOriginal
$globalNeedle = '      lasttimebestposition = Float'
$globalBlock = @'
      // NavalRamming experimental state (offline-only; change may affect old saves).
      navalSpeedFraction = Float
      navalLastRamTime = Float
'@
$globalPatched = InsertAfter $globalOriginal $globalNeedle ($globalNl + (FormatLines $globalBlock $globalNl)) 'TObj fields'

# 2. Every water vessel starts at 35% of original per-tick movement distance.
# All nine standard water-unit property files inherit MoveStep=0.03 from refunit.prop.
$initialOriginal = LoadRef 'reference\scripts\units\unit.inc\initial.inc'
$initialNl = NL $initialOriginal
$initialAnchor = "      [*] = ;   if (_unit_IsWaterUnit(myHnd)) then" + $initialNl + "      [*] = ;   begin"
$initialBlock = @'
      [*] = ;      // NavalRamming v7: type-specific gradual ship acceleration; no boost to vanilla top speed.
      [*] = ;      arg_obj.navalSpeedFraction := 0.35;
      [*] = ;      arg_obj.navalLastRamTime := -10000;
      [*] = ;      SetGameObjectMyTrackPointMoveStep(0.03*arg_obj.navalSpeedFraction);
      [*] = ;      Log('NavalRamming INIT ship='+basename);
'@
$initialPatched = InsertAfter $initialOriginal $initialAnchor ($initialNl + (FormatLines $initialBlock $initialNl)) 'ship initialization'

# 3. On the original server/offline unit tick, increase ship MoveStep smoothly.
# Only GALLEYS deal experimental ram damage so we can test damage and cooldown safely.
$unitOriginal = LoadRef 'reference\scripts\units\unit.inc\nothing.inc'
$unitNl = NL $unitOriginal
$unitNeedle = '      [*] = ;         _misc_DoProgressOrders(myHnd, bfindbestpos);'
$unitBlock = @'
      [*] = ;         // NavalRamming v7: OFFLINE EXPERIMENT (all water units accelerate; only galleys ram).
      [*] = ;         if (_net_IsOffline) and (bplayable) and (not arg_obj.bdead) and (TObjProp(pobjprop).media=gc_obj_media_water) then
      [*] = ;         begin
      [*] = ;            var navalMoving : Boolean = GetGameObjectMyTrackPointMovementModeInt<>0;
      [*] = ;            var navalOldSpeed : Float = arg_obj.navalSpeedFraction;
      [*] = ;            // All timings are game-time units. 35% -> 100% of original MoveStep.
      [*] = ;            var navalAccelTime : Float = 5;
      [*] = ;            case TObjProp(pobjprop).sid of
      [*] = ;               'galley', 'chaika', 'yacht', 'yachttur' : navalAccelTime := 4;
      [*] = ;               'frigate', 'xebec' : navalAccelTime := 6;
      [*] = ;               'battleship', 'ferry' : navalAccelTime := 8;
      [*] = ;               'fishboat' : navalAccelTime := 5;
      [*] = ;            end;
      [*] = ;            if navalMoving then
      [*] = ;            arg_obj.navalSpeedFraction := Clamp(arg_obj.navalSpeedFraction+MaxFloat(0, deltatime)*0.65/navalAccelTime, 0.35, 1.0)
      [*] = ;            else
      [*] = ;            arg_obj.navalSpeedFraction := 0.35;
      [*] = ;            // Never increase MoveStep above its original stock value.
      [*] = ;            SetGameObjectMyTrackPointMoveStep(0.03*arg_obj.navalSpeedFraction);
      [*] = ;            if navalMoving then
      [*] = ;            begin
      [*] = ;               if navalOldSpeed<=0.35 then
      [*] = ;               Log('NavalRamming ACCEL_START ship='+TObjProp(pobjprop).sid+' target='+FloatToStr(navalAccelTime)+' t='+FloatToStr(gametime));
      [*] = ;               if (navalOldSpeed<0.50) and (arg_obj.navalSpeedFraction>=0.50) then
      [*] = ;               Log('NavalRamming ACCEL_50 ship='+TObjProp(pobjprop).sid+' t='+FloatToStr(gametime));
      [*] = ;               if (navalOldSpeed<0.75) and (arg_obj.navalSpeedFraction>=0.75) then
      [*] = ;               Log('NavalRamming ACCEL_75 ship='+TObjProp(pobjprop).sid+' t='+FloatToStr(gametime));
      [*] = ;               if (navalOldSpeed<1) and (arg_obj.navalSpeedFraction>=1) then
      [*] = ;               Log('NavalRamming FULL_SPEED ship='+TObjProp(pobjprop).sid+' t='+FloatToStr(gametime));
      [*] = ;            end;
      [*] = ;
      [*] = ;            if (TObjProp(pobjprop).sid='galley') and navalMoving and (arg_obj.navalSpeedFraction>=0.55) and (gametime>arg_obj.navalLastRamTime+5) and (progresstick mod 4=0) then
      [*] = ;            begin
      [*] = ;               var navalX : Float = GetGameObjectMyPositionX;
      [*] = ;               var navalZ : Float = GetGameObjectMyPositionZ;
      [*] = ;               var navalFrontX : Float = GetGameObjectTransformedVirtualDirectionXByHandle(myHnd);
      [*] = ;               var navalFrontZ : Float = GetGameObjectTransformedVirtualDirectionZByHandle(myHnd);
      [*] = ;               _misc_MakeListObjectsInRadius(navalX, navalZ, 7, True, 0, 0, True, gTmpIntList);
      [*] = ;               var navalI : Integer;
      [*] = ;               for navalI:=gTmpIntList.GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var navalTarget : Integer = gTmpIntList.Get(navalI);
      [*] = ;                  if (navalTarget<>0) and (navalTarget<>myHnd) then
      [*] = ;                  begin
      [*] = ;                     var navalTargetObj : Pointer = _unit_GetTObjFast(navalTarget);
      [*] = ;                     if (navalTargetObj<>nil) then
      [*] = ;                     if (TBaseObj(navalTargetObj).baseid=gc_baseid_obj) and (not TObj(navalTargetObj).bdead) then
      [*] = ;                     if ((gPlayer[arg_obj.pl].enemyplmask and gPlayer[TObj(navalTargetObj).pl].myplmask)<>0) then
      [*] = ;                     begin
      [*] = ;                        var navalTargetProp : Pointer = gObjProp[TObj(navalTargetObj).cid][TObj(navalTargetObj).id];
      [*] = ;                        if (navalTargetProp<>nil) then
      [*] = ;                        if (TObjProp(navalTargetProp).media=gc_obj_media_water) then
      [*] = ;                        begin
      [*] = ;                           var navalDX : Float = GetGameObjectPositionXByHandle(navalTarget)-navalX;
      [*] = ;                           var navalDZ : Float = GetGameObjectPositionZByHandle(navalTarget)-navalZ;
      [*] = ;                           var navalDist : Float = VectorDistance(navalX, 0, navalZ, navalX+navalDX, 0, navalZ+navalDZ);
      [*] = ;                           if navalDist>0.001 then
      [*] = ;                           begin
      [*] = ;                              var navalContact : Float = TObjProp(pobjprop).radius+TObjProp(navalTargetProp).radius;
      [*] = ;                              var navalFacing : Float = (navalFrontX*navalDX+navalFrontZ*navalDZ)/navalDist;
      [*] = ;                              if (navalFacing>0.7) and (navalDist<=navalContact+0.5) then
      [*] = ;                              begin
      [*] = ;                                 var navalDamage : Integer = Round(8000*arg_obj.navalSpeedFraction);
      [*] = ;                                 arg_obj.navalLastRamTime := gametime;
      [*] = ;                                 Log('NavalRamming HIT damage='+IntToStr(navalDamage)+' dist='+FloatToStr(navalDist)+' target='+TObjProp(navalTargetProp).sid);
      [*] = ;                                 _misc_DoDamage(myHnd, navalTarget, navalDamage, 0, gc_obj_weapon_kind_cannonball);
      [*] = ;                                 _misc_DoDamage(0, myHnd, 900, 0, gc_obj_weapon_kind_cannonball);
      [*] = ;                                 break;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
'@
$unitPatched = InsertBefore $unitOriginal $unitNeedle ((FormatLines $unitBlock $unitNl) + $unitNl) 'ship progress'

# Refuse to produce outputs until ALL three anchors have been verified.
$outGlobal = Join-Path $root 'mod\data\scripts\dmscript.global'
$outInitial = Join-Path $root 'mod\data\scripts\units\unit.inc\initial.inc'
$outUnit = Join-Path $root 'mod\data\scripts\units\unit.inc\nothing.inc'
foreach ($path in @($outGlobal, $outInitial, $outUnit)) {
    New-Item -ItemType Directory -Path (Split-Path -Parent $path) -Force | Out-Null
}
[System.IO.File]::WriteAllText($outGlobal, $globalPatched, $enc)
[System.IO.File]::WriteAllText($outInitial, $initialPatched, $enc)
[System.IO.File]::WriteAllText($outUnit, $unitPatched, $enc)

Write-Host 'Generated experimental v7 files (OFFLINE ONLY):'
Write-Host $outGlobal
Write-Host $outInitial
Write-Host $outUnit
Write-Host 'IMPORTANT: copy ALL THREE into NavalRamming/data/scripts/ preserving paths.'
Write-Host 'Use a NEW offline editor scenario; save compatibility and multiplayer are not verified.'
Write-Host 'Check log for NavalRamming ACCEL_START, ACCEL_50, ACCEL_75, FULL_SPEED, HIT.'
