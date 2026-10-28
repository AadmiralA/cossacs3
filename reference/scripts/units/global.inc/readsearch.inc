section.begin
   Name = ReadSearch
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
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
      [*] = ;   else _net_ErrorLog('readsearch invalid uid('+IntToStr(hnduid)+')');
      [*] = ;end;
      [*] = ;
      [*] = ;if not _unit_ListSetSearchEnemy(GetPlayerCurrentPlHandle, _net_intlist, state, false) then
      [*] = ;   _net_ErrorLog('not _unit_ListSetSearchEnemy()');
      [*] = ;   
      [*] = ;if _net_debug_log_search then
      [*] = ;   _net_Log('readsearch(count='+IntToStr(count)+';state='+BoolToStr(state)+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end