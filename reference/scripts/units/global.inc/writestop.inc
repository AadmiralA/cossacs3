section.begin
   Name = WriteStop
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if arg_plr.fintlst=nil then
      [*] = ;   _net_ErrorLog('arg_plr.fintlst = nil');
      [*] = ;
      [*] = ;//if(arg_plr.fintlst<>nil) and (TIntegerList(arg_plr.fintlst).GetCount=0) then // can be 0 if ship selected
      [*] = ;//   _net_ErrorLog('TIntegerList(arg_plr.fintlst).GetCount = 0');
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadStop') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadStop'')');
      [*] = ;
      [*] = ;   var list: Pointer = arg_plr.fintlst;
      [*] = ;   var count: Integer;
      [*] = ;   if list<>nil then
      [*] = ;      count:=TIntegerList(list).GetCount;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteInteger(count);
      [*] = ;
      [*] = ;   var i: Integer;
      [*] = ;   for i:=0 to count-1 do begin
      [*] = ;      var hnd: Integer = TIntegerList(list).Get(i);
      [*] = ;      var uid: Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;      RecordCustomWriteInteger(uid);
      [*] = ;   end;
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

