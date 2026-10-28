# Gameplay Overhaul Phase 1. Generates overrides; leaves reference/ untouched.
# Based on the NavalRamming v7 experimental generator. Offline editor test ONLY.
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$enc = [System.Text.Encoding]::GetEncoding(28591)
function LoadText([string]$path) {
    if (-not (Test-Path -LiteralPath $path)) { throw "Missing file: $path" }
    return [System.IO.File]::ReadAllText($path, $enc)
}
function GetNL([string]$str) {
    if ($str.Contains(([string][char]13 + [string][char]10))) {
        return ([string][char]13 + [string][char]10)
    }
    return [string][char]10
}
function ReplaceOnce([string]$src, [string]$anchor, [string]$replacement, [string]$label) {
    $matches = [regex]::Matches($src, [regex]::Escape($anchor)).Count
    if ($matches -ne 1) { throw "$label : expected one anchor, got $matches" }
    return $src.Replace($anchor, $replacement)
}
function FormatLines([string]$src, [string]$nl) {
    return (($src -split '\r?\n') -join $nl)
}

# First build the proven ship prototype v7. It generates three files in mod/.
& (Join-Path $PSScriptRoot 'build_experimental_naval_v7.ps1')
if ($LASTEXITCODE -and ($LASTEXITCODE -ne 0)) { throw "Ship generator failed." }

$outGlobal = Join-Path $root 'mod\data\scripts\dmscript.global'
$outInitial = Join-Path $root 'mod\data\scripts\units\unit.inc\initial.inc'
$outTick = Join-Path $root 'mod\data\scripts\units\unit.inc\nothing.inc'
$outUnit = Join-Path $root 'mod\data\scripts\lib\unit.script'
$outDamage = Join-Path $root 'mod\data\scripts\lib\miscext2.script'
$outCountry = Join-Path $root 'mod\data\scripts\lib\country.script'

# Cav acceleration/charge state is serialized in TObj. No old saves/multiplayer.
$g = LoadText $outGlobal
$nl = GetNL $g
$gExtra = @'
      // Gameplay Overhaul phase1: cavalry run-up and short-lived enemy presence.
      cavalrySpeedFraction = Float
      cavalryChargeBoost = Float
      cavalryThreatUntil = Float
      cavalryLastHitTime = Float
'@
$g = ReplaceOnce $g '      navalLastRamTime = Float' ('      navalLastRamTime = Float' + $nl + (FormatLines $gExtra $nl)) 'TObj cavalry fields'

# Unit stat balance and a narrower artillery cooldown; keep game-source defaults for other classes.
$u = LoadText (Join-Path $root 'reference\scripts\lib\unit.script')
$nlU = GetNL $u
$uPost = @'
      // Gameplay Overhaul phase1. Combat movement/cost modifiers deliberately not here.
      if objprop.usage=gc_obj_usage_tower then
      objbase.maxhp := objbase.maxhp*3;
      if objprop.usage=gc_obj_usage_hardwall then
      objbase.maxhp := objbase.maxhp*2;
      if objprop.usage=gc_obj_usage_weakwall then
      objbase.maxhp := Round(objbase.maxhp*1.5);
      if objprop.sid='galley' then
      objprop.weapon[1].enabled := False; // Remove the galley's mortar weapon
      if (objprop.sid='cannon') or (objprop.sid='howitzer') or (objprop.sid='mortar') or (objprop.sid='supermortar') then
      objbase.weapon[0].pause := objbase.weapon[0].pause*1.5;
'@
$u = ReplaceOnce $u '      // post processing' ((FormatLines $uPost $nlU) + $nlU + '      // post processing') 'Unit postprocessing'
$pausePattern = '      TObj(pobj).attackdelay := attpause;'
$pauseInsert = @'
      // Only land combat units outside a formation fire 35% slower (ships and artillery cannot form squads).
      if (not TObjProp(pobjprop).bbuilding) and (TObjProp(pobjprop).media<>gc_obj_media_water) and (not TObjProp(pobjprop).bArtillery) and (attpause>0) then
      begin
         var pActualFormation : Pointer = nil;
         if TObj(pobj).squad>=0 then
         pActualFormation := gPlayer[TObj(pobj).pl].squads.Get(TObj(pobj).squad);
         if (pActualFormation=nil) then
         attpause := attpause/0.65
         else
         if TSquad(pActualFormation).fFormation<0 then
         attpause := attpause/0.65;
      end;
