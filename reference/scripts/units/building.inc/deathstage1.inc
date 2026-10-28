section.begin
   Name = DeathStage1
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;_unit_SetTagStates(myHnd, gc_statetag_essential_death or gc_statetag_visual_stage_1);
      [*] = ;
      [*] = ;var deathtime : Float = gc_building_deathtime_1;
      [*] = ;if (_unit_GetUsage(myHnd)=gc_obj_usage_mine) then
      [*] = ;deathtime := deathtime*2;
      [*] = ;
      [*] = ;GameObjectMyDelayExecuteState('DeathStage2', deathtime);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

