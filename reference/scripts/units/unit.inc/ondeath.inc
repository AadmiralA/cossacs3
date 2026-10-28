section.begin
   Name = OnDeath
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;if (arg_obj.sndwalkproc) then
      [*] = ;begin
      [*] = ;   case arg_obj.sndwalktype of
      [*] = ;      1 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkinfcloth, gSoundManager.counter.walkdstinfcloth);
      [*] = ;      2 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkinfmetal, gSoundManager.counter.walkdstinfmetal);
      [*] = ;      3 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkartwheels, gSoundManager.counter.walkdstartwheels);
      [*] = ;      4 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkartmortar, gSoundManager.counter.walkdstartmortar);
      [*] = ;      5 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkcavfast, gSoundManager.counter.walkdstcavfast);
      [*] = ;      6 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkcavslow, gSoundManager.counter.walkdstcavslow);
      [*] = ;      7 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkinfcloth, gSoundManager.counter.walkdstinfcloth);
      [*] = ;      8 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkpeastone, gSoundManager.counter.walkdstpeastone);
      [*] = ;      9 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkpea, gSoundManager.counter.walkdstpea);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (arg_obj.scenariogroupid>-1) then
      [*] = ;begin
      [*] = ;   gScenario.groups[arg_obj.scenariogroupid].unitslist.Remove(myHnd);
      [*] = ;   if (gScenario.ui.bshowui) then
      [*] = ;   gScenarioTmp.groups[arg_obj.scenariogroupid].unitslist.Remove(myHnd);
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
      [*] = ;
      [*] = ;_unit_DestroyObj(myHnd);
   struct.end
section.end

