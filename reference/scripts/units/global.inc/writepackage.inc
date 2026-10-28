section.begin
   Name = WritePackage
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var str: String = arg_plr.fbasename;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   if (str<>'') then
      [*] = ;   begin
      [*] = ;      if not RecordCustomBegin('ReadPackage') then
      [*] = ;      _net_ErrorLog('not RecordCustomBegin()');
      [*] = ;
      [*] = ;      RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;      RecordCustomWriteString(str);
      [*] = ;
      [*] = ;      if _net_debug_log_size_writer then _net_Log_SizeWriter;
      [*] = ;
      [*] = ;      RecordCustomEnd;
      [*] = ;
      [*] = ;      if _net_IsServer or _net_IsOffline then
      [*] = ;      _misc_ProcessNetMessage(str);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;//PlayerExecuteStateByHandle(GetPlayerHandleInterfaceIO, 'WritePackage');
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

