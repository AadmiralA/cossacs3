section.begin
   Name = WriteProjFree
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadProjFree') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadProjFree'')');
      [*] = ;
      [*] = ;   var hnd: Integer = arg_plr.fhandle;
      [*] = ;   var uid: Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteInteger(uid);
      [*] = ;   RecordCustomWriteInteger(GetPlayerGameObjectsCountByHandle(GetPlayerCurrentPlHandle){-1});
      [*] = ;
      [*] = ;   if _net_debug_log_projfree then
      [*] = ;      _net_Log('writeprojfree(uid='+IntToStr(uid)+';num='+IntToStr(GetPlayerGameObjectsCountByHandle(GetPlayerCurrentPlHandle))+')');
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

