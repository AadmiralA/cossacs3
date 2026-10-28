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

# A) Do NOT trace base templates on every _unit_InitBase call: our previous
# version generated >18,000 duplicate CONFIG entries per match. Instead, log
# the effective configuration ONCE PER SPAWN for the relevant on-map object.
$unit = ReadMod 'scripts\lib\unit.script'
$nl = Newline $unit
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
if ($LogMode -eq 'Full') {
    $pauseProbe=$pauseProbe.Replace('(TObj(pobj).progresstick mod 32=0) and (attpause>0)', '(attpause>0)')
}
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
                            ' atkSquad='+IntToStr(TObj(pobj).squad)+' tgtSquad='+IntToStr(TObj(pobj2).squad)+
                            ' atkPl='+IntToStr(TObj(pobj).pl)+' tgtPl='+IntToStr(TObj(pobj2).pl)+
                            ' atkHnd='+IntToStr(goHnd)+' tgtHnd='+IntToStr(trgHnd));
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
$spawnAnchor="      [*] = ;SwitchTo('Nothing');"
$spawnProbe=@'
      [*] = ;// OVDIAG: config only for spawned relevant objects, not repeated country prototypes.
      [*] = ;var diagObjProp : Pointer = gObjProp[arg_obj.cid][arg_obj.id];
      [*] = ;if (TObjProp(diagObjProp).bArtillery) or (TObjProp(diagObjProp).media=gc_obj_media_water) or
      [*] = ;   (TObjProp(diagObjProp).usage=gc_obj_usage_tower) or (TObjProp(diagObjProp).usage=gc_obj_usage_hardwall) or
      [*] = ;   (TObjProp(diagObjProp).usage=gc_obj_usage_weakwall) then
      [*] = ;begin
      [*] = ;   var diagObjBase : Pointer = gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id];
      [*] = ;   Log('OVDIAG CONFIG_SPAWN hnd='+IntToStr(myHnd)+' sid='+basename+' player='+IntToStr(arg_obj.pl)+' hp='+IntToStr(TObjBase(diagObjBase).maxhp)+
      [*] = ;       ' weapon0Damage='+IntToStr(TObjBase(diagObjBase).weapon[0].damage)+
      [*] = ;       ' reload0='+FloatToStr(TObjBase(diagObjBase).weapon[0].pause)+
      [*] = ;       ' reload1='+FloatToStr(TObjBase(diagObjBase).weapon[1].pause)+
      [*] = ;       ' weapon1Enabled='+BoolToStr(TObjProp(diagObjProp).weapon[1].enabled)+
      [*] = ;       ' goldConsume='+IntToStr(TObjProp(diagObjProp).consume[gc_resource_type_gold]));
      [*] = ;end;
'@
$initial=ReplaceOnce $initial $spawnAnchor ((Lines $spawnProbe $ni)+$ni+$spawnAnchor) 'Relevant unit spawn config'

$initCav='      [*] = ;   SetGameObjectMyTrackPointMoveStep(0.045*arg_obj.cavalrySpeedFraction);'
$initCavLog="      [*] = ;   Log('OVDIAG CAV_INIT hnd='+IntToStr(myHnd)+' sid='+basename+' time='+FloatToStr(GetGameTime)+' player='+IntToStr(arg_obj.pl)+' squad='+IntToStr(arg_obj.squad)+' step='+FloatToStr(0.045*arg_obj.cavalrySpeedFraction)+' targetStep=0.045 accelTime=1 movementMode=position chargeCap=1.5 chargeRadius=8.4');"
$initial=ReplaceOnce $initial $initCav ($initCav+$ni+$initCavLog) 'Cavalry initialization'
$initGun="      [*] = ;SetGameObjectMyTrackPointMoveStep(0.006);"
$initGunReplacement=@'
      [*] = ;begin
      [*] = ;   SetGameObjectMyTrackPointMoveStep(0.006);
      [*] = ;   Log('OVDIAG GUN_INIT hnd='+IntToStr(myHnd)+' sid='+basename+' step=0.006');
      [*] = ;end;
