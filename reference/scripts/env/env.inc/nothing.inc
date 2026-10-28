section.begin
   Name = Nothing
   Code : struct.begin
      [*] = ;args arg_res : TRes;
      [*] = ;
      [*] = ;if _net_IsClient or _net_IsReplay then
      [*] = ;begin
      [*] = ;   var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;   if (_misc_GetBaseObjID(myHnd)=gc_baseid_res) then
      [*] = ;   begin
      [*] = ;      case arg_res.itype of
      [*] = ;         gc_resource_type_gold, gc_resource_type_iron, gc_resource_type_coal : begin
      [*] = ;            if (GetProjectModeInt=gc_project_mode_game) then // update huds visibility
      [*] = ;            gGuiUpdateHighlights.Add(GetGameObjectMyHandle);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;   if (_misc_GetBaseObjID(myHnd)=gc_baseid_res) then
      [*] = ;   begin
      [*] = ;      case arg_res.itype of
      [*] = ;         gc_resource_type_food : begin
      [*] = ;            var myState : Integer = GetGameObjectMyStatesTag;
      [*] = ;            var myEssential : Integer = myState and gc_statetag_essential;
      [*] = ;            
      [*] = ;            if (myEssential=gc_statetag_essential_death) then
      [*] = ;            begin
      [*] = ;               var growtime : Float = GetGameTime-arg_res.timetransormlast;
      [*] = ;               const cFieldRestTime = 21.875; //need balancing
      [*] = ;               if (growtime>cFieldRestTime) then
      [*] = ;               _unit_SetTagStates(myHnd, gc_statetag_essential_birth or gc_statetag_visual_stage_0)
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               if (not arg_res.brised) then
      [*] = ;               begin
      [*] = ;                  var growtime : Float = GetGameTime-arg_res.timetransormlast;
      [*] = ;                  const cFieldGrowTime = 4*21.875;
      [*] = ;                  if (growtime>cFieldGrowTime) then
      [*] = ;                  _unit_SetTagStates(myHnd, gc_statetag_essential_none or gc_statetag_visual_stage_0)
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     var progress : Float = (growtime/cFieldGrowTime);
      [*] = ;                     var stage : Integer = floor(progress*4);
      [*] = ;                     var visualtag : Integer = gc_statetag_visual_none;
      [*] = ;                     case stage of
      [*] = ;                        0 : visualtag := gc_statetag_visual_stage_0;
      [*] = ;                        1 : visualtag := gc_statetag_visual_stage_1;
      [*] = ;                        2 : visualtag := gc_statetag_visual_stage_2;
      [*] = ;                        3 : visualtag := gc_statetag_visual_stage_3;
      [*] = ;                     end;
      [*] = ;                     if ((_unit_GetTagStateByType(myHnd, gc_statetag_visual) and visualtag)=0) then
      [*] = ;                     _unit_SetTagStates(myHnd, gc_statetag_essential_birth or visualtag)
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               //if (arg_res.brised)  then
      [*] = ;               begin
      [*] = ;                  var visualtag : Integer = gc_statetag_visual_none;
      [*] = ;                  if (arg_res.hp<0) then
      [*] = ;                  begin
      [*] = ;                     if (arg_res.worktransorm=0) then
      [*] = ;                     _unit_SetTagStates(myHnd, gc_statetag_essential_death or gc_statetag_visual_stage_1)
      [*] = ;                     else
      [*] = ;                     _unit_SetTagStates(myHnd, gc_statetag_essential_death or gc_statetag_visual_stage_0)
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     if (arg_res.hp<5000) then visualtag := gc_statetag_visual_stage_2
      [*] = ;                     else
      [*] = ;                     if (arg_res.hp<13000) then visualtag := gc_statetag_visual_stage_1
      [*] = ;                     else
      [*] = ;                     visualtag := gc_statetag_visual_stage_0;
      [*] = ;                     
      [*] = ;                     if (arg_res.hp<gc_FieldMaxHP) then
      [*] = ;                     begin
      [*] = ;                        var growtime : Float = GetGameTime-arg_res.timetransormlast;
      [*] = ;                        const cFieldRestartTime = 31.25;
      [*] = ;                        if (growtime>cFieldRestartTime) and (visualtag=gc_statetag_visual_stage_0) {and (arg_res.worktransorm=1)} then
      [*] = ;                        begin
      [*] = ;                           arg_res.hp := Min(arg_res.hp+Floor(gc_FieldMaxHP*random*0.1), gc_FieldMaxHP);
      [*] = ;                           arg_res.timetransormlast := GetGameTime;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                     if ((_unit_GetTagStateByType(myHnd, gc_statetag_visual) and visualtag)=0) then
      [*] = ;                     _unit_SetTagStates(myHnd, gc_statetag_essential_none or visualtag);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         gc_resource_type_gold, gc_resource_type_iron, gc_resource_type_coal : begin
      [*] = ;            if (GetProjectModeInt=gc_project_mode_game) then // update huds visibility
      [*] = ;            gGuiUpdateHighlights.Add(GetGameObjectMyHandle);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
   struct.end
section.end

