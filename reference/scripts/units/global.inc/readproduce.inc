section.begin
   Name = ReadProduce
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var proid: Integer = RecordCustomReadInteger;
      [*] = ;var prcid: Integer = RecordCustomReadInteger;
      [*] = ;var amoun: Integer = RecordCustomReadInteger;
      [*] = ;var state: Boolean = RecordCustomReadBoolean;
      [*] = ;var count: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;_net_intlist.Clear();
      [*] = ;
      [*] = ;var i: Integer;
      [*] = ;for i:=0 to count-1 do begin
      [*] = ;   var hnduid: Integer = RecordCustomReadInteger;
      [*] = ;   var handle: Integer = GetGameObjectHandleByUniqueId(hnduid);
      [*] = ;   if handle<>0 then
      [*] = ;      _net_intlist.Add(handle)
      [*] = ;   else _net_ErrorLog('proid('+IntToStr(proid)+') invalid uid('+IntToStr(hnduid)+')');
      [*] = ;end;
      [*] = ;
      [*] = ;_unit_ProduceUnit(GetPlayerCurrentPlHandle, _net_intlist, prcid, proid, amoun, state, _net_IsServer, _net_IsServer);
      [*] = ;//if not _unit_ProduceUnit(GetPlayerCurrentPlHandle, _net_intlist, prcid, proid, amoun, state, false) then // this check is uninformative
      [*] = ;//   _net_ErrorLog('not _unit_ProduceUnit()');
      [*] = ;
      [*] = ;if _net_debug_log_produce then
      [*] = ;   _net_Log('readproduce(count='+IntToStr(count)+';proid='+IntToStr(proid)+';amoun='+IntToStr(amoun)+';state='+BoolToStr(state)+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