'@
$u = ReplaceOnce $u $pausePattern ((FormatLines $pauseInsert $nlU) + $nlU + $pausePattern) 'Formation reload penalty'

# Damage multiplier at the actual damage entry point preserves upgraded weapon values.
$d = LoadText (Join-Path $root 'reference\scripts\lib\miscext2.script')
$nlD = GetNL $d
$dInsert = @'
            // Gameplay Overhaul phase1: reduce damage from mobile attackers to 62.5%.
            // Land troops without an actual formation additionally lose 35%. Ships and artillery are exempt.
            // Healing and damage not attributed to a living attacker stay unchanged.
            if (pobj<>nil) and (weapkind<>gc_obj_weapon_kind_heal) then
            begin
               var pOverhaulAttackerProp : Pointer = gObjProp[TObj(pobj).cid][TObj(pobj).id];
               if (not TObjProp(pOverhaulAttackerProp).bbuilding) then
               begin
                  damage := Max(1, Round(damage*0.625));
                  var pOverhaulSquad : Pointer = nil;
                  if TObj(pobj).squad>=0 then
                  pOverhaulSquad := gPlayer[TObj(pobj).pl].squads.Get(TObj(pobj).squad);
                  if (TObjProp(pOverhaulAttackerProp).media<>gc_obj_media_water) and (not TObjProp(pOverhaulAttackerProp).bArtillery) then
                  begin
                     if (pOverhaulSquad=nil) then
                     damage := Max(1, Round(damage*0.65))
                     else
                     if TSquad(pOverhaulSquad).fFormation<0 then
                     damage := Max(1, Round(damage*0.65));
                  end;
               end;
            end;
'@
$d = ReplaceOnce $d '            var damage : Integer = indamage;' ('            var damage : Integer = indamage;' + $nlD + (FormatLines $dInsert $nlD)) 'Combat damage modifier'

# Remove standard mortar from build and country-member lists without deleting original assets.
$c = LoadText (Join-Path $root 'reference\scripts\lib\country.script')
$nlC = GetNL $c
$c = ReplaceOnce $c "      _country_AddMember(country, 'mortar', ind, True, gc_country_editorplace_category_artillery, 30, gc_ai_unit_mortar);" '      // Gameplay Overhaul: no standalone mortar unit.' 'mortar country member'
$c = ReplaceOnce $c "      _country_AddFixedProduceWithAccessControl(country, fixedproduceind, member, 'mortar', 3, 0, ind, csid+'bla'{, csid+'sta'}, '', '');" '      // Gameplay Overhaul: no mortar production.' 'mortar artillery depot'

# Start cavalry movement with a slow walk, and artillery with lower maximum movement.
$init = LoadText $outInitial
$nlI = GetNL $init
$initInsert = @'
      [*] = ;// Gameplay Overhaul phase1: movement for horses and artillery.
      [*] = ;if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_hardhorse) or (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_fasthorse) or (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_horseshooter) then
      [*] = ;begin
      [*] = ;   arg_obj.cavalrySpeedFraction := 0.35;
      [*] = ;   arg_obj.cavalryChargeBoost := 1;
      [*] = ;   arg_obj.cavalryThreatUntil := -10000;
      [*] = ;   arg_obj.cavalryLastHitTime := -10000;
      [*] = ;   SetGameObjectMyTrackPointMoveStep(0.03*arg_obj.cavalrySpeedFraction);
      [*] = ;end;
      [*] = ;if (basename='cannon') or (basename='howitzer') then
      [*] = ;SetGameObjectMyTrackPointMoveStep(0.018);
'@
$init = ReplaceOnce $init '      [*] = ;SwitchTo(''Nothing'');' ((FormatLines $initInsert $nlI) + $nlI + '      [*] = ;SwitchTo(''Nothing'');') 'Unit Initial anchor'

