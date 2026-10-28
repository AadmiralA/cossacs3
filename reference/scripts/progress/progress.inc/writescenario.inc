section.begin
   Name = WriteScenario
   Code : struct.begin
      [*] = ;var ptrigger : Pointer = gpointer_register0;
      [*] = ;if (not _net_IsServer) then
      [*] = ;begin
      [*] = ;   ErrorLog('WriteScenario : not IsServer tries to writescenario; exit;');
      [*] = ;   exit;
      [*] = ;end;
      [*] = ;
      [*] = ;if (ptrigger<>nil) then
      [*] = ;begin
      [*] = ;   if (_net_IsOnline) then
      [*] = ;   begin
      [*] = ;      if not RecordCustomBegin('ReadScenario') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadScenario'')');
      [*] = ;
      [*] = ;      var count : Integer;
      [*] = ;      var j : Integer;
      [*] = ;      for j:=0 to gc_trigger_maxactions-1 do
      [*] = ;      begin
      [*] = ;         if (TScenarioTrigger(ptrigger).actions[j].id<>gc_trigger_action_none) and (TScenarioTrigger(ptrigger).actions[j].bactive) then
      [*] = ;         begin
      [*] = ;            var ptriggeraction : Pointer = TScenarioTrigger(ptrigger).actions[j];
      [*] = ;            case TScenarioTriggerAction(ptriggeraction).id of
      [*] = ;               gc_trigger_action_net_SetTriggerActive, gc_trigger_action_net_SetFlagState, gc_trigger_action_net_SetCounterValue, gc_trigger_action_net_ExecuteTriggerActions : begin
      [*] = ;                  if (TScenarioTrigger(ptrigger).actions[j].player<>GetPlayerIndexInterfaceIO) then
      [*] = ;                  count := count+1;
      [*] = ;               end;
      [*] = ;               gc_trigger_action_net_TeamSetTriggerActive, gc_trigger_action_net_TeamSetFlagState, gc_trigger_action_net_TeamSetCounterValue, gc_trigger_action_net_TeamExecuteTriggerActions : begin
      [*] = ;                  count := count+1;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      // write data to package
      [*] = ;      RecordCustomWriteByte(count);
      [*] = ;
      [*] = ;      for j:=0 to gc_trigger_maxactions-1 do
      [*] = ;      begin
      [*] = ;         if (TScenarioTrigger(ptrigger).actions[j].id<>gc_trigger_action_none) and (TScenarioTrigger(ptrigger).actions[j].bactive) then
      [*] = ;         begin
      [*] = ;            var ptriggeraction : Pointer = TScenarioTrigger(ptrigger).actions[j];
      [*] = ;            case TScenarioTriggerAction(ptriggeraction).id of
      [*] = ;               gc_trigger_action_net_SetTriggerActive, gc_trigger_action_net_TeamSetTriggerActive : begin
      [*] = ;                  if (TScenarioTriggerAction(ptriggeraction).id=gc_trigger_action_net_TeamSetTriggerActive) or (TScenarioTrigger(ptrigger).actions[j].player<>GetPlayerIndexInterfaceIO) then
      [*] = ;                  begin
      [*] = ;                     var player : Integer = TScenarioTriggerAction(ptriggeraction).player;
      [*] = ;                     var triggerid : Integer = TScenarioTriggerAction(ptriggeraction).triggerid;
      [*] = ;                     var bstate : Boolean = TScenarioTriggerAction(ptriggeraction).bstate;
      [*] = ;                     RecordCustomWriteByte(TScenarioTriggerAction(ptriggeraction).id);
      [*] = ;                     RecordCustomWriteByte(player);
      [*] = ;                     RecordCustomWriteInteger(triggerid);
      [*] = ;                     RecordCustomWriteBoolean(bstate);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_trigger_action_net_SetFlagState, gc_trigger_action_net_TeamSetFlagState : begin
      [*] = ;                  if (TScenarioTriggerAction(ptriggeraction).id=gc_trigger_action_net_TeamSetFlagState) or (TScenarioTrigger(ptrigger).actions[j].player<>GetPlayerIndexInterfaceIO) then
      [*] = ;                  begin
      [*] = ;                     var player : Integer = TScenarioTriggerAction(ptriggeraction).player;
      [*] = ;                     var flagid : Integer = TScenarioTriggerAction(ptriggeraction).flagid;
      [*] = ;                     var bstate : Boolean = TScenarioTriggerAction(ptriggeraction).bstate;
      [*] = ;                     RecordCustomWriteByte(TScenarioTriggerAction(ptriggeraction).id);
      [*] = ;                     RecordCustomWriteByte(player);
      [*] = ;                     RecordCustomWriteInteger(flagid);
      [*] = ;                     RecordCustomWriteBoolean(bstate);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_trigger_action_net_SetCounterValue, gc_trigger_action_net_TeamSetCounterValue : begin
      [*] = ;                  if (TScenarioTriggerAction(ptriggeraction).id=gc_trigger_action_net_TeamSetCounterValue) or (TScenarioTrigger(ptrigger).actions[j].player<>GetPlayerIndexInterfaceIO) then
      [*] = ;                  begin
      [*] = ;                     var player : Integer = TScenarioTriggerAction(ptriggeraction).player;
      [*] = ;                     var counterid : Integer = TScenarioTriggerAction(ptriggeraction).counterid;
      [*] = ;                     var amount : Integer;
      [*] = ;                     if (TScenarioTriggerAction(ptriggeraction).busecounterasamount) then
      [*] = ;                     amount := gScenario.counters[TScenarioTriggerAction(ptriggeraction).usecounterid].value
      [*] = ;                     else
      [*] = ;                     amount := TScenarioTriggerAction(ptriggeraction).amount;
      [*] = ;                     RecordCustomWriteByte(TScenarioTriggerAction(ptriggeraction).id);
      [*] = ;                     RecordCustomWriteByte(player);
      [*] = ;                     RecordCustomWriteInteger(counterid);
      [*] = ;                     RecordCustomWriteInteger(amount);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_trigger_action_net_ExecuteTriggerActions, gc_trigger_action_net_TeamExecuteTriggerActions : begin
      [*] = ;                  if (TScenarioTriggerAction(ptriggeraction).id=gc_trigger_action_net_TeamExecuteTriggerActions) or (TScenarioTrigger(ptrigger).actions[j].player<>GetPlayerIndexInterfaceIO) then
      [*] = ;                  begin
      [*] = ;                     var player : Integer = TScenarioTriggerAction(ptriggeraction).player;
      [*] = ;                     var triggerid : Integer = TScenarioTriggerAction(ptriggeraction).triggerid;
      [*] = ;                     RecordCustomWriteByte(TScenarioTriggerAction(ptriggeraction).id);
      [*] = ;                     RecordCustomWriteByte(player);
      [*] = ;                     RecordCustomWriteInteger(triggerid);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if _net_debug_log_size_writer then
      [*] = ;      _net_Log_SizeWriter;
      [*] = ;
      [*] = ;      // finish package
      [*] = ;      RecordCustomEnd;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   ErrorLog('WriteScenario : not IsOnline');
      [*] = ;end
      [*] = ;else
      [*] = ;ErrorLog('WriteScenario : ptrigger=nil');
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

