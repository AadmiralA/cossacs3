section.begin
   Name = WriteRally
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadRally') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadRally'')');
      [*] = ;
      [*] = ;   var hnd : Integer = arg_plr.fhandle;
      [*] = ;   var posx: Float   = arg_plr.fposx;
      [*] = ;   var posz: Float   = arg_plr.fposz;
      [*] = ;   var rally : Boolean = arg_plr.fbstate;
      [*] = ;   var uid : Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteInteger(uid);
      [*] = ;   RecordCustomWriteBoolean(rally);
      [*] = ;   RecordCustomWriteFloat(posx);
      [*] = ;   RecordCustomWriteFloat(posz);
      [*] = ;
      [*] = ;   if _net_debug_log_rally then
      [*] = ;      _net_Log('writerally(uid='+IntToStr(uid)+';coords=['+FloatToStr(posx)+';'+FloatToStr(posz)+'])');
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

