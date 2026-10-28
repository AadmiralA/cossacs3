section.begin
   Name = WriteFree
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadFree') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadFree'')');
      [*] = ;
      [*] = ;   var hnd: Integer = arg_plr.fhandle;
      [*] = ;   var uid: Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteInteger(uid);
      [*] = ;   RecordCustomWriteInteger(GetPlayerGameObjectsCountByHandle(GetPlayerCurrentPlHandle){-1});
      [*] = ;
      [*] = ;   if _net_debug_log_free then
      [*] = ;      _net_Log('writefree(uid='+IntToStr(uid)+';num='+IntToStr(GetPlayerGameObjectsCountByHandle(GetPlayerCurrentPlHandle))+')');
      [*] = ;
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;      _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end