# Charging: only formed mounted groups; heavy cavalry receives a 2.5x temporary
# MOVEMENT multiplier around enemies, reached smoothly. Initial mount acceleration
# is 4s light / 6s heavy. One first-impact hit per mount per cooldown.
$tick = LoadText $outTick
$nlT = GetNL $tick
$tickInsert = @'
      [*] = ;         // Gameplay Overhaul phase1: OFFLINE cavalry movement and charge prototype.
      [*] = ;         if (_net_IsOffline) and (bplayable) and (not arg_obj.bdead) then
      [*] = ;         if (TObjProp(pobjprop).usage=gc_obj_usage_hardhorse) or (TObjProp(pobjprop).usage=gc_obj_usage_fasthorse) or (TObjProp(pobjprop).usage=gc_obj_usage_horseshooter) then
      [*] = ;         begin
      [*] = ;            var cavMoving : Boolean = GetGameObjectMyTrackPointMovementModeInt<>0;
      [*] = ;            var cavSquad : Pointer = nil;
      [*] = ;            if arg_obj.squad>=0 then
      [*] = ;            cavSquad := gPlayer[arg_obj.pl].squads.Get(arg_obj.squad);
      [*] = ;            var cavFormed : Boolean = False;
      [*] = ;            if cavSquad<>nil then
      [*] = ;            cavFormed := TSquad(cavSquad).fFormation>=0;
      [*] = ;            var cavIsHeavy : Boolean = TObjProp(pobjprop).usage=gc_obj_usage_hardhorse;
      [*] = ;            var cavStartTime : Float = 4;
      [*] = ;            if cavIsHeavy then cavStartTime := 6;
      [*] = ;            if cavMoving then
      [*] = ;            arg_obj.cavalrySpeedFraction := Clamp(arg_obj.cavalrySpeedFraction+MaxFloat(0, deltatime)*0.65/cavStartTime, 0.35, 1.0)
      [*] = ;            else
      [*] = ;            arg_obj.cavalrySpeedFraction := 0.35;
      [*] = ;
      [*] = ;            if cavMoving and cavFormed and (progresstick mod 4=0) then
      [*] = ;            begin
      [*] = ;               var cavX : Float = GetGameObjectMyPositionX;
      [*] = ;               var cavZ : Float = GetGameObjectMyPositionZ;
      [*] = ;               var cavFX : Float = GetGameObjectTransformedVirtualDirectionXByHandle(myHnd);
      [*] = ;               var cavFZ : Float = GetGameObjectTransformedVirtualDirectionZByHandle(myHnd);
      [*] = ;               _misc_MakeListObjectsInRadius(cavX, cavZ, 12, True, 0, 0, True, gTmpIntList);
      [*] = ;               var cavI : Integer;
      [*] = ;               for cavI:=gTmpIntList.GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var cavTarget : Integer = gTmpIntList.Get(cavI);
      [*] = ;                  if (cavTarget<>0) and (cavTarget<>myHnd) then
      [*] = ;                  begin
      [*] = ;                     var cavObj : Pointer = _unit_GetTObjFast(cavTarget);
      [*] = ;                     if (cavObj<>nil) then
      [*] = ;                     if (TBaseObj(cavObj).baseid=gc_baseid_obj) and (not TObj(cavObj).bdead) then
      [*] = ;                     if ((gPlayer[arg_obj.pl].enemyplmask and gPlayer[TObj(cavObj).pl].myplmask)<>0) then
      [*] = ;                     begin
      [*] = ;                        var cavProp : Pointer = gObjProp[TObj(cavObj).cid][TObj(cavObj).id];
      [*] = ;                        if (cavProp<>nil) and (TObjProp(cavProp).material=gc_obj_material_body) then
      [*] = ;                        begin
      [*] = ;                           var cavDX : Float = GetGameObjectPositionXByHandle(cavTarget)-cavX;
      [*] = ;                           var cavDZ : Float = GetGameObjectPositionZByHandle(cavTarget)-cavZ;
      [*] = ;                           var cavDist : Float = VectorDistance(cavX, 0, cavZ, cavX+cavDX, 0, cavZ+cavDZ);
      [*] = ;                           if cavDist>0.001 then
      [*] = ;                           begin
      [*] = ;                              var cavFront : Float = (cavFX*cavDX+cavFZ*cavDZ)/cavDist;
      [*] = ;                              if cavFront>0.70 then
      [*] = ;                              begin
      [*] = ;                                 // Pikes block the charge: never apply ram damage to pike formations.
      [*] = ;                                 var cavIsPike : Boolean = (TObjProp(cavProp).usage=gc_obj_usage_lightinfantry) and (TObjProp(cavProp).weapon[0].kind=gc_obj_weapon_kind_pike);
      [*] = ;                                 var cavEnemyFormed : Boolean = False;
      [*] = ;                                 if TObj(cavObj).squad>=0 then
      [*] = ;                                 begin
      [*] = ;                                    var cavEnemySquad : Pointer = gPlayer[TObj(cavObj).pl].squads.Get(TObj(cavObj).squad);
      [*] = ;                                    if cavEnemySquad<>nil then
      [*] = ;                                    cavEnemyFormed := TSquad(cavEnemySquad).fFormation>=0;
      [*] = ;                                 end;
      [*] = ;                                 if (not cavIsPike) then
      [*] = ;                                 begin
      [*] = ;                                    arg_obj.cavalryThreatUntil := gametime+1;
      [*] = ;                                    if (cavDist <= (TObjProp(pobjprop).radius+TObjProp(cavProp).radius+0.5)) and (arg_obj.cavalrySpeedFraction>=0.65) and (arg_obj.cavalryChargeBoost>=1) and (gametime>arg_obj.cavalryLastHitTime+5) then
      [*] = ;                                    begin
      [*] = ;                                       var cavHit : Integer = 250;
      [*] = ;                                       if not cavEnemyFormed then cavHit := 500000;
      [*] = ;                                       arg_obj.cavalryLastHitTime := gametime;
      [*] = ;                                       Log('CavalryCharge HIT damage='+IntToStr(cavHit)+' target='+TObjProp(cavProp).sid+' dist='+FloatToStr(cavDist));
      [*] = ;                                       _misc_DoDamage(myHnd, cavTarget, cavHit, 0, gc_obj_weapon_kind_pike);
      [*] = ;                                    end;
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            // Heavy formed cavalry increases speed to 2.5x near a hostile infantry target.
      [*] = ;            var cavDesiredBoost : Float = 1;
      [*] = ;            if cavIsHeavy and cavFormed and cavMoving and (gametime<arg_obj.cavalryThreatUntil) then
      [*] = ;            cavDesiredBoost := 2.5;
      [*] = ;            if arg_obj.cavalryChargeBoost<cavDesiredBoost then
      [*] = ;            arg_obj.cavalryChargeBoost := Clamp(arg_obj.cavalryChargeBoost+MaxFloat(0, deltatime)*0.5, 1, cavDesiredBoost)
      [*] = ;            else
      [*] = ;            arg_obj.cavalryChargeBoost := MaxFloat(cavDesiredBoost, arg_obj.cavalryChargeBoost-MaxFloat(0, deltatime)*0.75);
      [*] = ;            SetGameObjectMyTrackPointMoveStep(0.03*arg_obj.cavalrySpeedFraction*arg_obj.cavalryChargeBoost);
      [*] = ;         end;
