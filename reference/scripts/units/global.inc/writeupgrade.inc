section.begin
   Name = WriteUpgrade
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;if arg_plr.fintlst=nil then
      [*] = ;_net_ErrorLog('arg_plr.fintlst = nil');
      [*] = ;
      [*] = ;if(arg_plr.fintlst<>nil) and (TIntegerList(arg_plr.fintlst).GetCount=0) then
      [*] = ;_net_ErrorLog('TIntegerList(arg_plr.fintlst).GetCount = 0');
      [*] = ;
      [*] = ;var upgid: Integer = arg_plr.fcid;
      [*] = ;var state: Boolean = arg_plr.fbstate;
      [*] = ;var ilist: Pointer = arg_plr.fintlst;
      [*] = ;
      [*] = ;var bcanmake: Boolean;
      [*] = ;var trghnd: Integer;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then begin
      [*] = ;   var bldHnd : Integer = TIntegerList(ilist).Get(0);
      [*] = ;   if (bldHnd<>0) then
      [*] = ;   begin
      [*] = ;      var pobj : Pointer = _unit_GetTObj(bldHnd);
      [*] = ;      var plInd : Integer = GetPlayerIndexByHandle(plHnd);
      [*] = ;      var cid : Integer = TObj(pobj).cid;
      [*] = ;      var i : Integer;
      [*] = ;      for [MAIN]i := TIntegerList(ilist).GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var tmpHnd : Integer = TIntegerList(ilist).Get(i);
      [*] = ;         var pobj : Pointer = _unit_GetTObj(tmpHnd);
      [*] = ;         var bPerformUpgrade : Boolean;
      [*] = ;         var j : Integer;
      [*] = ;         for j:=gc_obj_MaxOrderCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            if (TObj(pobj).orders[j].itype=gc_obj_order_type_performupgrade) then
      [*] = ;            begin
      [*] = ;               bPerformUpgrade := True;
      [*] = ;               break;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         if (state) then
      [*] = ;         begin
      [*] = ;            if (not bPerformUpgrade) and (TObj(pobj).bbuilt) and (not TObj(pobj).bdead) then
      [*] = ;            begin
      [*] = ;               if (not gCountry[cid].upgrade[upgid].bindividual) or (gCountry[cid].upgrade[upgid].level=TObj(pobj).individual.upglevel+2) then
      [*] = ;               begin
      [*] = ;                  bldHnd := tmpHnd;
      [*] = ;                  break;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               bldHnd := 0;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            bldHnd := 0;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (bPerformUpgrade) then
      [*] = ;         begin
      [*] = ;            for j:=gc_obj_MaxOrderCount-1 downto 0 do
      [*] = ;            if (TObj(pobj).orders[j].info.upgradeid=upgid) then
      [*] = ;            begin
      [*] = ;               bldHnd := tmpHnd;
      [*] = ;               break(MAIN);
      [*] = ;            end;
      [*] = ;            bldHnd := 0;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      var upgsid : String;
      [*] = ;      _country_GetUpgradeSIDByUpgradeID(cid, upgid, upgsid);
      [*] = ;      if (bldHnd<>0) then
      [*] = ;      begin
      [*] = ;         if (state) then
      [*] = ;         begin
      [*] = ;            if (_unit_CanApplyUpgradeCostBySID(plHnd, cid, upgsid)) then
      [*] = ;            begin
      [*] = ;               if (not gPlayer[plInd].upgstate[cid][upgid].inprogress) or (gCountry[cid].upgrade[upgid].bindividual) then
      [*] = ;               begin
      [*] = ;                  _unit_OrderUpgrade(bldHnd, upgsid);
      [*] = ;                  _unit_ApplyUpgradeCostBySID(plHnd, cid, upgsid);
      [*] = ;                  gPlayer[plInd].upgstate[cid][upgid].inprogress := True;
      [*] = ;                  gPlayer[plInd].upgstate[cid][upgid].timestart := GetGameTime;
      [*] = ;                  bcanmake := True;
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            // Show log if player InterfaceIO
      [*] = ;            if (plHnd=GetPlayerHandleInterfaceIO) then
      [*] = ;            begin
      [*] = ;               var txtres, s2 : String;
      [*] = ;               var food, wood, stone, gold, iron, coal : Integer;
      [*] = ;               _country_GetUpgradeCostBySID(cid, upgsid, food, wood, stone, gold, iron, coal);
      [*] = ;               _misc_PrepareLogMessageNotEnoughResourcesText(txtres, food, wood, stone, gold, iron, coal);
      [*] = ;               _misc_ShowLogMessage(gc_logmessagesid_notenoughres, txtres, s2, False);
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            _unit_CancelUpgradePerform(bldHnd, upgsid);
      [*] = ;            bcanmake := True;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   if bcanmake then
      [*] = ;   trghnd := bldHnd;
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   if not (_net_IsServer and not bcanmake) then
      [*] = ;   begin
      [*] = ;      // create package
      [*] = ;      if not RecordCustomBegin('ReadUpgrade') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadUpgrade'')');
      [*] = ;
      [*] = ;      var count: Integer;
      [*] = ;      if ilist<>nil then
      [*] = ;      count:=TIntegerList(ilist).GetCount
      [*] = ;      else count:=0;
      [*] = ;
      [*] = ;      // write data to package
      [*] = ;      RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;      RecordCustomWriteInteger(upgid);
      [*] = ;      RecordCustomWriteBoolean(state);
      [*] = ;      if (trghnd<>0) then
      [*] = ;      begin
      [*] = ;         RecordCustomWriteInteger(1); // count
      [*] = ;         var trguid: Integer = GetGameObjectUniqueIdByHandle(trghnd);
      [*] = ;         RecordCustomWriteInteger(trguid);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         RecordCustomWriteInteger(count);
      [*] = ;
      [*] = ;         var i: Integer;
      [*] = ;         for i:=0 to count-1 do begin
      [*] = ;            var handle: Integer = TIntegerList(ilist).Get(i);
      [*] = ;            var hnduid: Integer = GetGameObjectUniqueIdByHandle(handle);
      [*] = ;            RecordCustomWriteInteger(hnduid);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if _net_debug_log_upgrade then
      [*] = ;      _net_Log('writeupgrade(count='+IntToStr(count)+';upgid='+IntToStr(upgid)+';state='+BoolToStr(state)+')');
      [*] = ;
      [*] = ;      if _net_debug_log_size_writer then
      [*] = ;      _net_Log_SizeWriter;
      [*] = ;
      [*] = ;      // finish package
      [*] = ;      RecordCustomEnd;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