'@
$initial=ReplaceOnce $initial $initGun (Lines $initGunReplacement $ni) 'Artillery movement init'

# D) Live cavalry: reasons for failure plus speed evolution, near target,
# pike detection, charge boost, trigger and resulting applied damage.
$tick=ReadMod 'scripts\units\unit.inc\nothing.inc'
$nt=Newline $tick
# Unique anchors in stage-1 injected text.
$movingAnchor='      [*] = ;            if cavMoving then'
$stateLog=@'
      [*] = ;            if (progresstick mod 64=0) then
      [*] = ;            Log('OVDIAG CAV_STATE hnd='+IntToStr(myHnd)+' sid='+TObjProp(pobjprop).sid+' squad='+IntToStr(arg_obj.squad)+' formed='+BoolToStr(cavFormed)+' moving='+BoolToStr(cavMoving)+' baseSpeed='+FloatToStr(arg_obj.cavalrySpeedFraction)+' chargeSpeed='+FloatToStr(arg_obj.cavalryChargeBoost)+' effectiveFactor='+FloatToStr(arg_obj.cavalrySpeedFraction*arg_obj.cavalryChargeBoost)+' step='+FloatToStr(0.045*arg_obj.cavalrySpeedFraction*arg_obj.cavalryChargeBoost)+' x='+FloatToStr(GetGameObjectMyPositionX)+' z='+FloatToStr(GetGameObjectMyPositionZ)+' t='+FloatToStr(gametime));
      [*] = ;            if cavMoving and (not cavFormed) and (progresstick mod 64=0) then
      [*] = ;            Log('OVDIAG CAV_BLOCK reason=NO_FORMATION hnd='+IntToStr(myHnd)+' sid='+TObjProp(pobjprop).sid+' squad='+IntToStr(arg_obj.squad));
'@
if ($LogMode -eq 'Full') {
    # Measure physical displacement frequently enough to see a 1-second ramp.
    # Compact keeps the original 64-tick period to avoid flooding big battles.
    $stateLog = $stateLog.Replace('if (progresstick mod 64=0) then', 'if (progresstick mod 8=0) then')
}
$tick=ReplaceOnce $tick $movingAnchor ((Lines $stateLog $nt)+$nt+$movingAnchor) 'Cavalry periodic motion and formation'

$targetCheck='      [*] = ;                              if cavFront>0.70 then'
$targetLog=@'
      [*] = ;                              if (progresstick mod 64=0) then
      [*] = ;                              Log('OVDIAG CAV_ENEMY hnd='+IntToStr(myHnd)+' target='+TObjProp(cavProp).sid+' distance='+FloatToStr(cavDist)+' direction='+FloatToStr(cavFront)+' formed='+BoolToStr(cavFormed));
'@
$tick=ReplaceOnce $tick $targetCheck ((Lines $targetLog $nt)+$nt+$targetCheck) 'Cavalry target search'
$blockAnchor='      [*] = ;                                 if (not cavIsPike) then'
$pikeLog=@'
      [*] = ;                                 if cavIsPike and (cavDist<4) and (progresstick mod 32=0) then
      [*] = ;                                 Log('OVDIAG CAV_BLOCK reason=PIKES hnd='+IntToStr(myHnd)+' target='+TObjProp(cavProp).sid+' distance='+FloatToStr(cavDist));
      [*] = ;                                 if (not cavIsPike) and (cavDist<4) and (progresstick mod 32=0) then
      [*] = ;                                 Log('OVDIAG CAV_CONTACT hnd='+IntToStr(myHnd)+' target='+TObjProp(cavProp).sid+' formed='+BoolToStr(cavEnemyFormed)+' distance='+FloatToStr(cavDist)+' speed='+FloatToStr(arg_obj.cavalrySpeedFraction)+' cooldownUntil='+FloatToStr(arg_obj.cavalryLastHitTime+5));
'@
$tick=ReplaceOnce $tick $blockAnchor ((Lines $pikeLog $nt)+$nt+$blockAnchor) 'Pike and contact reasons'

