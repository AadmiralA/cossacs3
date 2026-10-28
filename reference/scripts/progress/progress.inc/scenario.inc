section.begin
   Name = Scenario
   Code : struct.begin
      [*] = ;_misc_ProfilerBegin('progress:Scenario');
      [*] = ;
      [*] = ;var i, j : Integer;
      [*] = ;for i:=0 to gc_scenario_maxtimers-1 do
      [*] = ;begin
      [*] = ;   if (gScenario.timers[i].bstarted) and (not gScenario.timers[i].bfinished) then
      [*] = ;   begin
      [*] = ;      var delta : Float = (GetGameTime-gScenario.timers[i].starttime)*1000;
      [*] = ;      if (delta>=gScenario.timers[i].time) then
      [*] = ;      gScenario.timers[i].bfinished := True;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;var bUpdateAIRegionsActiveState : Boolean;
      [*] = ;for i:=0 to gc_scenario_maxtriggers-1 do
      [*] = ;begin
      [*] = ;   if (gScenario.triggers[i].bactive) then
      [*] = ;   begin
      [*] = ;      var ptrigger : Pointer = gScenario.triggers[i];
      [*] = ;      if (TScenarioTrigger(ptrigger).activetime<=0) then
      [*] = ;      TScenarioTrigger(ptrigger).activetime := GetGameTime;
      [*] = ;
      [*] = ;      var bConditionExists : Boolean;
      [*] = ;      var j : Integer;
      [*] = ;      for j:=0 to gc_trigger_maxconditions-1 do
      [*] = ;      begin
      [*] = ;         if (TScenarioTrigger(ptrigger).conditions[j].id<>gc_trigger_condition_none) and (TScenarioTrigger(ptrigger).conditions[j].id<>gc_trigger_condition_undefined) and (TScenarioTrigger(ptrigger).conditions[j].bactive) then
      [*] = ;         begin
      [*] = ;            bConditionExists := True;
      [*] = ;            if (gMap.bbattle) then
      [*] = ;            begin
      [*] = ;               case TScenarioTrigger(ptrigger).conditions[j].id of
      [*] = ;                  gc_trigger_condition_net_IsServer : begin
      [*] = ;                     if (not _net_IsServer) then
      [*] = ;                     begin
      [*] = ;                        bConditionExists := False;
      [*] = ;                        break;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;                  gc_trigger_condition_net_IsClient : begin
      [*] = ;                     if (not _net_IsClient) then
      [*] = ;                     begin
      [*] = ;                        bConditionExists := False;
      [*] = ;                        break;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;                  gc_trigger_condition_net_IsOnline : begin
      [*] = ;                     if (not _net_IsOnline) then
      [*] = ;                     begin
      [*] = ;                        bConditionExists := False;
      [*] = ;                        break;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;                  gc_trigger_condition_net_IsOffline : begin
      [*] = ;                     if (not _net_IsOffline) then
      [*] = ;                     begin
      [*] = ;                        bConditionExists := False;
      [*] = ;                        break;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            break;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if (bConditionExists) and (_scenario_CheckTriggerConditions(ptrigger)) then
      [*] = ;      begin
      [*] = ;         gScenario.triggers[i].bactive := False;
      [*] = ;         gScenario.triggers[i].activetime := 0;
      [*] = ;
      [*] = ;         // Process all Trigger Actions
      [*] = ;         _scenario_EvaluateTriggerActions(ptrigger);
      [*] = ;
      [*] = ;         // UPDATE UI IF OPEN
      [*] = ;         var elmParent : Integer = GetGUIElementIndexByNameParent('ScenarioEditor', _gui_GetTop);
      [*] = ;         if (elmParent<>0) and (GetGUIElementVisible(elmParent)) and (gScenario.ui.helpertype=gc_scenario_ui_helpertype_none) then
      [*] = ;         GUIExecuteState('ShowScenarioEditor');
      [*] = ;
      [*] = ;         bUpdateAIRegionsActiveState := True;
      [*] = ;
      [*] = ;         if (_net_IsOffline) and (GetProgressingPauseMode) then // if query show requested
      [*] = ;         break;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;if (bUpdateAIRegionsActiveState) then
      [*] = ;_scenario_UpdateAIRegionsActiveStateToTriggers(gScenario);
      [*] = ;
      [*] = ;_misc_ProfilerEnd('progress:Scenario');
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

