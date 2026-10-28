section.begin
   Name = WriteConstruct
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if arg_plr.fintlst=nil then
      [*] = ;   _net_ErrorLog('arg_plr.fintlst = nil');
      [*] = ;
      [*] = ;//if (arg_plr.fintlst<>nil) and (TIntegerList(arg_plr.fintlst).GetCount=0) then // it is ok for AI to place building without selected peasants
      [*] = ;//   _net_ErrorLog('TIntegerList(arg_plr.fintlst).GetCount = 0');
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadConstruct') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadConstruct'')');
      [*] = ;
      [*] = ;   var cid: Integer = arg_plr.fcid;
      [*] = ;   var sid: String = arg_plr.fsid;
      [*] = ;   var posx: Float = arg_plr.fposx;
      [*] = ;   var posz: Float = arg_plr.fposz;
      [*] = ;   var clrord: Boolean = arg_plr.fclrord;
      [*] = ;   var list: Pointer = arg_plr.fintlst;
      [*] = ;   var count: Integer;
      [*] = ;   if list<>nil then
      [*] = ;      count:=TIntegerList(list).GetCount
      [*] = ;   else count:=0;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;   RecordCustomWriteInteger(cid);
      [*] = ;   RecordCustomWriteString(sid);
      [*] = ;   RecordCustomWriteFloat(posx);
      [*] = ;   RecordCustomWriteFloat(posz);
      [*] = ;   RecordCustomWriteBoolean(clrord);
      [*] = ;   RecordCustomWriteInteger(count);
      [*] = ;
      [*] = ;   var i: Integer;
      [*] = ;   for i:=0 to count-1 do begin
      [*] = ;      var handle: Integer = TIntegerList(list).Get(i);
      [*] = ;      var hnduid: Integer = GetGameObjectUniqueIdByHandle(handle);
      [*] = ;      RecordCustomWriteInteger(hnduid);
      [*] = ;   end;
      [*] = ;
      [*] = ;   if _net_debug_log_construct then
      [*] = ;      _net_Log('writeconstruct(count='+IntToStr(count)+';cid='+IntToStr(cid)+';sid='+sid+')');
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

