section.begin
   Name = WriteSquadNew
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;var pl       : Integer = arg_plr.fid; // to identify, which player create squad and we should auto-select this squad. In ca_se when several players plays for 1 color (plInd)=Have same interfaceIO;
      [*] = ;var sid      : String  = arg_plr.fsid;
      [*] = ;var formind  : Integer = arg_plr.find;
      [*] = ;var offhnd   : Integer = arg_plr.fofficer;
      [*] = ;var drmhnd   : Integer = arg_plr.fdrummer;
      [*] = ;var bposition: Boolean = arg_plr.fposition;
      [*] = ;var binsquad : Boolean = arg_plr.fbstate;
      [*] = ;var list     : Pointer = arg_plr.fintlst;
      [*] = ;var squid    : Integer = -1;
      [*] = ;var sqind    : Integer = -1;
      [*] = ;var psquad   : Pointer = nil;
      [*] = ;
      [*] = ;var netlist : TIntegerList;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then begin
      [*] = ;   var bCreateUsingList : Boolean = (list<>nil) and (TIntegerList(list).GetCount>0);
      [*] = ;   if (bCreateUsingList) then
      [*] = ;   begin
      [*] = ;      psquad := _player_CreateSquad(plhnd, formind, offhnd, drmhnd, TIntegerList(list), bposition, binsquad);
      [*] = ;      if (psquad<>nil) then
      [*] = ;      begin
      [*] = ;         var i : Integer;
      [*] = ;         for i:=0 to TIntegerList(list).GetCount-1 do
      [*] = ;         netlist.Add(TIntegerList(list).Get(i));
      [*] = ;         squid := TSquad(psquad).fUID;
      [*] = ;         sqind := TSquad(pSquad).fIndex;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   if (offhnd<>0) then
      [*] = ;   begin
      [*] = ;      gTmpIntListGUISquadList.Clear;
      [*] = ;      if (formind>=0) then
      [*] = ;      begin
      [*] = ;         var rad : Float = gc_gameplay_officersearchradius*1.25; // Server search units in larger radius, in ca_se if client and server units position not sync.
      [*] = ;         if _unit_MakeSquadList(offhnd, formind, -1, rad, sid, gTmpIntListGUISquadList, true) then
      [*] = ;         begin
      [*] = ;            netlist := gTmpIntListGUISquadList;
      [*] = ;            var count : Integer = gTmpIntListGUISquadList.GetCount;
      [*] = ;            var officerHnd : Integer = gTmpIntListGUISquadList.Get(count-2);
      [*] = ;            var drummerHnd : Integer = gTmpIntListGUISquadList.Get(count-1);
      [*] = ;            if _unit_IsDrummer(drummerHnd) then
      [*] = ;            gTmpIntListGUISquadList.Delete(count-1)
      [*] = ;            else
      [*] = ;            drummerHnd := 0;
      [*] = ;            if _unit_IsOfficer(officerHnd) then
      [*] = ;            gTmpIntListGUISquadList.Delete(count-2)
      [*] = ;            else
      [*] = ;            officerHnd := 0;
      [*] = ;            psquad := _player_CreateSquad(plhnd, formind, officerHnd, drummerHnd, gTmpIntListGUISquadList, bposition, binsquad);
      [*] = ;            if (psquad<>nil) then
      [*] = ;            begin
      [*] = ;               sqind := TSquad(pSquad).fIndex;
      [*] = ;               squid := TSquad(psquad).fUID;
      [*] = ;               if (plhnd=GetPlayerHandleInterfaceIO) then
      [*] = ;               _control_SelectOnlySquad(psquad);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         netlist.Add(offhnd);
      [*] = ;         psquad := _player_CreateSquad(plHnd, formind, 0, 0, netlist, false, false);
      [*] = ;         if (psquad<>nil) then
      [*] = ;         begin
      [*] = ;            sqind := TSquad(pSquad).fIndex;
      [*] = ;            squid := TSquad(psquad).fUID;
      [*] = ;         end;
      [*] = ;         offhnd := 0;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      psquad := _player_CreateSquad(plhnd, formind, offhnd, drmhnd, TIntegerList(list), bposition, binsquad);
      [*] = ;      if (psquad<>nil) then
      [*] = ;      begin
      [*] = ;         sqind := TSquad(pSquad).fIndex;
      [*] = ;         squid := TSquad(psquad).fUID;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      _net_ErrorLog('WriteSquadNew : unknown data');
      [*] = ;   end;
      [*] = ;end;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   if (psquad<>0) or (not ((_net_IsServer) or (_net_IsOffline))) then
      [*] = ;   begin
      [*] = ;      // create package
      [*] = ;      if not RecordCustomBegin('ReadSquadNew') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadSquadNew'')');
      [*] = ;
      [*] = ;      var offuid : Integer = GetGameObjectUniqueIdByHandle(offhnd);
      [*] = ;      var drmuid : Integer = GetGameObjectUniqueIdByHandle(drmhnd);
      [*] = ;
      [*] = ;      // write data to package
      [*] = ;      RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;      RecordCustomWriteInteger(pl);
      [*] = ;      RecordCustomWriteString(sid);
      [*] = ;      RecordCustomWriteInteger(formind);
      [*] = ;      RecordCustomWriteInteger(offuid);
      [*] = ;      RecordCustomWriteInteger(drmuid);
      [*] = ;      RecordCustomWriteBoolean(bposition);
      [*] = ;      RecordCustomWriteBoolean(binsquad);
      [*] = ;      RecordCustomWriteInteger(squid);
      [*] = ;      RecordCustomWriteInteger(netlist.GetCount); // count
      [*] = ;      var i: Integer;
      [*] = ;      for i:=0 to netlist.GetCount-1 do begin
      [*] = ;         var hnd: Integer = netlist.Get(i);
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
      [*] = ;// do after finishing rec.ord/package to create new squad.
      [*] = ;if (psquad<>nil) and ((_net_IsServer) or (_net_IsOffline)) then
      [*] = ;begin
      [*] = ;   if (formind>=0) then
      [*] = ;   _player_SetSquadFormation(plhnd, sqind, formind, bposition, True)
      [*] = ;   else
      [*] = ;   _squad_SetCustomFormation(psquad, True);
      [*] = ;
      [*] = ;   {if (bposition) then
      [*] = ;   begin
      [*] = ;      var posx, posz, dirx, dirz : Float;
      [*] = ;      _squad_GetCentralPosAndDir(psquad, posx, posz, dirx, dirz);
      [*] = ;      _player_SetSquadPosition(plhnd, sqind, posx, posz, dirx, dirz, false, false, gc_obj_order_move_mode_default, True);
      [*] = ;   end;}
      [*] = ;end;
      [*] = ;
      [*] = ;arg_plr.fpointer0 := psquad;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

