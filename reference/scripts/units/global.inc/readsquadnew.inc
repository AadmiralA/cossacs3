section.begin
   Name = ReadSquadNew
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bFromServer : Boolean = RecordCustomReadBoolean;
      [*] = ;var pl       : Integer = RecordCustomReadInteger;
      [*] = ;var sid      : String  = RecordCustomReadString;
      [*] = ;var formind  : Integer = RecordCustomReadInteger;
      [*] = ;var offuid   : Integer = RecordCustomReadInteger;
      [*] = ;var drmuid   : Integer = RecordCustomReadInteger;
      [*] = ;var bposition: Boolean = RecordCustomReadBoolean;
      [*] = ;var binsquad : Boolean = RecordCustomReadBoolean;
      [*] = ;var squid    : Integer = RecordCustomReadInteger;
      [*] = ;var count    : Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;_net_intlist.Clear;
      [*] = ;
      [*] = ;if _net_IsServer then begin
      [*] = ;   var offhnd : Integer = GetGameObjectHandleByUniqueId(offuid);
      [*] = ;   var drmhnd : Integer = GetGameObjectHandleByUniqueId(drmuid);
      [*] = ;   arg_plr.fsid := sid;
      [*] = ;   arg_plr.find := formind;
      [*] = ;   arg_plr.fofficer := offhnd;
      [*] = ;   arg_plr.fdrummer := drmhnd;
      [*] = ;   arg_plr.fposition := bposition;
      [*] = ;   arg_plr.fbstate := binsquad;
      [*] = ;   arg_plr.fintlst := _net_intlist;
      [*] = ;   arg_plr.fsquad := squid;
      [*] = ;   ExecuteState('WriteSquadNew');
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;begin // _net_IsClient or _net_IsReplay
      [*] = ;   // do
      [*] = ;   //if (count>0) then // ai squad can be empty
      [*] = ;   begin
      [*] = ;      var i: Integer;
      [*] = ;      for i:=0 to count-1 do
      [*] = ;      begin
      [*] = ;         var uid: Integer = RecordCustomReadInteger;
      [*] = ;         var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;         if (hnd<>0) then
      [*] = ;         _net_intlist.Add(hnd);
      [*] = ;      end;
      [*] = ;      var psquad : Pointer = nil;
      [*] = ;      if (formind>=0) then
      [*] = ;      begin
      [*] = ;         count := _net_intlist.GetCount;
      [*] = ;         var officerHnd : Integer = _net_intlist.Get(count-2);
      [*] = ;         var drummerHnd : Integer = _net_intlist.Get(count-1);
      [*] = ;         if _unit_IsDrummer(drummerHnd) then
      [*] = ;         _net_intlist.Delete(count-1)
      [*] = ;         else
      [*] = ;         drummerHnd := 0;
      [*] = ;         if _unit_IsOfficer(officerHnd) then
      [*] = ;         _net_intlist.Delete(count-2)
      [*] = ;         else
      [*] = ;         officerHnd := 0;
      [*] = ;         psquad := _player_CreateSquad(plhnd, formind, officerHnd, drummerHnd, _net_intlist, bposition, binsquad);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      psquad := _player_CreateSquad(plHnd, formind, 0, 0, _net_intlist, bposition, binsquad);
      [*] = ;
      [*] = ;      if (psquad<>nil) then
      [*] = ;      begin
      [*] = ;         if (TSquad(psquad).fUID<>squid) then
      [*] = ;         begin
      [*] = ;            if (squid<0) then
      [*] = ;            Log('ReadSquadNew squid<0. something realy wrong');
      [*] = ;            // perhaps several formations at one time were created and packages come in wrong order
      [*] = ;            Log('ReadSquadNew fUID<>sqUID. squid='+IntToStr(squid)+' it is now force set at client machine');
      [*] = ;            TSquad(psquad).fUID := squid;
      [*] = ;         end;
      [*] = ;         if (plhnd=GetPlayerHandleInterfaceIO) and (TSquad(psquad).GetCount>1) and (not _net_IsReplay) then // not ship
      [*] = ;         _control_SelectOnlySquad(psquad);
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   // logs
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

