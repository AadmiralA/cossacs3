section.begin
   Name = ReadMove
   Code : struct.begin
      [*] = ;// server test
      [*] = ;
      [*] = ;if (not _net_IsServer) and (not _net_IsReplay) then
      [*] = ;   _net_ErrorLog('_net_IsServer=false and _net_IsReplay=false');
      [*] = ;
      [*] = ;// read order data from package
      [*] = ;
      [*] = ;var dirx: Float = RecordCustomReadFloat;
      [*] = ;var dirz: Float = RecordCustomReadFloat;
      [*] = ;var addo: Boolean = RecordCustomReadBoolean;
      [*] = ;var dofirst: Boolean = RecordCustomReadBoolean;
      [*] = ;var mode: Integer = RecordCustomReadInteger;
      [*] = ;var count: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;// read uids and coords from package
      [*] = ;
      [*] = ;var i: Integer;
      [*] = ;for i:=0 to count-1 do begin
      [*] = ;   var uid: Integer = RecordCustomReadInteger;
      [*] = ;   var x: Float = RecordCustomReadFloat;
      [*] = ;   var z: Float = RecordCustomReadFloat;
      [*] = ;
      [*] = ;   if uid<>0 then
      [*] = ;   begin
      [*] = ;      var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;      if hnd<>0 then
      [*] = ;      begin
      [*] = ;         var obj: Pointer = _unit_GetTObj(hnd);
      [*] = ;         if obj<>nil then
      [*] = ;         begin
      [*] = ;            TObj(obj).bstandground:=false;
      [*] = ;            if not addo then
      [*] = ;            _unit_ClearOrders(hnd);
      [*] = ;            var pOrder : Pointer = _unit_OrderMove(hnd, x, z, dirx, dirz, mode, dofirst);
      [*] = ;            if (pOrder<>nil) then
      [*] = ;            TOrder(pOrder).info.amount := gPathTag;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      ErrorLog('ReadMove hnd=0');
      [*] = ;   end
      [*] = ;   else
      [*] = ;   ErrorLog('ReadMove uid=0');
      [*] = ;end;
      [*] = ;
      [*] = ;var bOldVersion : Boolean = (gRecordGeneratorVersion<91) and (_net_IsReplay);
      [*] = ;if (not bOldVersion) then
      [*] = ;begin
      [*] = ;   var squaduid : Word = RecordCustomReadWord;
      [*] = ;   if (squaduid>0) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = RecordCustomReadByte;
      [*] = ;      var pSquad : Pointer = gPlayer[plInd].squads.GetByUID(squaduid);
      [*] = ;      if (pSquad<>nil) then
      [*] = ;      begin
      [*] = ;         TSquad(psquad).fHoldMode := False;
      [*] = ;         TSquad(psquad).fHoldModeProgress := 0;
      [*] = ;         TSquad(psquad).fStandGround := False
      [*] = ;      end
      [*] = ;      else
      [*] = ;      ErrorLog('WriteMove:psquad=nil');
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if gPathTag < 255 then
      [*] = ;gPathTag := gPathTag+1
      [*] = ;else
      [*] = ;gPathTag := 1;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

