section.begin
   Name = WriteSearch
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if arg_plr.fintlst=nil then
      [*] = ;   _net_ErrorLog('arg_plr.fintlst = nil');
      [*] = ;
      [*] = ;if (arg_plr.fintlst<>nil) and (TIntegerList(arg_plr.fintlst).GetCount=0) then
      [*] = ;   _net_ErrorLog('TIntegerList(arg_plr.fintlst).GetCount = 0');
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadSearch') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadSearch'')');
      [*] = ;
      [*] = ;   var state: Boolean = arg_plr.fbstate;
      [*] = ;   var ilist: Pointer = arg_plr.fintlst;
      [*] = ;   var count: Integer;
      [*] = ;   if ilist<>nil then
      [*] = ;      count:=TIntegerList(ilist).GetCount
      [*] = ;   else count:=0;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteBoolean(state);
      [*] = ;   RecordCustomWriteInteger(count);
      [*] = ;
      [*] = ;   var i: Integer;
      [*] = ;   for i:=0 to count-1 do begin
      [*] = ;      var handle: Integer = TIntegerList(ilist).Get(i);
      [*] = ;      var hnduid: Integer = GetGameObjectUniqueIdByHandle(handle);
      [*] = ;      RecordCustomWriteInteger(hnduid);
      [*] = ;   end;
      [*] = ;
      [*] = ;   if _net_debug_log_search then
      [*] = ;      _net_Log('writesearch(count='+IntToStr(count)+';state='+BoolToStr(state)+')');
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