'@
$tick = ReplaceOnce $tick '      [*] = ;         _misc_DoProgressOrders(myHnd, bfindbestpos);' ((FormatLines $tickInsert $nlT) + $nlT + '      [*] = ;         _misc_DoProgressOrders(myHnd, bfindbestpos);') 'Cavalry progress hook'

# AI artillery cap: at most 10 artillery across cannon/howitzer/mortar/multi-barrel
# in the normal AI economy production plan. Not a hard global ownership cap.
$outAI = Join-Path $root 'mod\data\scripts\units\global.inc\progresseconomicai.inc'
$ai = LoadText (Join-Path $root 'reference\scripts\units\global.inc\progresseconomicai.inc')
$nlAI = GetNL $ai
$ai = ReplaceOnce $ai '      [*] = ;   var cannon_count : integer = ClampInt(num_depo*6, 6, 30);' '      [*] = ;   var cannon_count : integer = 10;' 'AI artillery cannon quota'
$ai = ReplaceOnce $ai '      [*] = ;      howitzer_count := ClampInt(num_depo*2, 2, 8);' '      [*] = ;      howitzer_count := 10;' 'AI howitzer quota'
$aiMortar = @'
      [*] = ;      var curcount : Integer = _ai_GetUnitCountAnyFaction(plind, gc_ai_unit_mortar);
      [*] = ;      var ordercount : Integer = Min(num_depo_my_cid, mortar_count-curcount-ordered_mortar_count);
      [*] = ;      ordercount := Max(ordercount, 0);
      [*] = ;      ordered_mortar_count := ordered_mortar_count+ordercount;
      [*] = ;      if (curcount<mortar_count) then
      [*] = ;      _ai_TryUnit(plind, cid, gc_ai_unit_mortar, ordercount, False);
