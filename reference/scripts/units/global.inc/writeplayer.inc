section.begin
   Name = WritePlayer
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadPlayer') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadPlayer'')');
      [*] = ;
      [*] = ;   var hnd: Integer = arg_plr.fhandle;
      [*] = ;   var cap: Boolean = arg_plr.fcapture;
      [*] = ;   var uid: Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteInteger(uid);
      [*] = ;   RecordCustomWriteBoolean(cap);
      [*] = ;
      [*] = ;   if _net_debug_log_player then
      [*] = ;      _net_Log('writeplayer(uid='+IntToStr(uid)+';cap='+BoolToStr(cap)+')');
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