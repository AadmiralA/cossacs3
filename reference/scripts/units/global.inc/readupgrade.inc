section.begin
   Name = ReadUpgrade
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bOldVersion : Boolean = (gRecordGeneratorVersion<53) and (_net_IsReplay);
      [*] = ;
      [*] = ;var bFromServer: Boolean;
      [*] = ;if (not bOldVersion) then
      [*] = ;bFromServer:= RecordCustomReadBoolean;
      [*] = ;var upgid: Integer = RecordCustomReadInteger;
      [*] = ;var state: Boolean = RecordCustomReadBoolean;
      [*] = ;var count: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;_net_intlist.Clear();
      [*] = ;
      [*] = ;var i: Integer;
      [*] = ;for i:=0 to count-1 do begin
      [*] = ;   var hnduid: Integer = RecordCustomReadInteger;
      [*] = ;   var handle: Integer = GetGameObjectHandleByUniqueId(hnduid);
      [*] = ;   if handle<>0 then
      [*] = ;   _net_intlist.Add(handle)
      [*] = ;   else _net_ErrorLog('upgid('+IntToStr(upgid)+') invalid uid('+IntToStr(hnduid)+')');
      [*] = ;end;
      [*] = ;
      [*] = ;if (not bOldVersion) then
      [*] = ;begin
      [*] = ;   if _net_IsServer then begin
      [*] = ;      arg_plr.fintlst:=_net_intlist;
      [*] = ;      arg_plr.fcid:=upgid;
      [*] = ;      arg_plr.fbstate:=state;
      [*] = ;      ExecuteState('WriteUpgrade');
      [*] = ;   end
      [*] = ;   else
      [*] = ;   if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;   begin // _net_IsClient or _net_IsReplay
      [*] = ;      var bldHnd : Integer = _net_intlist.Get(0);
      [*] = ;      var bcanmake : Boolean;
      [*] = ;      if (bldHnd<>0) then
      [*] = ;      begin
      [*] = ;         var pobj : Pointer = _unit_GetTObj(bldHnd);
      [*] = ;         var cid : Integer = TObj(pobj).cid;
      [*] = ;         var upgsid : String;
      [*] = ;         _country_GetUpgradeSIDByUpgradeID(cid, upgid, upgsid);
      [*] = ;         if (state) then
      [*] = ;         begin
      [*] = ;            _unit_OrderUpgrade(bldHnd, upgsid);
      [*] = ;            _unit_ApplyUpgradeCostBySID(plHnd, cid, upgsid);
      [*] = ;            gPlayer[plInd].upgstate[cid][upgid].inprogress := True;
      [*] = ;            gPlayer[plInd].upgstate[cid][upgid].timestart := GetGameTime;
      [*] = ;            bcanmake := True;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            _unit_CancelUpgradePerform(bldHnd, upgsid);
      [*] = ;            bcanmake := True;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      if (not bcanmake) then
      [*] = ;      _net_ErrorLog('not _unit_MakeUpgrade()');
      [*] = ;   end;
      [*] = ;end
      [*] = ;else
      [*] = ;_unit_MakeUpgrade(GetPlayerCurrentPlHandle, _net_intlist, upgid, state, false);
      [*] = ;
      [*] = ;if _net_debug_log_upgrade then
      [*] = ;_net_Log('readupgrade(count='+IntToStr(count)+';upgid='+IntToStr(upgid)+';state='+BoolToStr(state)+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

