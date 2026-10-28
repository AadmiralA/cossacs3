section.begin
   Name = WriteApply
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadApply') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadApply'')');
      [*] = ;
      [*] = ;   var plr: Integer = arg_plr.fplayer;
      [*] = ;   var hnd: Integer = arg_plr.fhandle;
      [*] = ;   var plind: Integer = GetPlayerIndexByHandle(plr);
      [*] = ;   var uid: Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;   var cid: Integer = arg_plr.fcid;
      [*] = ;   var ind: Integer = arg_plr.find;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteInteger(plind);
      [*] = ;   RecordCustomWriteInteger(uid);
      [*] = ;   RecordCustomWriteInteger(cid);
      [*] = ;   RecordCustomWriteInteger(ind);
      [*] = ;
      [*] = ;   if _net_debug_log_apply then
      [*] = ;      _net_Log('writeapply(plind='+IntToStr(plind)+';uid='+IntToStr(uid)+';cid='+IntToStr(cid)+';ind='+IntToStr(ind)+')');
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