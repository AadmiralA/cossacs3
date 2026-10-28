section.begin
   Name = ReadLeave
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var dead : Boolean = RecordCustomReadBoolean;
      [*] = ;var uid : Integer = RecordCustomReadInteger;
      [*] = ;var goHnd : Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;
      [*] = ;if goHnd <> 0 then
      [*] = ;begin
      [*] = ;   _unit_GoOutside(goHnd, dead, false);
      [*] = ;
      [*] = ;   if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;end
      [*] = ;else
      [*] = ;_net_ErrorLog('ReadLeaveFull invalid uid('+IntToStr(uid)+')');
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

