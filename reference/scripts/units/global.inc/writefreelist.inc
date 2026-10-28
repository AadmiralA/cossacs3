section.begin
   Name = WriteFreeList
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadFreeList') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadFreeList'')');
      [*] = ;
      [*] = ;   var count : Integer;
      [*] = ;   var list: Pointer = arg_plr.fintlst;
      [*] = ;   if (list<>nil) then
      [*] = ;   count:=TIntegerList(list).GetCount;
      [*] = ;   
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteInteger(count);
      [*] = ;   var i : Integer;
      [*] = ;   for i:=0 to count-1 do begin
      [*] = ;      var hnd: Integer = TIntegerList(list).Get(i);
      [*] = ;      var uid: Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;      RecordCustomWriteInteger(uid);
      [*] = ;   end;
      [*] = ;
      [*] = ;   //if _net_debug_log_free then
      [*] = ;   //   _net_Log('writefreelist(count='+IntToStr(count));
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

