section.begin
   Name = WriteLeave
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadLeave') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadLeave'')');
      [*] = ;
      [*] = ;   var dead : Boolean = arg_plr.fdead;
      [*] = ;   var goHnd : Integer = arg_plr.fHandle;
      [*] = ;   var uid : Integer = GetGameObjectUniqueIdByHandle(goHnd);
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteBoolean(dead);
      [*] = ;   RecordCustomWriteInteger(uid);
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
