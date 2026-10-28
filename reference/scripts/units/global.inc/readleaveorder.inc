section.begin
   Name = ReadLeaveOrder
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var dead: Boolean = RecordCustomReadBoolean;
      [*] = ;var count: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;_net_intlist.Clear();
      [*] = ;
      [*] = ;var i: Integer;
      [*] = ;for i:=0 to count-1 do begin
      [*] = ;   var uid: Integer = RecordCustomReadInteger;
      [*] = ;   var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;   if hnd<>0 then
      [*] = ;      _net_intlist.Add(hnd)
      [*] = ;   else _net_ErrorLog('ReadLeave invalid uid('+IntToStr(uid)+')');
      [*] = ;end;
      [*] = ;
      [*] = ;//_unit_DoUnitsGoOutside(_net_intlist, dead, _net_IsServer);
      [*] = ;_unit_DoUnitsGoOutside(_net_intlist, dead, false);
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

