# NavalRamming proximity probe v3. Does not deal damage.
# Generates local mod files from reference/ without changing original files.
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$enc = [System.Text.Encoding]::GetEncoding(28591) # lossless for source bytes + ASCII additions

function Read-Reference([string]$relative) {
    $f = Join-Path $root $relative
    if (-not (Test-Path -LiteralPath $f)) { throw "Reference file missing: $f" }
    return [System.IO.File]::ReadAllText($f, $enc)
}
function Source-Newline([string]$s) {
    $crlf = [string][char]13 + [string][char]10
    if ($s.Contains($crlf)) { return $crlf }
    return [string][char]10
}
function Patch-Once([string]$original, [string]$anchor, [string]$addition, [string]$name) {
    $matches = [regex]::Matches($original, [regex]::Escape($anchor)).Count
    if ($matches -ne 1) {
        throw "$name expected one anchor, got $matches. Nothing was written."
    }
    return $original.Replace($anchor, $addition + $anchor)
}

$unitOriginal = Read-Reference 'reference\scripts\units\unit.inc\nothing.inc'
$initialOriginal = Read-Reference 'reference\scripts\units\unit.inc\initial.inc'
$unitNl = Source-Newline $unitOriginal
$initialNl = Source-Newline $initialOriginal

# Called on the authoritative offline game loop, not on the unreliable ship collision callback.
# The probe scans only galleys, 1 in 32 ticks, and logs at most one nearby water unit.
$unitAnchor = '      [*] = ;         _misc_DoProgressOrders(myHnd, bfindbestpos);'
$unitProbe = @'
      [*] = ;         // NavalRamming V3: diagnostic only. Scan on server-side unit progress.
      [*] = ;         if (_net_IsOffline) and (bplayable) and (not arg_obj.bdead) and (TObjProp(pobjprop).sid='galley') and (progresstick mod 32=0) then
      [*] = ;         begin
      [*] = ;            var ramX : Float = GetGameObjectMyPositionX;
      [*] = ;            var ramZ : Float = GetGameObjectMyPositionZ;
      [*] = ;            var ramUpX, ramUpY, ramUpZ, ramDirX, ramDirY, ramDirZ : Float;
      [*] = ;            GetGameObjectOrientationByHandle(myHnd, ramUpX, ramUpY, ramUpZ, ramDirX, ramDirY, ramDirZ);
      [*] = ;            _misc_MakeListObjectsInRadius(ramX, ramZ, 12, True, 0, 0, True, gTmpIntList);
      [*] = ;            var ramI : Integer;
      [*] = ;            for ramI:=gTmpIntList.GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var ramTargetHnd : Integer = gTmpIntList.Get(ramI);
      [*] = ;               if (ramTargetHnd<>0) and (ramTargetHnd<>myHnd) then
      [*] = ;               begin
      [*] = ;                  var ramTargetObj : Pointer = _unit_GetTObjFast(ramTargetHnd);
      [*] = ;                  if (ramTargetObj<>nil) then
      [*] = ;                  if (TBaseObj(ramTargetObj).baseid=gc_baseid_obj) and (not TObj(ramTargetObj).bdead) then
      [*] = ;                  begin
      [*] = ;                     var ramTargetProp : Pointer = gObjProp[TObj(ramTargetObj).cid][TObj(ramTargetObj).id];
      [*] = ;                     if (ramTargetProp<>nil) then
      [*] = ;                     if (TObjProp(ramTargetProp).media=gc_obj_media_water) then
      [*] = ;                     begin
      [*] = ;                        var ramDX : Float = GetGameObjectPositionXByHandle(ramTargetHnd)-ramX;
      [*] = ;                        var ramDZ : Float = GetGameObjectPositionZByHandle(ramTargetHnd)-ramZ;
      [*] = ;                        var ramDistance : Float = VectorDistance(ramX, 0, ramZ, ramX+ramDX, 0, ramZ+ramDZ);
      [*] = ;                        if ramDistance>0.001 then
      [*] = ;                        begin
      [*] = ;                           var ramContact : Float = TObjProp(pobjprop).radius+TObjProp(ramTargetProp).radius;
      [*] = ;                           var ramFront : Float = (ramDirX*ramDX+ramDirZ*ramDZ)/ramDistance;
      [*] = ;                           var ramMove : Integer = GetGameObjectMyTrackPointMovementModeInt;
      [*] = ;                           var ramEnemy : Integer = 0;
      [*] = ;                           if ((gPlayer[arg_obj.pl].enemyplmask and gPlayer[TObj(ramTargetObj).pl].myplmask)<>0) then
      [*] = ;                           ramEnemy := 1;
      [*] = ;                           Log('NavalRamming NEAR dist='+FloatToStr(ramDistance)+' contact='+FloatToStr(ramContact)+' front='+FloatToStr(ramFront)+' moving='+IntToStr(ramMove)+' enemy='+IntToStr(ramEnemy));
      [*] = ;                           if (ramEnemy=1) and (ramMove<>0) and (ramFront>0.65) and (ramDistance<=ramContact+0.5) then
      [*] = ;                           Log('NavalRamming RAM_CANDIDATE ship=galley target='+TObjProp(ramTargetProp).sid);
      [*] = ;                           break;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
'@
$unitInsert = ($unitProbe -split '\r?\n') -join $unitNl
$unitPatched = Patch-Once $unitOriginal $unitAnchor ($unitInsert + $unitNl) 'unit-progress'

# Independent proof that our modified initializer was loaded.
$initialAnchor = "      [*] = ;   if (_unit_IsWaterUnit(myHnd)) then" + $initialNl + "      [*] = ;   begin"
$initialProbe = "      [*] = ;      Log('NavalRamming INIT ship='+basename);"
$initialPatched = Patch-Once $initialOriginal $initialAnchor ($initialNl + $initialProbe + $initialNl) 'initial-state'

# Only write after validating both patch anchors.
$outUnit = Join-Path $root 'mod\data\scripts\units\unit.inc\nothing.inc'
$outInitial = Join-Path $root 'mod\data\scripts\units\unit.inc\initial.inc'
New-Item -ItemType Directory -Path (Split-Path -Parent $outUnit) -Force | Out-Null
[System.IO.File]::WriteAllText($outUnit, $unitPatched, $enc)
[System.IO.File]::WriteAllText($outInitial, $initialPatched, $enc)
Write-Host 'NavalRamming proximity probe v3 generated (NO DAMAGE):'
Write-Host $outUnit
Write-Host $outInitial
Write-Host 'Copy BOTH files to mods/NavalRamming/data/scripts/units/unit.inc/'
Write-Host 'Search the newest log for NavalRamming INIT, NEAR, RAM_CANDIDATE.'
