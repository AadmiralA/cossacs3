section.begin
   Name = SetTagStates
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;var switchState : Integer = gint_unit_tagstate;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var oldState : Integer = GetGameObjectMyStatesTag;
      [*] = ;var newState : Integer = oldState;
      [*] = ;
      [*] = ;//var iOldAction : Integer = oldState and gc_statetag_action;
      [*] = ;var iOldEssential : Integer = oldState and gc_statetag_essential;
      [*] = ;//var iOldVisual : Integer = oldState and gc_statetag_visual;
      [*] = ;
      [*] = ;var switchEssential : Integer = (switchState and gc_statetag_essential);
      [*] = ;if (switchEssential<>0) then
      [*] = ;newState := (newState and not gc_statetag_essential) or switchEssential;
      [*] = ;
      [*] = ;var switchVisual : Integer = (switchState and gc_statetag_visual);
      [*] = ;if (switchVisual<>0) then
      [*] = ;newState := (newState and not gc_statetag_visual) or switchVisual;
      [*] = ;
      [*] = ;var switchAction : Integer = (switchState and gc_statetag_action);
      [*] = ;if (switchAction<>0) then
      [*] = ;newState := (newState and not gc_statetag_action) or switchAction;
      [*] = ;
      [*] = ;//var iAction : Integer = newState and gc_statetag_action;
      [*] = ;//var iEssential : Integer = newState and gc_statetag_essential;
      [*] = ;//var iVisual : Integer = newState and gc_statetag_visual;
      [*] = ;
      [*] = ;if (not gObjProp[arg_obj.cid][arg_obj.id].bwall) and (switchEssential=gc_statetag_essential_death) then
      [*] = ;begin
      [*] = ;   if (iOldEssential=gc_statetag_essential_birth) then
      [*] = ;   begin
      [*] = ;      switchVisual := gc_statetag_visual_stage_1;
      [*] = ;      newState := (newState and not gc_statetag_visual) or gc_statetag_visual_stage_1;
      [*] = ;      newState := newState or gc_statetag_essential_death;
      [*] = ;      GameObjectMyDelayExecuteState('DeathStage2', gc_building_deathtime_1);
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (switchVisual<>gc_statetag_visual_stage_1) then
      [*] = ;   begin
      [*] = ;      GameObjectMyCancelDelayExecuteState;
      [*] = ;
      [*] = ;      var deathtime : Float = gc_building_deathtime_0;
      [*] = ;      if (_unit_GetUsage(myHnd)=gc_obj_usage_mine) then
      [*] = ;      deathtime := deathtime*2;
      [*] = ;
      [*] = ;      GameObjectMyDelayExecuteState('DeathStage1', deathtime);
      [*] = ;      newState := (newState and not gc_statetag_visual) or gc_statetag_visual_stage_0;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (newState<>oldState) then
      [*] = ;SetGameObjectMyStatesTag(newState);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

