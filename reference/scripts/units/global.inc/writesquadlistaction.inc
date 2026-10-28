section.begin
   Name = WriteSquadListAction
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;   var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;   var sqlist : Pointer = arg_plr.fpointer0;
      [*] = ;   var action : Integer = arg_plr.fordtyp;
      [*] = ;   var bstate : Boolean = arg_plr.fbstate;
      [*] = ;   var form   : Integer = arg_plr.fform;
      [*] = ;   var list   : Pointer = arg_plr.fintlst;
      [*] = ;
      [*] = ;   var sqcount, gocount : Integer;
      [*] = ;   if (sqlist<>nil) then sqcount := TPtrList(sqlist).GetCount;
      [*] = ;   if (sqcount>0) then
      [*] = ;   begin
      [*] = ;      if (list<>nil) then gocount := TIntegerList(list).GetCount;
      [*] = ;      //Log('WriteSquadListAction : action='+IntToStr(action)+' sqcount='+IntToStr(sqcount)+' gocount='+IntToStr(gocount));
      [*] = ;
      [*] = ;      // create package
      [*] = ;      if not RecordCustomBegin('ReadSquadListAction') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadSquadListAction'')');
      [*] = ;
      [*] = ;      // write data to package
      [*] = ;      RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;      RecordCustomWriteInteger(action);
      [*] = ;      RecordCustomWriteBoolean(bstate);
      [*] = ;      RecordCustomWriteInteger(form);
      [*] = ;      RecordCustomWriteInteger(sqcount);
      [*] = ;      var i: Integer;
      [*] = ;      for i:=0 to sqcount-1 do begin
      [*] = ;         var psquad : Pointer = TPtrList(sqlist).GetPointer(i);
      [*] = ;         if (psquad<>nil) then
      [*] = ;         begin
      [*] = ;            var squid : Integer = TSquad(psquad).fUID;
      [*] = ;            RecordCustomWriteInteger(squid);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         ErrorLog('WriteSquadListAction : unknown psquad');
      [*] = ;      end;
      [*] = ;
      [*] = ;      RecordCustomWriteInteger(gocount);
      [*] = ;      for i:=0 to gocount-1 do begin
      [*] = ;         var hnd: Integer = TIntegerList(list).Get(i);
      [*] = ;         var uid: Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;         RecordCustomWriteInteger(uid);
      [*] = ;      end;
      [*] = ;      // logs
      [*] = ;
      [*] = ;      // finish package
      [*] = ;      RecordCustomEnd;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

