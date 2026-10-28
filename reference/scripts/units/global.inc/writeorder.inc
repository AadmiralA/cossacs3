section.begin
   Name = WriteOrder
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if arg_plr.fintlst=nil then
      [*] = ;   _net_ErrorLog('arg_plr.fintlst = nil');
      [*] = ;
      [*] = ;if (arg_plr.fintlst<>nil) and (TIntegerList(arg_plr.fintlst).GetCount=0) then
      [*] = ;   _net_ErrorLog('TIntegerList(arg_plr.fintlst).GetCount = 0');
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin // _net_IsClient ?
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadOrder') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadOrder'')');
      [*] = ;
      [*] = ;   //var handle: Integer = arg_plr.fhandle;
      [*] = ;   //var hnduid: Integer = GetGameObjectUniqueIdByHandle(handle);
      [*] = ;   var ordtyp: Integer = arg_plr.fordtyp;
      [*] = ;   var target: Integer = arg_plr.ftarget;
      [*] = ;   var taruid: Integer = GetGameObjectUniqueIdByHandle(target);
      [*] = ;   var clrord: Boolean = arg_plr.fclrord;
      [*] = ;   var locktrg: Boolean = arg_plr.flocktrg;
      [*] = ;   var intlst: Pointer = arg_plr.fintlst;
      [*] = ;   var number: Integer;
      [*] = ;   if intlst<>nil then
      [*] = ;      number:=TIntegerList(intlst).GetCount
      [*] = ;   else number:=0;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   //RecordCustomWriteInteger(hnduid);
      [*] = ;   RecordCustomWriteInteger(ordtyp);
      [*] = ;   RecordCustomWriteInteger(taruid);
      [*] = ;   RecordCustomWriteBoolean(clrord);
      [*] = ;   RecordCustomWriteBoolean(locktrg);
      [*] = ;   RecordCustomWriteInteger(number);
      [*] = ;
      [*] = ;   if (ordtyp=gc_obj_order_type_patrol) or (ordtyp=gc_obj_order_type_attackpoint) then begin
      [*] = ;      RecordCustomWriteFloat(arg_plr.fposx);
      [*] = ;      RecordCustomWriteFloat(arg_plr.fposz);
      [*] = ;   end;
      [*] = ;
      [*] = ;   var i: Integer;
      [*] = ;   for i:=0 to number-1 do begin //for i:=number-1 downto 0 do begin
      [*] = ;      var handle: Integer = TIntegerList(intlst).Get(i);
      [*] = ;      var hnduid: Integer = GetGameObjectUniqueIdByHandle(handle);
      [*] = ;      RecordCustomWriteInteger(hnduid);
      [*] = ;   end;
      [*] = ;
      [*] = ;   if _net_debug_log_order then
      [*] = ;      _net_Log('writeorder(number='+IntToStr(number)+';ordtyp='+IntToStr(ordtyp)+';taruid='+IntToStr(taruid)+';clrord='+BoolToStr(clrord)+')');
      [*] = ;      //_net_Log('writeorder(hnduid='+IntToStr(hnduid)+';ordtyp='+IntToStr(ordtyp)+';taruid='+IntToStr(taruid)+';clrord='+BoolToStr(clrord)+')');
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

