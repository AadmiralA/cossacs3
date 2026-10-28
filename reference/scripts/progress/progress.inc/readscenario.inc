section.begin
   Name = ReadScenario
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;if (_net_IsServer) then
      [*] = ;begin
      [*] = ;   ErrorLog('ReadScenario : IsServer tries to readscenario; exit;');
      [*] = ;   exit;
      [*] = ;end;
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var count : Byte = RecordCustomReadByte;
      [*] = ;
      [*] = ;var i, j : Integer;
      [*] = ;for i:=0 to count-1 do
      [*] = ;begin
      [*] = ;   var myplind : Integer = GetPlayerIndexByHandle(GetPlayerHandleInterfaceIO);
      [*] = ;   var id : Integer = RecordCustomReadByte;
      [*] = ;   case id of
      [*] = ;      gc_trigger_action_net_SetTriggerActive, gc_trigger_action_net_TeamSetTriggerActive : begin
      [*] = ;         var player : Byte = RecordCustomReadByte;
      [*] = ;         var triggerid : Integer = RecordCustomReadInteger;
      [*] = ;         var bstate : Boolean = RecordCustomReadBoolean;
      [*] = ;         if (id=gc_trigger_action_net_TeamSetTriggerActive) then
      [*] = ;         begin
      [*] = ;            if (gMap.players[player].team=gMap.players[myplind].team) then
      [*] = ;            gScenario.triggers[triggerid].bactive := bstate;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (player=myplind) then
      [*] = ;         gScenario.triggers[triggerid].bactive := bstate;
      [*] = ;      end;
      [*] = ;      gc_trigger_action_net_SetFlagState, gc_trigger_action_net_TeamSetFlagState : begin
      [*] = ;         var player : Byte = RecordCustomReadByte;
      [*] = ;         var flagid : Integer = RecordCustomReadInteger;
      [*] = ;         var bstate : Boolean = RecordCustomReadBoolean;
      [*] = ;         if (id=gc_trigger_action_net_TeamSetFlagState) then
      [*] = ;         begin
      [*] = ;            if (gMap.players[player].team=gMap.players[myplind].team) then
      [*] = ;            gScenario.flags[flagid].bactive := bstate;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (player=myplind) then
      [*] = ;         gScenario.flags[flagid].bactive := bstate;
      [*] = ;      end;
      [*] = ;      gc_trigger_action_net_SetCounterValue, gc_trigger_action_net_TeamSetCounterValue : begin
      [*] = ;         var player : Byte = RecordCustomReadByte;
      [*] = ;         var counterid : Integer = RecordCustomReadInteger;
      [*] = ;         var amount : Integer = RecordCustomReadInteger;
      [*] = ;         if (id=gc_trigger_action_net_TeamSetCounterValue) then
      [*] = ;         begin
      [*] = ;            if (gMap.players[player].team=gMap.players[myplind].team) then
      [*] = ;            gScenario.counters[counterid].value := amount;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (player=myplind) then
      [*] = ;         gScenario.counters[counterid].value := amount;
      [*] = ;      end;
      [*] = ;      gc_trigger_action_net_ExecuteTriggerActions, gc_trigger_action_net_TeamExecuteTriggerActions : begin
      [*] = ;         var player : Byte = RecordCustomReadByte;
      [*] = ;         var triggerid : Integer = RecordCustomReadInteger;
      [*] = ;         if (id=gc_trigger_action_net_TeamExecuteTriggerActions) then
      [*] = ;         begin
      [*] = ;            if (gMap.players[player].team=gMap.players[myplind].team) then
      [*] = ;            begin
      [*] = ;               var ptrigger : Pointer = gScenario.triggers[triggerid];
      [*] = ;               _scenario_EvaluateTriggerActions(ptrigger);
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (player=myplind) then
      [*] = ;         begin
      [*] = ;            var ptrigger : Pointer = gScenario.triggers[triggerid];
      [*] = ;            _scenario_EvaluateTriggerActions(ptrigger);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

