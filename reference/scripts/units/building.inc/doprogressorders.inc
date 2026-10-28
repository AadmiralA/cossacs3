section.begin
   Name = DoProgressOrders
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;args arg_stolist : TIntegerList;
      [*] = ;args arg_attlist : TIntegerList;
      [*] = ;args arg_ordlist : TIntegerList;
      [*] = ;args arg_inside : TIntegerList;
      [*] = ;
      [*] = ;var bProcess : Boolean = not (_net_IsClient or _net_IsReplay);
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var plHnd : Integer = GetGameObjectPlayerHandleByHandle(myHnd);
      [*] = ;var plind : Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;function GetPortPlaceIsFree(const goHnd : Integer) : Boolean;
      [*] = ;begin
      [*] = ;   Result := True;
      [*] = ;   const cMinDist = 3;
      [*] = ;   var posx, posz : Float;
      [*] = ;   if (_misc_GetPortResourcePoint(goHnd, posx, posz, True)) then
      [*] = ;   begin
      [*] = ;      GetGameObjectsInRadius(posx, posz, cMinDist, false, true, 0, -1, 0, false, false, false, false, false, false);
      [*] = ;      var i : Integer;
      [*] = ;      for i := GetGameObjectListCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var objHnd : Integer = GetGameObjectListByIndex(i);
      [*] = ;         if (objHnd<>0) then
      [*] = ;         begin
      [*] = ;            if (_misc_GetBaseObjID(objHnd)=gc_baseid_obj) then
      [*] = ;            begin
      [*] = ;               var pObj : Pointer =  _unit_GetTObj(objHnd);
      [*] = ;               if (pObj<>nil) and (gObjProp[TObj(pobj).cid][TObj(pobj).id].explmedia=gc_obj_explmedia_ship) and (not TObj(pobj).bdead) and (TObj(pobj).bbuilt) and (VectorDistance(GetGameObjectPositionXByHandle(objHnd), 0, GetGameObjectPositionZByHandle(objHnd), posx, 0, posz)<cMinDist) then
      [*] = ;               begin
      [*] = ;                  if (TObj(pobj).orders[0].itype<>gc_obj_order_type_move) and (TObj(pobj).orders[0].itype<>gc_obj_order_type_attackobj) and (TObj(pobj).orders[0].itype<>gc_obj_order_type_attackpoint) then
      [*] = ;                  begin
      [*] = ;                     if (not GetGameObjectCIMovableByHandle(goHnd)) then
      [*] = ;                     continue;
      [*] = ;                     var px, pz : Float;
      [*] = ;                     if (_unit_GetRandomWaterPosition(objHnd, 4, px, pz)) then
      [*] = ;                     _unit_OrderMove(objHnd, px, pz, 0, 0, gc_obj_order_move_mode_default, True);
      [*] = ;                  end;
      [*] = ;                  Result := False;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;function GetShipDummy(const goHnd : Integer) : Integer;
      [*] = ;begin
      [*] = ;   Result := 0;
      [*] = ;   var plHnd : Integer = GetGameObjectPlayerHandleByHandle(goHnd);
      [*] = ;   if (plHnd<>0) then
      [*] = ;   begin
      [*] = ;      var posx, posz : Float;
      [*] = ;      if (_misc_GetPortResourcePoint(goHnd, posx, posz, True)) then
      [*] = ;      begin
      [*] = ;         GetGameObjectsInRadius(posx, posz, 2, false, true, plHnd, -1, 0, false, false, false, false, false, false);
      [*] = ;         var i : Integer;
      [*] = ;         for i := GetGameObjectListCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var objHnd : Integer = GetGameObjectListByIndex(i);
      [*] = ;            if (objHnd<>0) then
      [*] = ;            begin
      [*] = ;               if (_misc_GetBaseObjID(objHnd)=gc_baseid_obj) then
      [*] = ;               begin
      [*] = ;                  var pObj : Pointer = _unit_GetTObj(objHnd);
      [*] = ;                  if (pObj<>nil) and (gObjProp[TObj(pobj).cid][TObj(pobj).id].explmedia=gc_obj_explmedia_ship) and (not TObj(pobj).bbuilt) and (not TObj(pobj).bdead) then
      [*] = ;                  begin
      [*] = ;                     Result := objHnd;
      [*] = ;                     break;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;procedure ProgressShipDummy(const shipHnd : Integer; const progress : Float);
      [*] = ;begin
      [*] = ;   var stage : Integer = floor(progress*4);
      [*] = ;   var visualtag : Integer = gc_statetag_visual_none;
      [*] = ;   case stage of
      [*] = ;      0 : visualtag := gc_statetag_visual_stage_0;
      [*] = ;      1 : visualtag := gc_statetag_visual_stage_1;
      [*] = ;      2 : visualtag := gc_statetag_visual_stage_2;
      [*] = ;      3 : visualtag := gc_statetag_visual_stage_3;
      [*] = ;   end;
      [*] = ;   //_unit_SetShipDummyOffset(shipHnd);
      [*] = ;   _unit_SetTagStates(shipHnd, visualtag or gc_statetag_essential_birth);
      [*] = ;end;
      [*] = ;
      [*] = ;procedure CreateShipDummy(const portHnd, cid, id : Integer);
      [*] = ;begin
      [*] = ;   _unit_RemoveShipDummy(portHnd);
      [*] = ;   var posx, posz : Float;
      [*] = ;   if (_misc_GetPortResourcePoint(portHnd, posx, posz, True)) then
      [*] = ;   begin
      [*] = ;      _unit_SetTagStates(portHnd, gc_statetag_action_build);
      [*] = ;      var plHnd : Integer = GetGameObjectPlayerHandleByHandle(portHnd);
      [*] = ;      var plInd : Integer = GetPlayerIndexByHandle(plHnd);
      [*] = ;      var basename : String = gObjProp[cid][id].sid;
      [*] = ;      gint_unit_parentcid := arg_obj.cid;
      [*] = ;      var roll : Float = gc_obj_shipcontructrollangle;
      [*] = ;      var parg: Pointer = StateMachineGetArgDataByInd(GetPlayerStateMachineHandle(plHnd), gc_argplayer_plr);
      [*] = ;      TPlayerArgs(parg).fposx:=posx;
      [*] = ;      TPlayerArgs(parg).fposz:=posz;
      [*] = ;      TPlayerArgs(parg).fdirx:=roll;
      [*] = ;      TPlayerArgs(parg).fracename:=gc_racename_units;
      [*] = ;      TPlayerArgs(parg).fbasename:=basename;
      [*] = ;      TPlayerArgs(parg).fplayer:=plInd;
      [*] = ;      PlayerExecuteStateByHandle(plHnd, 'WriteNewP');
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// ќбработка текущего приказа
      [*] = ;case arg_obj.orders[0].itype of
      [*] = ;   gc_obj_order_type_none : begin
      [*] = ;   end;
      [*] = ;   gc_obj_order_type_produce : begin
      [*] = ;      // прогрессим приказ производства юнита
      [*] = ;      var porderinfo : Pointer = arg_obj.orders[0].info;
      [*] = ;      var cid : Integer = TOrderInfo(porderinfo).upgradeid; // stores cid for unit produce
      [*] = ;      var id : Integer = TOrderInfo(porderinfo).produceid;
      [*] = ;      if (id<>0) and ((TOrderInfo(porderinfo).amount>0) or (TOrderInfo(porderinfo).amount=gc_obj_order_produce_infinite)) then
      [*] = ;      begin
      [*] = ;         if (gObjProp[cid][id].usage=gc_obj_usage_field) then
      [*] = ;         begin
      [*] = ;            if (bProcess) then
      [*] = ;            _unit_DoSeedWheat(myHnd, False);
      [*] = ;            TOrderInfo(porderinfo).amount := 0;
      [*] = ;            _unit_RemoveOrder(myHnd, 0);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            var bStart : Boolean = (TOrderInfo(porderinfo).progress=0); // поступил приказ на начало производства юнита. ≈сли progress>0, то юнит уже прошел стадию заказа и лишь продолжает строитьс€.
      [*] = ;            var bEnoughRes, bEnoughResInfinite : Boolean = True;
      [*] = ;            var bPortPlaceIsFree : Boolean = True;
      [*] = ;            var bFarmOk : Boolean = (gPlayer[arg_obj.pl].counter.farmused<gPlayer[arg_obj.pl].farm);
      [*] = ;            case gMap.settings.additional.limit of
      [*] = ;               1 : bFarmOk := (gPlayer[arg_obj.pl].counter.farmused<Min(gPlayer[arg_obj.pl].farm, gc_mapsettings_limit_1));
      [*] = ;               2 : bFarmOk := (gPlayer[arg_obj.pl].counter.farmused<Min(gPlayer[arg_obj.pl].farm, gc_mapsettings_limit_2));
      [*] = ;               3 : bFarmOk := (gPlayer[arg_obj.pl].counter.farmused<Min(gPlayer[arg_obj.pl].farm, gc_mapsettings_limit_3));
      [*] = ;               4 : bFarmOk := (gPlayer[arg_obj.pl].counter.farmused<Min(gPlayer[arg_obj.pl].farm, gc_mapsettings_limit_4));
      [*] = ;               5 : bFarmOk := (gPlayer[arg_obj.pl].counter.farmused<Min(gPlayer[arg_obj.pl].farm, gc_mapsettings_limit_5));
      [*] = ;               6 : bFarmOk := (gPlayer[arg_obj.pl].counter.farmused<Min(gPlayer[arg_obj.pl].farm, gc_mapsettings_limit_6));
      [*] = ;               7 : bFarmOk := (gPlayer[arg_obj.pl].counter.farmused<Min(gPlayer[arg_obj.pl].farm, gc_mapsettings_limit_7));
      [*] = ;               8 : bFarmOk := (gPlayer[arg_obj.pl].counter.farmused<Min(gPlayer[arg_obj.pl].farm, gc_mapsettings_limit_8));
      [*] = ;            end;
      [*] = ;            // artillery fix
      [*] = ;            var artind : Integer = gObjProp[cid][id].artind;
      [*] = ;            var bArtOk : Boolean;
      [*] = ;            var bArtillery : Boolean = (artind>=0);
      [*] = ;            var iconName : String;
      [*] = ;            _misc_GetUnitIconNameByID(cid, id, iconName);
      [*] = ;            // ƒостаточно ли жилых мест и не достигнут ли общий лимит населени€. ¬ыдача соответствующего сообщени€ игроку, в случае отказа в производстве.
      [*] = ;            var unitcount : Integer;
      [*] = ;            var i : Integer;
      [*] = ;            for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;            unitcount := unitcount+gPlayer[i].counter.total;
      [*] = ;            var bUnitLimitOk : Boolean = (unitcount<gc_MaxObjCount);
      [*] = ;            // artillery check
      [*] = ;            if (bArtillery) then
      [*] = ;            bArtOk := (gPlayer[arg_obj.pl].artcount[artind]<gPlayer[arg_obj.pl].artlimit[artind])
      [*] = ;            else
      [*] = ;            bArtOk := True;
      [*] = ;            if (bStart) then
      [*] = ;            begin
      [*] = ;               // Show log if player InterfaceIO
      [*] = ;               var bIO : Boolean = (plHnd=GetPlayerHandleInterfaceIO);
      [*] = ;               if (bIO) and ((not bFarmOk) or (not bUnitLimitOk)) then
      [*] = ;               begin
      [*] = ;                  var s1, s2 : String;
      [*] = ;                  if (not bUnitLimitOk) then
      [*] = ;                  _misc_ShowLogMessage(gc_logmessagesid_populationlimitreached, s1, s2, False)
      [*] = ;                  else
      [*] = ;                  if (not bFarmOk) then
      [*] = ;                  _misc_ShowLogMessage(gc_logmessagesid_notenoughfarms, s1, s2, False);
      [*] = ;               end;
      [*] = ;               // ƒостаточно ли ресурсов дл€ строительства. ¬ыдача соответствующего сообщени€ игроку, в случае отказа в производстве.
      [*] = ;               if (TOrderInfo(porderinfo).amount=gc_obj_order_produce_infinite) then
      [*] = ;               begin
      [*] = ;                  if (not _unit_CanApplyCostByID(cid, id, arg_obj.pl)) then
      [*] = ;                  begin
      [*] = ;                     bEnoughRes := False;
      [*] = ;                     if (bIO) then
      [*] = ;                     begin
      [*] = ;                        var txtres, s2 : String;
      [*] = ;                        var food, wood, stone, gold, iron, coal : Integer;
      [*] = ;                        _unit_GetCostByID(cid, id, arg_obj.pl, food, wood, stone, gold, iron, coal);
      [*] = ;                        _misc_PrepareLogMessageNotEnoughResourcesText(txtres, food, wood, stone, gold, iron, coal);
      [*] = ;                        _misc_ShowLogMessage(gc_logmessagesid_notenoughres, txtres, s2, False);
      [*] = ;                        bEnoughResInfinite := False;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;                  if (not gPlayer[arg_obj.pl].res[gc_resource_type_food]<500) and (gPlayer[plInd].objbase[cid][id].price[gc_resource_type_food]>0) then
      [*] = ;                  bEnoughResInfinite := False;
      [*] = ;               end;
      [*] = ;               if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) then
      [*] = ;               bPortPlaceIsFree := GetPortPlaceIsFree(myHnd);
      [*] = ;            end;
      [*] = ;            if (bFarmOk) and (bEnoughResInfinite) and (bEnoughRes) and (bUnitLimitOk) and (bPortPlaceIsFree) and (bArtOk) then
      [*] = ;            begin
      [*] = ;               // в случае "старта производства", через механизм "заказ бесконечности штук" взымаем плату за юнита
      [*] = ;               if (bStart) then
      [*] = ;               begin
      [*] = ;                  if ((TOrderInfo(porderinfo).amount=gc_obj_order_produce_infinite)) and (_unit_CanApplyCostByID(cid, id, arg_obj.pl)) then
      [*] = ;                  _unit_ApplyCostByID(cid, id, arg_obj.pl);
      [*] = ;                  if (bProcess) and (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) then
      [*] = ;                  CreateShipDummy(myHnd, cid, id);
      [*] = ;               end;
      [*] = ;
      [*] = ;               // изменение прогресса произсодства юнита. ≈сли прогресс достиг 1, значит юнит достроилс€ и его пора создавать
      [*] = ;               var lastprogresstime : Float = arg_obj.lastprogresstime;
      [*] = ;               var deltatime : Float = GetGameTime-lastprogresstime;
      [*] = ;               if (lastprogresstime>0) and (deltatime>0) then
      [*] = ;               begin
      [*] = ;                  var deltabuildtime : Float = (deltatime/gPlayer[arg_obj.pl].objbase[cid][id].buildtime);
      [*] = ;                  if (gPlayer[plind].bai) and (not gPlayer[plInd].aiData.bhumanai) then
      [*] = ;                  begin
      [*] = ;                     case gPlayer[plind].difficulty of
      [*] = ;                        gc_player_difficulty_easy : deltabuildtime:=deltabuildtime*0.3; //easy
      [*] = ;                        gc_player_difficulty_normal : deltabuildtime:=deltabuildtime*0.50; //normal
      [*] = ;                        gc_player_difficulty_hard : deltabuildtime:=deltabuildtime*0.75; //hard
      [*] = ;                        gc_player_difficulty_veryhard : deltabuildtime:=deltabuildtime*1; //very hard
      [*] = ;                        gc_player_difficulty_impossible : deltabuildtime:=deltabuildtime*1.25; //impossible
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;                  TOrderInfo(porderinfo).progress := TOrderInfo(porderinfo).progress+deltabuildtime;
      [*] = ;                  if (TOrderInfo(porderinfo).progress>1) then
      [*] = ;                  TOrderInfo(porderinfo).progress := 1;
      [*] = ;                  if (gPlayer[plind].bai) and (not gPlayer[plInd].aidata.bhumanai) and (TOrderInfo(porderinfo).progress>=1) then
      [*] = ;                  begin
      [*] = ;                     if (_unit_CheckCapturersStopProduce(myHnd)) then // ai do not produce units when enemy near
      [*] = ;                     TOrderInfo(porderinfo).progress := 0.995;
      [*] = ;                  end;
      [*] = ;                  if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) and (TOrderInfo(porderinfo).progress<1) then
      [*] = ;                  begin
      [*] = ;                     var shipDummy : Integer = GetShipDummy(myHnd);
      [*] = ;                     if (shipDummy<>0) then
      [*] = ;                     ProgressShipDummy(shipDummy, TOrderInfo(porderinfo).progress);
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if (TOrderInfo(porderinfo).progress>=1) and (bProcess) then // bDone
      [*] = ;                  begin
      [*] = ;                     var px, py, pz : Float;
      [*] = ;                     var exitCount : Integer;
      [*] = ;                     if (bProcess) then
      [*] = ;                     begin
      [*] = ;                        exitCount := gCustomObjPoints[arg_obj.cid, arg_obj.id].exitCount;
      [*] = ;                        if (exitCount>0) then
      [*] = ;                        begin
      [*] = ;                           px := GetGameObjectMyPositionX+gCustomObjPoints[arg_obj.cid, arg_obj.id].exitPoints[exitCount-1].x;
      [*] = ;                           py := GetGameObjectMyPositionY+gCustomObjPoints[arg_obj.cid, arg_obj.id].exitPoints[exitCount-1].y;
      [*] = ;                           pz := GetGameObjectMyPositionZ+gCustomObjPoints[arg_obj.cid, arg_obj.id].exitPoints[exitCount-1].z;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           px := GetGameObjectMyPositionX;
      [*] = ;                           py := GetGameObjectMyPositionY;
      [*] = ;                           pz := GetGameObjectMyPositionZ;
      [*] = ;                           //ErrorLog('No start point for creating unit!');
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;
      [*] = ;                     if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) then
      [*] = ;                     _misc_GetPortResourcePoint(myHnd, px, pz, True);
      [*] = ;
      [*] = ;                     // изменение счетчика числа заказанных дл€ производства юнитов, и удаление приказа, если счетчик достиг 0
      [*] = ;                     if (TOrderInfo(porderinfo).amount<>gc_obj_order_produce_infinite) then
      [*] = ;                     TOrderInfo(porderinfo).amount := TOrderInfo(porderinfo).amount-1;
      [*] = ;                     TOrderInfo(porderinfo).progress := 0;
      [*] = ;
      [*] = ;                     var sid : String;
      [*] = ;                     _unit_ConvertObjIDToSID(cid, id, sid);
      [*] = ;
      [*] = ;                     if (not ((gScenario.bactive) and (arg_obj.pl<>0) and (not gPlayer[arg_obj.pl].bai))) then
      [*] = ;                     if (TOrderInfo(porderinfo).amount<>0) and (_player_CheckAccessControlRequirements(plHnd, cid, sid)<>gc_result_checkaccesscontrolreq_ok) then
      [*] = ;                     begin
      [*] = ;                        if (bProcess) then
      [*] = ;                        begin
      [*] = ;                           gIntegerList.Clear;
      [*] = ;                           gIntegerList.Add(myHnd);
      [*] = ;                           _unit_ProduceUnit(plHnd, gIntegerList, cid, id, TOrderInfo(porderinfo).amount, False, False, True);
      [*] = ;                        end;
      [*] = ;                        TOrderInfo(porderinfo).amount := 0;
      [*] = ;                     end;
      [*] = ;
      [*] = ;                     if (TOrderInfo(porderinfo).amount=0) then
      [*] = ;                     _unit_RemoveOrder(myHnd, 0)
      [*] = ;                     else
      [*] = ;                     if (TOrderInfo(porderinfo).amount=gc_obj_order_produce_infinite) then
      [*] = ;                     begin
      [*] = ;                        // если заказано "бесконечное производство", то смотрим, нет ли других заказов, и переключаемс€ на следующий. ј этот приказ кладем в конец списка приказов.
      [*] = ;                        var lastInd : Integer;
      [*] = ;                        var i : Integer;
      [*] = ;                        for i:=0 to gc_obj_MaxOrderCount-1 do
      [*] = ;                        begin
      [*] = ;                           if (arg_obj.orders[i].itype=gc_obj_order_type_none) then
      [*] = ;                           begin
      [*] = ;                              lastInd := i;
      [*] = ;                              break;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                        if (lastInd>0) then
      [*] = ;                        begin
      [*] = ;                           arg_obj.orders[lastInd] := arg_obj.orders[0];
      [*] = ;                           _unit_RemoveOrder(myHnd, 0);
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;
      [*] = ;                     var goHnd : Integer;
      [*] = ;                     if (bProcess) then
      [*] = ;                     begin
      [*] = ;                        var builduid : Integer = GetGameObjectUniqueIdByHandle(myHnd);
      [*] = ;                        if (not gbool_progress_usewritenewrequestlist) then
      [*] = ;                        begin
      [*] = ;                           var parg: Pointer = StateMachineGetArgDataByInd(GetPlayerStateMachineHandle(plHnd), gc_argplayer_plr);
      [*] = ;                           TPlayerArgs(parg).fposx:=px;
      [*] = ;                           TPlayerArgs(parg).fposz:=pz;
      [*] = ;                           TPlayerArgs(parg).fracename:=gc_racename_units;
      [*] = ;                           TPlayerArgs(parg).fbasename:=sid;
      [*] = ;                           TPlayerArgs(parg).fcid:=builduid;
      [*] = ;                           PlayerExecuteStateByHandle(plHnd, 'WriteNew');
      [*] = ;                           goHnd:=TPlayerArgs(parg).fresult;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        _player_AddToWriteNewRequestList(plind, gc_racename_units, sid, arg_obj.cid, builduid, px, pz, 0, False);
      [*] = ;                        {if (goHnd<>0) then
      [*] = ;                        begin
      [*] = ;                           // creating squad done in WriteNew
      [*] = ;                           var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                           if _unit_IsWaterUnit(goHnd) then
      [*] = ;                           begin
      [*] = ;                              TPlayerArgs(parg).fsid := sid;
      [*] = ;                              TPlayerArgs(parg).find := -1;
      [*] = ;                              TPlayerArgs(parg).fofficer := goHnd;
      [*] = ;                              TPlayerArgs(parg).fposition := True;
      [*] = ;                              TPlayerArgs(parg).fbstate := False;
      [*] = ;                              TPlayerArgs(parg).fintlst := nil;
      [*] = ;                              PlayerExecuteStateByHandle(plHnd, 'WriteSquadNew');
      [*] = ;                           end;
      [*] = ;                        end;}
      [*] = ;                     end;
      [*] = ;
      [*] = ;                     if (not gbool_progress_usewritenewrequestlist) and (bProcess) and (goHnd<>0) then
      [*] = ;                     begin
      [*] = ;                        if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) then
      [*] = ;                        begin
      [*] = ;                           var resHnd : Integer;
      [*] = ;                           _unit_GetBuildingExitData(goHnd, myHnd, False, resHnd, px, pz);
      [*] = ;                           _unit_RemoveShipDummy(myHnd);
      [*] = ;
      [*] = ;                           if _unit_GetUsage(goHnd) <> gc_obj_usage_fisher then
      [*] = ;                           _unit_OrderLeave(goHnd, myHnd, px, pz);
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           _unit_SetSTO(goHnd, myHnd);
      [*] = ;                           _unit_SetTagStates(goHnd, gc_statetag_visual_hide or gc_statetag_essential_birth);
      [*] = ;                           var bCheckRes : Boolean = arg_obj.brally;
      [*] = ;                           var resHnd : Integer;
      [*] = ;                           _unit_GetBuildingExitData(goHnd, myHnd, bCheckRes, resHnd, px, pz);
      [*] = ;                           _unit_OrderLeave(goHnd, myHnd, px, pz);
      [*] = ;                           if (resHnd<>0) then
      [*] = ;                           _unit_OrderGainResource(goHnd, resHnd, GetGameObjectPositionXByHandle(resHnd), GetGameObjectPositionZByHandle(resHnd));
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      _unit_RemoveOrder(myHnd, 0);
      [*] = ;   end;
      [*] = ;   gc_obj_order_type_performupgrade : begin
      [*] = ;      // прогрессим приказ совершени€ апгрейда
      [*] = ;      _misc_UnitProgressOrderPerformUpgrade(myHnd);
      [*] = ;   end;
      [*] = ;   gc_obj_order_type_attackobj : begin
      [*] = ;      if (bProcess) then
      [*] = ;      begin
      [*] = ;         var porderinfo : Pointer = arg_obj.orders[0].info;
      [*] = ;         var trgHnd : Integer = TOrderInfo(porderinfo).trg;
      [*] = ;         if (trgHnd<>0) and (GetGameObjectPlayableObjectByHandle(trgHnd)) then
      [*] = ;         _unit_BuildingTryAttack(myHnd, trgHnd, False, 0, 0)
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            _unit_RemoveOrder(myHnd, 0);
      [*] = ;            _unit_SetTagStates(myHnd, gc_statetag_action_none);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   gc_obj_order_type_attackpoint : begin
      [*] = ;      if (bProcess) then
      [*] = ;      begin
      [*] = ;         var porderinfo : Pointer = arg_obj.orders[0].info;
      [*] = ;         _unit_BuildingTryAttack(myHnd, 0, True, TOrderInfo(porderinfo).dx, TOrderInfo(porderinfo).dy);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   gc_obj_order_type_exitunits : begin
      [*] = ;      if bProcess then
      [*] = ;      _unit_TryExitUnits(myHnd);
      [*] = ;   end
      [*] = ;   else
      [*] = ;   _unit_RemoveOrder(myHnd, 0);
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