'@
$ai = ReplaceOnce $ai (FormatLines $aiMortar $nlAI) ('      [*] = ;      var curcount : Integer;' + $nlAI + '      [*] = ;      var ordercount : Integer;') 'AI mortar production'
$ai = ReplaceOnce $ai '      [*] = ;      ordercount := Min(num_depo_my_cid*2, cannon_count-curcount-ordered_cannon_count);' '      [*] = ;      ordercount := Min(num_depo_my_cid*2, 10-_ai_GetUnitCountAnyFaction(plind, gc_ai_unit_cannon)-_ai_GetUnitCountAnyFaction(plind, gc_ai_unit_howitzer)-_ai_GetUnitCountAnyFaction(plind, gc_ai_unit_mortar)-_ai_GetUnitCountAnyFaction(plind, gc_ai_unit_mbarrel)-ordered_cannon_count-ordered_howitzer_count);' 'AI combined cannon quota'
$ai = ReplaceOnce $ai '      [*] = ;      ordercount := Min(2, howitzer_count-curcount-ordered_howitzer_count);' '      [*] = ;      ordercount := Min(2, 10-_ai_GetUnitCountAnyFaction(plind, gc_ai_unit_cannon)-_ai_GetUnitCountAnyFaction(plind, gc_ai_unit_howitzer)-_ai_GetUnitCountAnyFaction(plind, gc_ai_unit_mortar)-_ai_GetUnitCountAnyFaction(plind, gc_ai_unit_mbarrel)-ordered_cannon_count-ordered_howitzer_count);' 'AI combined howitzer quota'

# Enforce a combined ownership limit for AI artillery using native artcount[].
# This supplements (does not replace) the AI plan's order-count cap above.
$outPlayer = Join-Path $root 'mod\data\scripts\lib\player.script'
$player = LoadText (Join-Path $root 'reference\scripts\lib\player.script')
$nlPlayer = GetNL $player
$playerMarker = @'
   else
   Result := gc_result_checkaccesscontrolreq_notenabled;
end;

// _player_UpdateFOW
'@
$playerAddon = @'
   else
   Result := gc_result_checkaccesscontrolreq_notenabled;

   // Gameplay Overhaul: maximum 10 physical artillery pieces owned by a bot.
   // Normal artillery access checks still apply; humans retain original limits.
   if (Result=gc_result_checkaccesscontrolreq_ok) and gPlayer[plInd].bAI and bMember then
   if (sid='cannon') or (sid='howitzer') or (sid='mortar') or (sid='multicannon') or (sid='framegun') then
   begin
      var aiArtTotal : Integer = 0;
      var aiArtI : Integer;
      for aiArtI:=0 to gc_MaxArtilleryType-1 do
      aiArtTotal := aiArtTotal+gPlayer[plInd].artcount[aiArtI];
      if aiArtTotal>=10 then
      Result := gc_result_checkaccesscontrolreq_artlimit;
   end;
end;

// _player_UpdateFOW
'@
$player = ReplaceOnce $player (FormatLines $playerMarker $nlPlayer) (FormatLines $playerAddon $nlPlayer) 'AI artillery access gate'

# Commit outputs only after validating all anchors. A failed run can leave v7 outputs
# from the underlying generator, but it never overwrites original source files.
foreach ($target in @($outUnit, $outDamage, $outCountry, $outAI, $outPlayer)) {
    New-Item -ItemType Directory -Path (Split-Path -Parent $target) -Force | Out-Null
}
[System.IO.File]::WriteAllText($outGlobal, $g, $enc)
[System.IO.File]::WriteAllText($outInitial, $init, $enc)
[System.IO.File]::WriteAllText($outTick, $tick, $enc)
[System.IO.File]::WriteAllText($outUnit, $u, $enc)
[System.IO.File]::WriteAllText($outDamage, $d, $enc)
[System.IO.File]::WriteAllText($outCountry, $c, $enc)
[System.IO.File]::WriteAllText($outAI, $ai, $enc)
[System.IO.File]::WriteAllText($outPlayer, $player, $enc)
Write-Host 'Gameplay Overhaul phase 1 generated. UNTESTED IN GAME. OFFLINE ONLY.'
Write-Host 'Copy these EIGHT files, preserving folders:'
Write-Host $outGlobal
Write-Host $outInitial
Write-Host $outTick
Write-Host $outUnit
Write-Host $outDamage
Write-Host $outCountry
Write-Host $outAI
Write-Host $outPlayer
Write-Host 'Check logs for CavalryCharge HIT; test old NavalRamming HIT as regression.'
