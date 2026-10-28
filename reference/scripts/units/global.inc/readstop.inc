section.begin
   Name = ReadStop
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var count: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;var i: Integer;
      [*] = ;for i:=0 to count-1 do begin
      [*] = ;   var uid: Integer = RecordCustomReadInteger;
      [*] = ;   var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;   if hnd<>0 then
      [*] = ;   begin
      [*] = ;      if (not _unit_IsBuilding(hnd)) or (_unit_GetUsage(hnd)=gc_obj_usage_tower) then
      [*] = ;      _unit_Stop(hnd);
      [*] = ;   end
      [*] = ;   else _net_ErrorLog('ReadStop invalid uid('+IntToStr(uid)+')');
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

