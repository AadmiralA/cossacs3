# Experimental diagnostics for ALL currently implemented Gameplay Overhaul phase-1 features.
# Requires build_gameplay_overhaul_phase1.ps1; patches generated mod/data/ ONLY.
# WARNING: Detailed combat logs can be huge. Do not use in multiplayer or massive battles.
param(
  [ValidateSet('Full','Compact')][string]$LogMode = 'Full'
)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$enc = [System.Text.Encoding]::GetEncoding(28591)
function ReadMod([string]$rel) {
    $p = Join-Path $root ('mod\data\' + $rel)
    if (-not (Test-Path -LiteralPath $p)) { throw "Missing generated file: $p" }
    return [System.IO.File]::ReadAllText($p,$enc)
}
function Newline([string]$s) {
    $crlf = [string][char]13 + [string][char]10
    if ($s.Contains($crlf)) { return $crlf }
    return [string][char]10
}
function Lines([string]$s,[string]$nl) { return (($s -split '\r?\n') -join $nl) }
function ReplaceOnce([string]$s,[string]$needle,[string]$replacement,[string]$label) {
    $c=[regex]::Matches($s,[regex]::Escape($needle)).Count
    if ($c -ne 1) { throw "$label : expected 1 anchor, got $c" }
    return $s.Replace($needle,$replacement)
}
& (Join-Path $PSScriptRoot 'build_gameplay_overhaul_phase1.ps1')
if (-not $?) { throw 'Base gameplay-overhaul generator failed' }

# A) Startup flags and per-unit configuration. This shows whether changes to
# walls, towers, mortars, galley, artillery and formation damage were loaded.
$unit = ReadMod 'scripts\lib\unit.script'
$nl = Newline $unit
$cfg = @'
      // OVERHAUL_DIAG: one line per relevant unit BASE variant (not per game tick).
      if (objprop.usage=gc_obj_usage_tower) or (objprop.usage=gc_obj_usage_hardwall) or (objprop.usage=gc_obj_usage_weakwall) or
         (objprop.bartillery) or (objprop.media=gc_obj_media_water) then
      Log('OVDIAG CONFIG sid='+objprop.sid+' usage='+IntToStr(objprop.usage)+' hp='+IntToStr(objbase.maxhp)+
         ' dmg0='+IntToStr(objbase.weapon[0].damage)+' reload0='+FloatToStr(objbase.weapon[0].pause)+
         ' mortarEnabled='+BoolToStr(objprop.weapon[1].enabled)+' goldConsume='+IntToStr(objprop.consume[gc_resource_type_gold]));
'@
$unit=ReplaceOnce $unit '      // post processing' ((Lines $cfg $nl)+$nl+'      // post processing') 'Unit config diagnostic'

$pauseTarget='      TObj(pobj).attackdelay := attpause;'
$pauseProbe=@'
      // OV_DIAG: reload penalty is applied only if the attacker has NO active formation.
      if (TObj(pobj).progresstick mod 32=0) and (attpause>0) then
      begin
         var diagForm : Boolean = False;
         if TObj(pobj).squad>=0 then
         begin
            var diagSQ : Pointer = gPlayer[TObj(pobj).pl].squads.Get(TObj(pobj).squad);
            if diagSQ<>nil then diagForm := TSquad(diagSQ).fFormation>=0;
         end;
         Log('OVDIAG RELOAD sid='+TObjProp(pobjprop).sid+' formed='+BoolToStr(diagForm)+' pause='+FloatToStr(attpause)+' squad='+IntToStr(TObj(pobj).squad));
      end;
'@
$unit=ReplaceOnce $unit $pauseTarget ((Lines $pauseProbe $nl)+$nl+$pauseTarget) 'Attack reload trace'

# B) Damage after shields/protection/formation bonuses, immediately before the
# main offline HP subtraction. We log actual damage, not only the requested hit.
$damage = ReadMod 'scripts\lib\miscext2.script'
$nd=Newline $damage
$damageNeedle='                        TObj(pobj2).hp := TObj(pobj2).hp-damage;'+$nd+'                        //log('
$damageGate='True'
if ($LogMode -eq 'Compact') {
    $damageGate='(TObj(pobj).progresstick mod 32=0) or (damage>=200)'
}
$actual=@'
                        // OVDIAG HP_APPLY: final damage AFTER defense; includes charge/ship impacts.
                        if __GATE__ then
                        Log('OVDIAG HP_APPLY attacker='+GetGameObjectBaseNameByHandle(goHnd)+' target='+GetGameObjectBaseNameByHandle(trgHnd)+
                            ' weapon='+IntToStr(weapkind)+' requested='+IntToStr(indamage)+' applied='+IntToStr(damage)+
                            ' hpBefore='+IntToStr(TObj(pobj2).hp)+' hpAfter='+IntToStr(TObj(pobj2).hp-damage)+
                            ' atkSquad='+IntToStr(TObj(pobj).squad)+' tgtSquad='+IntToStr(TObj(pobj2).squad));
'@
$actual=$actual.Replace('__GATE__',$damageGate)
$damage=ReplaceOnce $damage $damageNeedle ((Lines $actual $nd)+$nd+$damageNeedle) 'Final HP damage trace'
# Mark initial reduction even where defenses later absorb it. Emits only in
# Full mode; Compact mode logs final HP damage above.
if ($LogMode -eq 'Full') {
    $reduction='                  damage := Max(1, Round(damage*0.625));'
    $add=@'
                  Log('OVDIAG DAMAGE_GLOBAL attacker='+GetGameObjectBaseNameByHandle(goHnd)+' target='+GetGameObjectBaseNameByHandle(trgHnd)+' original='+IntToStr(indamage)+' reduced='+IntToStr(damage));
'@
    $damage=ReplaceOnce $damage $reduction ($reduction+$nd+(Lines $add $nd)) 'Global damage reduction trace'
    $formationReduction='                  damage := Max(1, Round(damage*0.65))'
    $add2="                  Log('OVDIAG DAMAGE_NO_FORMATION attacker='+GetGameObjectBaseNameByHandle(goHnd)+' reduced='+IntToStr(damage));"
    # Do not append a statement between an IF consequent and ELSE. Patch the
    # COMMON 'damage' result later in HP_APPLY instead.
}

# C) Per-object live initialization for cavalry and artillery.
$initial = ReadMod 'scripts\units\unit.inc\initial.inc'
$ni=Newline $initial
$initCav='      [*] = ;   SetGameObjectMyTrackPointMoveStep(0.03*arg_obj.cavalrySpeedFraction);'
$initCavLog="      [*] = ;   Log('OVDIAG CAV_INIT sid='+basename+' player='+IntToStr(arg_obj.pl)+' squad='+IntToStr(arg_obj.squad)+' step='+FloatToStr(0.03*arg_obj.cavalrySpeedFraction));"
$initial=ReplaceOnce $initial $initCav ($initCav+$ni+$initCavLog) 'Cavalry initialization'
$initGun="      [*] = ;SetGameObjectMyTrackPointMoveStep(0.018);"
$initGunReplacement=@'
      [*] = ;begin
      [*] = ;   SetGameObjectMyTrackPointMoveStep(0.018);
      [*] = ;   Log('OVDIAG GUN_INIT sid='+basename+' step=0.018');
      [*] = ;end;
'@
$initial=ReplaceOnce $initial $initGun (Lines $initGunReplacement $ni) 'Artillery movement init'

# D) Live cavalry: reasons for failure plus speed evolution, near target,
# pike detection, charge boost, trigger and resulting applied damage.
$tick=ReadMod 'scripts\units\unit.inc\nothing.inc'
$nt=Newline $tick
$probes=@(
    @('            if cavMoving then',
      @'
      [*] = ;            if (progresstick mod 64=0) then
      [*] = ;            Log('OVDIAG CAV_STATE sid='+TObjProp(pobjprop).sid+' squad='+IntToStr(arg_obj.squad)+' formed='+BoolToStr(cavFormed)+' moving='+BoolToStr(cavMoving)+' baseSpeed='+FloatToStr(arg_obj.cavalrySpeedFraction)+' chargeSpeed='+FloatToStr(arg_obj.cavalryChargeBoost)+' t='+FloatToStr(gametime));
'@,
      'Cavalry periodic state'),
    @('      [*] = ;            if cavMoving then', '', '')
)
# Unique anchors in stage-1 injected text.
$movingAnchor='      [*] = ;            if cavMoving then'
$stateLog=@'
      [*] = ;            if (progresstick mod 64=0) then
      [*] = ;            Log('OVDIAG CAV_STATE sid='+TObjProp(pobjprop).sid+' squad='+IntToStr(arg_obj.squad)+' formed='+BoolToStr(cavFormed)+' moving='+BoolToStr(cavMoving)+' baseSpeed='+FloatToStr(arg_obj.cavalrySpeedFraction)+' chargeSpeed='+FloatToStr(arg_obj.cavalryChargeBoost)+' t='+FloatToStr(gametime));
      [*] = ;            if cavMoving and (not cavFormed) and (progresstick mod 64=0) then
      [*] = ;            Log('OVDIAG CAV_BLOCK reason=NO_FORMATION sid='+TObjProp(pobjprop).sid+' squad='+IntToStr(arg_obj.squad));
'@
$tick=ReplaceOnce $tick $movingAnchor ((Lines $stateLog $nt)+$nt+$movingAnchor) 'Cavalry periodic motion and formation'

$targetCheck='      [*] = ;                              if cavFront>0.70 then'
$targetLog=@'
      [*] = ;                              if (progresstick mod 64=0) then
      [*] = ;                              Log('OVDIAG CAV_ENEMY target='+TObjProp(cavProp).sid+' distance='+FloatToStr(cavDist)+' direction='+FloatToStr(cavFront)+' formed='+BoolToStr(cavFormed));
'@
$tick=ReplaceOnce $tick $targetCheck ((Lines $targetLog $nt)+$nt+$targetCheck) 'Cavalry target search'
$blockAnchor='      [*] = ;                                 if (not cavIsPike) then'
$pikeLog=@'
      [*] = ;                                 if cavIsPike and (cavDist<4) and (progresstick mod 32=0) then
      [*] = ;                                 Log('OVDIAG CAV_BLOCK reason=PIKES target='+TObjProp(cavProp).sid+' distance='+FloatToStr(cavDist));
      [*] = ;                                 if (not cavIsPike) and (cavDist<4) and (progresstick mod 32=0) then
      [*] = ;                                 Log('OVDIAG CAV_CONTACT target='+TObjProp(cavProp).sid+' formed='+BoolToStr(cavEnemyFormed)+' distance='+FloatToStr(cavDist)+' speed='+FloatToStr(arg_obj.cavalrySpeedFraction)+' cooldownUntil='+FloatToStr(arg_obj.cavalryLastHitTime+5));
'@
$tick=ReplaceOnce $tick $blockAnchor ((Lines $pikeLog $nt)+$nt+$blockAnchor) 'Pike and contact reasons'

$chargeOld='      [*] = ;            if arg_obj.cavalryChargeBoost<cavDesiredBoost then'
$boostPre='      [*] = ;            var cavBoostWas : Float = arg_obj.cavalryChargeBoost;'
$tick=ReplaceOnce $tick $chargeOld ($boostPre+$nt+$chargeOld) 'Speed boost prior'
$boostNew='      [*] = ;            SetGameObjectMyTrackPointMoveStep(0.03*arg_obj.cavalrySpeedFraction*arg_obj.cavalryChargeBoost);'
$boostPost=@'
      [*] = ;            if (cavBoostWas<1.5) and (arg_obj.cavalryChargeBoost>=1.5) then
      [*] = ;            Log('OVDIAG CAV_BOOST_START sid='+TObjProp(pobjprop).sid+' boost='+FloatToStr(arg_obj.cavalryChargeBoost));
      [*] = ;            if (cavBoostWas<2.4) and (arg_obj.cavalryChargeBoost>=2.4) then
      [*] = ;            Log('OVDIAG CAV_BOOST_MAX sid='+TObjProp(pobjprop).sid+' boost='+FloatToStr(arg_obj.cavalryChargeBoost));
      [*] = ;            if (cavBoostWas>1.05) and (arg_obj.cavalryChargeBoost<=1.05) then
      [*] = ;            Log('OVDIAG CAV_BOOST_END sid='+TObjProp(pobjprop).sid);
'@
$tick=ReplaceOnce $tick $boostNew ($boostNew+$nt+(Lines $boostPost $nt)) 'Cavalry boost transitions'
$chargeTrigger="      [*] = ;                                       Log('CavalryCharge HIT damage='+IntToStr(cavHit)+' target='+TObjProp(cavProp).sid+' dist='+FloatToStr(cavDist));"
$chargeExtra="      [*] = ;                                       Log('OVDIAG CAV_HIT_CHECK attacker='+TObjProp(pobjprop).sid+' target='+TObjProp(cavProp).sid+' targetHP='+IntToStr(TObj(cavObj).hp)+' formedTarget='+BoolToStr(cavEnemyFormed)+' facing='+FloatToStr(cavFront)+' boost='+FloatToStr(arg_obj.cavalryChargeBoost)+' raw='+IntToStr(cavHit));"
$tick=ReplaceOnce $tick $chargeTrigger ($chargeExtra+$nt+$chargeTrigger) 'Cavalry impact detail'

# E) Bot artillery production: trace actual orders and ownership-limit refusal.
$ai=ReadMod 'scripts\units\global.inc\progresseconomicai.inc'
$na=Newline $ai
foreach ($sid in @('cannon','howitzer')) {
    $anchor="      [*] = ;      _ai_TryUnit(plind, cid, gc_ai_unit_$sid, ordercount, False);"
    $log="      [*] = ;      if ordercount>0 then Log('OVDIAG AI_ORDER sid=$sid count='+IntToStr(ordercount)+' player='+IntToStr(plind));"
    $ai=ReplaceOnce $ai $anchor ($log+$na+$anchor) "AI $sid order trace"
}

$player=ReadMod 'scripts\lib\player.script'
$np=Newline $player
$limit='      if aiArtTotal>=10 then'+$np+'      Result := gc_result_checkaccesscontrolreq_artlimit;'
$limitNew=@'
      if aiArtTotal>=10 then
      begin
         Log('OVDIAG AI_ART_LIMIT player='+IntToStr(plInd)+' total='+IntToStr(aiArtTotal)+' sid='+sid+' blocked=1');
         Result := gc_result_checkaccesscontrolreq_artlimit;
      end;
'@
$player=ReplaceOnce $player $limit (Lines $limitNew $np) 'AI artillery hard cap trace'

# F) Start-of-session country/production confirmation.
$country=ReadMod 'scripts\lib\country.script'
$nc=Newline $country
$removed='      // Gameplay Overhaul: no standalone mortar unit.'
$country=ReplaceOnce $country $removed ($removed+$nc+"      Log('OVDIAG MORTAR_MEMBER_DISABLED country='+IntToStr(country.id));") 'Mortar country-init trace'

# Write only after all unique markers have been verified.
$out=@{
 'scripts\lib\unit.script'=$unit
 'scripts\lib\miscext2.script'=$damage
 'scripts\lib\player.script'=$player
 'scripts\lib\country.script'=$country
 'scripts\units\unit.inc\initial.inc'=$initial
 'scripts\units\unit.inc\nothing.inc'=$tick
 'scripts\units\global.inc\progresseconomicai.inc'=$ai
}
foreach($entry in $out.GetEnumerator()) {
    $file=Join-Path $root ('mod\data\'+$entry.Key)
    [System.IO.File]::WriteAllText($file,$entry.Value,$enc)
}
Write-Host ('Gameplay Overhaul DIAGNOSTICS generated, mode='+$LogMode+', files=' + $out.Count)
Write-Host 'Deploy all EIGHT stage-1 files including scripts/dmscript.global.'
Write-Host 'Only NEW offline test scenario. Watch for OVDIAG CONFIG, CAV_*, HP_APPLY, RELOAD, AI_*, GUN_INIT.'
Write-Host 'WARNING: Full mode logs almost every recorded combat hit. Use test maps with FEW units.'
