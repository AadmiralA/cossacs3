section.begin
   Name = WriteSyncUnitsParams
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadSyncUnitsParams') then
      [*] = ;   _net_ErrorLog('RecordCustomBegin(''ReadSyncUnitsParams'')');
      [*] = ;   // write data to package
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
      [*] = ;      var uid: Integer = TIntegerList(list).Get(i);
      [*] = ;      var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;      var pobj: Pointer = _unit_GetTObj(hnd);
      [*] = ;      RecordCustomWriteInteger(uid);
      [*] = ;      RecordCustomWriteBoolean(pobj<>nil);
      [*] = ;      if (pobj<>nil) then
      [*] = ;      begin
      [*] = ;         var hp : Integer = TObj(pobj).hp;
      [*] = ;         RecordCustomWriteInteger(hp);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         // invalid object
      [*] = ;         ErrorLog('WriteSyncUnitsParams : invalid object with pobj=nil');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