$chargeOld='      [*] = ;            if arg_obj.cavalryChargeBoost<cavDesiredBoost then'
$boostPre='      [*] = ;            var cavBoostWas : Float = arg_obj.cavalryChargeBoost;'
$tick=ReplaceOnce $tick $chargeOld ($boostPre+$nt+$chargeOld) 'Speed boost prior'
$boostNew='      [*] = ;            SetGameObjectMyTrackPointMoveStep(0.045*arg_obj.cavalrySpeedFraction*arg_obj.cavalryChargeBoost);'
$boostPost=@'
      [*] = ;            if (cavBoostWas<1.15) and (arg_obj.cavalryChargeBoost>=1.15) then
      [*] = ;            Log('OVDIAG CAV_BOOST_START hnd='+IntToStr(myHnd)+' sid='+TObjProp(pobjprop).sid+' boost='+FloatToStr(arg_obj.cavalryChargeBoost));
      [*] = ;            if (cavBoostWas<1.45) and (arg_obj.cavalryChargeBoost>=1.45) then
      [*] = ;            Log('OVDIAG CAV_BOOST_MAX hnd='+IntToStr(myHnd)+' sid='+TObjProp(pobjprop).sid+' boost='+FloatToStr(arg_obj.cavalryChargeBoost));
      [*] = ;            if (cavBoostWas>1.05) and (arg_obj.cavalryChargeBoost<=1.05) then
      [*] = ;            Log('OVDIAG CAV_BOOST_END hnd='+IntToStr(myHnd)+' sid='+TObjProp(pobjprop).sid);
'@
$tick=ReplaceOnce $tick $boostNew ($boostNew+$nt+(Lines $boostPost $nt)) 'Cavalry boost transitions'
$chargeTrigger="      [*] = ;                                       Log('CavalryCharge HIT damage='+IntToStr(cavHit)+' target='+TObjProp(cavProp).sid+' dist='+FloatToStr(cavDist));"
$chargeExtra="      [*] = ;                                       Log('OVDIAG CAV_HIT_CHECK hnd='+IntToStr(myHnd)+' attacker='+TObjProp(pobjprop).sid+' target='+TObjProp(cavProp).sid+' targetHP='+IntToStr(TObj(cavObj).hp)+' formedTarget='+BoolToStr(cavEnemyFormed)+' facing='+FloatToStr(cavFront)+' boost='+FloatToStr(arg_obj.cavalryChargeBoost)+' raw='+IntToStr(cavHit));"
$tick=ReplaceOnce $tick $chargeTrigger ($chargeExtra+$nt+$chargeTrigger) 'Cavalry impact detail'

# E) Bot artillery production: trace actual orders and ownership-limit refusal.
$ai=ReadMod 'scripts\units\global.inc\progresseconomicai.inc'
$na=Newline $ai
foreach ($sid in @('cannon','howitzer')) {
    $quotaName = if ($sid -eq 'cannon') { 'cannon_count' } else { 'howitzer_count' }
    $anchor = '      [*] = ;      if (curcount<' + $quotaName + ') then'
    $call="      [*] = ;      _ai_TryUnit(plind, cid, gc_ai_unit_$sid, ordercount, False);"
    $old=$anchor+$na+$call
    $log="      [*] = ;         if ordercount>0 then Log('OVDIAG AI_ORDER sid=$sid count='+IntToStr(ordercount)+' player='+IntToStr(plind));"
    $new=$anchor+$na+'      [*] = ;      begin'+$na+$log+$na+$call+$na+'      [*] = ;      end;'
    $ai=ReplaceOnce $ai $old $new "AI $sid order trace"
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
Write-Host 'Only NEW offline test scenario. Watch for OVDIAG CONFIG_SPAWN, CAV_*, HP_APPLY, RELOAD, AI_*, GUN_INIT.'
Write-Host 'WARNING: Full mode logs almost every recorded combat hit. Use test maps with FEW units.'
