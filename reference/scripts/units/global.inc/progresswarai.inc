section.begin
   Name = ProgressWarAI
   Code : struct.begin
      [*] = ;//exit;
      [*] = ;const gc_ai_ZoneSizeFactor = 2; // difference between topology zone size in original Cossacks and in Cossacks 3
      [*] = ;//const gc_ai_ApproachDist = 16; // absolute dist between zone centers in original Cossacks
      [*] = ;//const gc_ai_ApproachDist = 2; // absolute dist between zone centers in original Cossacks
      [*] = ;var gc_ai_ApproachDist : Float = 1 * gc_ai_ZoneSizeFactor * TopologyGetZoneSize; // absolute dist between zone centers in original Cossacks
      [*] = ;var gc_ai_WaterApproachDist : Float = 2 * gc_ai_ZoneSizeFactor * TopologyGetZoneSize; // absolute dist between zone centers in original Cossacks
      [*] = ;var gc_ai_ScanGridFactor : Float = gc_scangrid_countx / 64; // difference between army grid in original Cossacks and in Cossacks3
      [*] = ;const gc_ai_GrenadierTargetFactor = 5;
      [*] = ;// 1 array index in original cossacks = 1/512 of standard map = 0.6 tiles
      [*] = ;
      [*] = ;var plHnd : Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plInd : Integer = GetPlayerIndexByHandle(plHnd);
      [*] = ;var bai : Boolean = gPlayer[plInd].bai;
      [*] = ;
      [*] = ;// _ai_ArmyCheckWallAttack
      [*] = ;//
      [*] = ;function _ai_ArmyCheckWallAttack(pArmy : Pointer) : Boolean;
      [*] = ;begin
      [*] = ;   Result := false;
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var wHnd : Integer;
      [*] = ;      if TArmy(pArmy).fOrder.targetUID <> 0 then
      [*] = ;      begin
      [*] = ;         var trgHnd : Integer = GetGameObjectHandleByUniqueId(TArmy(pArmy).fOrder.targetUID);
      [*] = ;         if trgHnd <> 0 then
      [*] = ;         begin
      [*] = ;            var pObjProp : Pointer = _unit_GetObjProp(trgHnd);
      [*] = ;            if TObjProp(pObjProp).bWall then
      [*] = ;            wHnd := trgHnd;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         _ai_ArmyClearOrder(pArmy);
      [*] = ;      end;
      [*] = ;
      [*] = ;      if wHnd <> 0 then
      [*] = ;      begin
      [*] = ;         var trgX : Float = GetGameObjectPositionXByHandle(wHnd);
      [*] = ;         var trgZ : Float = GetGameObjectPositionZByHandle(wHnd);
      [*] = ;
      [*] = ;         gIntegerList.Clear;
      [*] = ;         gPtrList.Clear;
      [*] = ;         var attack : Boolean;
      [*] = ;         var isArt : Boolean;
      [*] = ;         var i, j : Integer;
      [*] = ;         for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;            if (pSquad<>nil) then
      [*] = ;            begin
      [*] = ;               gPtrList.AddPointer(pSquad);
      [*] = ;               isArt := isArt or _ai_SquadIsArtillery(pSquad);
      [*] = ;
      [*] = ;               if not attack then
      [*] = ;               for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var goHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;
      [*] = ;                  var goX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;                  var goZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;                  var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;
      [*] = ;                  var maxRad : Float;
      [*] = ;                  if _unit_IsArtillery(goHnd) then
      [*] = ;                  maxRad := _unit_GetMaxAttackRadius(goHnd)
      [*] = ;                  else
      [*] = ;                  maxRad := TObjProp(pObjProp).searchRadius;
      [*] = ;
      [*] = ;                  if VectorDistance(trgX, 0, trgZ, goX, 0, goZ) < maxRad then
      [*] = ;                  begin
      [*] = ;                     attack := true;
      [*] = ;                     break;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if isArt then
      [*] = ;         attack := false
      [*] = ;         else
      [*] = ;         if attack then
      [*] = ;         begin
      [*] = ;            _player_OrderUnitsToAttack(TArmy(pArmy).fPlIndex, wHnd, gIntegerList, gPtrList, true, false);
      [*] = ;            TArmy(pArmy).fOrder.iType := gc_ai_armyorder_attackwall;
      [*] = ;         end;
      [*] = ;
      [*] = ;         Result := attack;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_TransportLandUnits
      [*] = ;//
      [*] = ;function _ai_TransportLandUnits(trHnd, island : Integer) : Boolean;
      [*] = ;begin
      [*] = ;   Result := false;
      [*] = ;   var pArmy : Pointer = _unit_GetArmy(trHnd);
      [*] = ;   var plHnd : Integer = GetGameObjectPlayerHandleByHandle(trHnd);
      [*] = ;   var plInd : Integer = GetPlayerIndexByHandle(plHnd);
      [*] = ;   var pInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;   if (pArmy <> nil) and (pInfo <> nil) then
      [*] = ;   begin
      [*] = ;      var trX : Float = GetGameObjectPositionXByHandle(trHnd);
      [*] = ;      var trZ : Float = GetGameObjectPositionZByHandle(trHnd);
      [*] = ;
      [*] = ;      var minDist : Float = gc_MaxFloat;
      [*] = ;      var landX, landZ : Float;
      [*] = ;      var i : Integer;
      [*] = ;      for i := TEnemyInfo(pInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pArmyInfo : Pointer = TEnemyInfo(pInfo).armyInfos.Get(i);
      [*] = ;         if (pArmyInfo<>nil) and (TArmyInfo(pArmyInfo).landCount > 0) then
      [*] = ;         begin
      [*] = ;            var posX, posZ : Float;
      [*] = ;            _misc_GetArmyInfoPosition(pArmyInfo, posX, posZ);
      [*] = ;            var dist : Float = VectorDistance(trX, 0, trZ, posX, 0, posZ);
      [*] = ;            var isl : Integer = _misc_GetIsland(posX, posZ);
      [*] = ;            if ((gIslands.count <= 1) or (isl <> island)) and (dist < minDist) then
      [*] = ;            begin
      [*] = ;               var landing : Integer = _ai_TestArmyBrushPosition(pArmy, posX, posZ, 0, 0, false);
      [*] = ;               if landing > 0 then
      [*] = ;               begin
      [*] = ;                  landX := posX;
      [*] = ;                  landZ := posZ;
      [*] = ;                  minDist := dist;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if minDist < gc_MaxFloat then
      [*] = ;      begin
      [*] = ;         _ai_SetArmyBrushPosition(pArmy, landX, landZ, 0, 0);
      [*] = ;         Result := true;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         if island >= 0 then
      [*] = ;         Result := _ai_TransportLandUnits(trHnd, -1);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_TransportPickArmy
      [*] = ;//
      [*] = ;function _ai_TransportPickArmy(trHnd : Integer) : Pointer;
      [*] = ;begin
      [*] = ;   var pResArmy : Pointer = nil;
      [*] = ;   var pObj : Pointer = _unit_GetTObj(trHnd);
      [*] = ;   if (pObj<>nil) then
      [*] = ;   begin
      [*] = ;      var pObjProp : Pointer = _unit_GetObjProp(trHnd);
      [*] = ;      var pObjInside : Pointer = _misc_GetObjectArgData(trHnd, gc_argunit_inside);
      [*] = ;      var pickCount : Integer = TObjProp(pObjProp).transport + TObj(pObj).individual.addtransport;
      [*] = ;      pickCount := pickCount - TObj(pObj).insideReserved - TIntegerList(pObjInside).GetCount;
      [*] = ;
      [*] = ;      if (pickCount > 0) and TObj(pObj).bRally then
      [*] = ;      begin
      [*] = ;         var plInd : Integer = TObj(pObj).pl;
      [*] = ;         var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;         var island : Integer = _misc_GetIslandInRadius(TObj(pObj).rallyX, TObj(pObj).rallyY, gc_WaterBorderSize);
      [*] = ;         if island >= 0 then
      [*] = ;         begin
      [*] = ;            var i : Integer;
      [*] = ;            for i := gPlayer[plInd].aiData.armyList.GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var pArmy : Pointer = gPlayer[plInd].aiData.armyList.Get(i);
      [*] = ;               if (pArmy<>nil) then
      [*] = ;               begin
      [*] = ;                  var aType : Integer = _ai_GetArmyType(pArmy);
      [*] = ;                  if (not _ai_IsWaterArmy(pArmy)) and (aType <> gc_ai_armytype_none) and (aType <> gc_ai_armytype_peasant)
      [*] = ;                  and (_misc_GetIsland(TArmy(pArmy).fCurX, TArmy(pArmy).fCurZ) = island) then
      [*] = ;                  begin
      [*] = ;                     if pResArmy = nil then
      [*] = ;                     pResArmy := pArmy else
      [*] = ;                     begin
      [*] = ;                        var resCount : Integer = _ai_GetArmyUnitsCount(pResArmy);
      [*] = ;                        var count : Integer = _ai_GetArmyUnitsCount(pArmy);
      [*] = ;                        if resCount > pickCount then
      [*] = ;                        begin
      [*] = ;                           if (count > pickCount) and (count < resCount) then
      [*] = ;                           pResArmy := pArmy;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           if count > pickCount then
      [*] = ;                           pResArmy := pArmy;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if pResArmy <> nil then
      [*] = ;         begin
      [*] = ;            gIntegerList.Clear;
      [*] = ;            var i, j : Integer;
      [*] = ;            var sqCount : Integer = TArmy(pResArmy).fSquadList.GetCount;
      [*] = ;            var count : Integer = 0;
      [*] = ;            for i := sqCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var pSquad : Pointer = TArmy(pResArmy).fSquadList.GetPointer(i);
      [*] = ;               if (pSquad<>nil) then
      [*] = ;               begin
      [*] = ;                  for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     var goHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;                     if (goHnd<>0) then
      [*] = ;                     begin
      [*] = ;                        if count >= pickCount then
      [*] = ;                        begin
      [*] = ;                           var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                           if (pobj<>nil) then
      [*] = ;                           begin
      [*] = ;                              if (not gObjProp[TObj(pObj).cid][TObj(pObj).id].bOfficer) and (not gObjProp[TObj(pObj).cid][TObj(pObj).id].bDrummer) and (gObjProp[TObj(pObj).cid][TObj(pObj).id].usage<>gc_obj_usage_peasant) then
      [*] = ;                              gPlayer[plInd].aiData.agressors.Add(goHnd)
      [*] = ;                              else
      [*] = ;                              if (gObjProp[TObj(pObj).cid][TObj(pObj).id].usage=gc_obj_usage_peasant) then
      [*] = ;                              ErrorLog('_ai_TransportPickArmy : tried to take peasant');
      [*] = ;                           end;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           count := count+1;
      [*] = ;                           gIntegerList.Add(goHnd);
      [*] = ;                        end;
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     ErrorLog('_ai_TransportPickArmy : goHnd=0');
      [*] = ;                  end;
      [*] = ;                  _misc_DisbandSquad(plHnd, TSquad(pSquad).fIndex, true);
      [*] = ;               end
      [*] = ;               else
      [*] = ;               ErrorLog('_ai_TransportPickArmy : psqaud=nil');
      [*] = ;               if count >= pickCount then
      [*] = ;               break;
      [*] = ;            end;
      [*] = ;            gPtrList.Clear;
      [*] = ;            _player_OrderUnitsToGoInside(plInd, gIntegerList, gPtrList, trHnd, true, true);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   Result := pResArmy;
      [*] = ;end;
      [*] = ;
      [*] = ;// _misc_GetNearestPort
      [*] = ;//
      [*] = ;function _misc_GetNearestPort(plInd : Integer; posX, posZ : Float) : Integer;
      [*] = ;begin
      [*] = ;   Result := 0;
      [*] = ;   if IsInRangeInt(plInd, 0, gc_MaxPlayerCount-1) then
      [*] = ;   begin
      [*] = ;      var i : Integer;
      [*] = ;      var minDist : Float = gc_MaxFloat;
      [*] = ;      for i := gPlayer[plInd].lists.ports.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var hnd : Integer = gPlayer[plInd].lists.ports.Get(i);
      [*] = ;         var x : Float = GetGameObjectPositionXByHandle(hnd);
      [*] = ;         var z : Float = GetGameObjectPositionZByHandle(hnd);
      [*] = ;         var dist : Float = VectorDistance(posX, 0, posZ, x, 0, z);
      [*] = ;         if dist < minDist then
      [*] = ;         begin
      [*] = ;            minDist := dist;
      [*] = ;            Result := hnd;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_IsWaterBattleArmy
      [*] = ;//
      [*] = ;function _ai_IsWaterBattleArmy(pArmy : Pointer) : Boolean;
      [*] = ;begin
      [*] = ;   Result := false;
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      if TArmy(pArmy).fSquadList.GetCount > 0 then
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(0);
      [*] = ;         if pSquad<>nil then
      [*] = ;         begin
      [*] = ;            var goHnd : Integer = _unit_GetSquadUnit(pSquad);
      [*] = ;            if (goHnd<>0) then
      [*] = ;            begin
      [*] = ;               var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;               if (pObjProp<>nil) then
      [*] = ;               Result := ((TObjProp(pObjProp).media = gc_obj_media_water) and (TObjProp(pObjProp).transport = 0));
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyAlign
      [*] = ;//
      [*] = ;procedure _ai_ArmyAlign(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      if IsInRangeInt(plInd, 0, gc_MaxPlayerCount-1) and (pArmy <> nil) then
      [*] = ;      begin
      [*] = ;         var i, j : Integer;
      [*] = ;         var cx, cz, cDirX, cDirZ : Float;
      [*] = ;         _ai_GetArmyCentralPosAndDir(pArmy, cx, cz, cDirX, cDirZ);
      [*] = ;
      [*] = ;         var newZone : Integer = TopologyGetZoneIndex(cx, cz);
      [*] = ;         if newZone <> TArmy(pArmy).fTopZone then
      [*] = ;         begin
      [*] = ;            var topdist : Integer = -1;
      [*] = ;            var attackCount, unitCount : Integer;
      [*] = ;            if (TArmy(pArmy).fTopZone>=0) then
      [*] = ;            begin
      [*] = ;               var dist : Float = VectorDistance(TArmy(pArmy).fX, 0, TArmy(pArmy).fZ, cx, 0, cz);
      [*] = ;               attackCount := _ai_GetArmyAttackCount(pArmy);
      [*] = ;               unitCount := _ai_GetArmyUnitsCount(pArmy);
      [*] = ;               topdist := TopologyGetZonesGraphDist(newZone, TArmy(pArmy).fTopZone, 4);
      [*] = ;            end;
      [*] = ;            if (topdist>=0) and ((topdist < 3 {2}) or (attackCount = unitCount)) then
      [*] = ;            begin
      [*] = ;               if newZone >= 0 then
      [*] = ;               TArmy(pArmy).fTopZone := newZone
      [*] = ;               else
      [*] = ;               TArmy(pArmy).fTopZone := TArmy(pArmy).fCurTopZone;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            _ai_SetArmyPosition(pArmy, cx, cz, cDirX, cDirZ, gc_obj_order_move_mode_default);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_GetDistanceToArmy
      [*] = ;//
      [*] = ;function _ai_GetDistanceToArmy(x, y : Float; region : Integer; pArmyInfo : Pointer) : Float;
      [*] = ;begin
      [*] = ;   // result is scan grid distance
      [*] = ;   Result := gc_MaxFloat;
      [*] = ;
      [*] = ;   if (region >= 0) and (TArmyInfo(pArmyInfo).region = region) or (TArmyInfo(pArmyInfo).regions.IndexOf(region) >= 0) then
      [*] = ;   begin
      [*] = ;      var xVal, yVal : Integer;
      [*] = ;      var scanX, scanY : Float;
      [*] = ;      _misc_PosToScanGridPos(x, y, scanX, scanY);
      [*] = ;
      [*] = ;      if (scanX <= TArmyInfo(pArmyInfo).minX) then
      [*] = ;      xVal := -1
      [*] = ;      else
      [*] = ;      if (scanX < TArmyInfo(pArmyInfo).maxX) then
      [*] = ;      xVal := 0
      [*] = ;      else
      [*] = ;      xVal := 1;
      [*] = ;
      [*] = ;      if (scanY <= TArmyInfo(pArmyInfo).minY) then
      [*] = ;      yVal := -1
      [*] = ;      else
      [*] = ;      if (scanY < TArmyInfo(pArmyInfo).maxY) then
      [*] = ;      yVal := 0
      [*] = ;      else
      [*] = ;      yVal := 1;
      [*] = ;
      [*] = ;      case xVal of
      [*] = ;         -1: begin
      [*] = ;            case yVal of
      [*] = ;               -1: Result := VectorDistance(scanX, 0, scanY, TArmyInfo(pArmyInfo).minX, 0, TArmyInfo(pArmyInfo).minY);
      [*] = ;               0:  Result := TArmyInfo(pArmyInfo).minX-scanX;
      [*] = ;               1:  Result := VectorDistance(scanX, 0, scanY, TArmyInfo(pArmyInfo).minX, 0, TArmyInfo(pArmyInfo).maxY);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         0: begin
      [*] = ;            case yVal of
      [*] = ;               -1: Result := TArmyInfo(pArmyInfo).minY-scanY;
      [*] = ;               0:  Result := 0;
      [*] = ;               1:  Result := scanY-TArmyInfo(pArmyInfo).maxY;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         1: begin
      [*] = ;            case yVal of
      [*] = ;               -1: Result := VectorDistance(scanX, 0, scanY, TArmyInfo(pArmyInfo).maxX, 0, TArmyInfo(pArmyInfo).minY);
      [*] = ;               0:  Result := scanX-TArmyInfo(pArmyInfo).maxX;
      [*] = ;               1:  Result := VectorDistance(scanX, 0, scanY, TArmyInfo(pArmyInfo).maxX, 0, TArmyInfo(pArmyInfo).maxY);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_CheckArmyBitva
      [*] = ;//
      [*] = ;function _ai_CheckArmyBitva(pArmy : Pointer; checkArt : Boolean; var enX, enZ : Float) : Boolean;
      [*] = ;begin
      [*] = ;   Result := false;
      [*] = ;   enX := 0;
      [*] = ;   enZ := 0;
      [*] = ;
      [*] = ;   if (pArmy<>nil) and (_ai_ArmyCanAttack(pArmy)) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var pEnInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;      if pEnInfo <> nil then
      [*] = ;      begin
      [*] = ;         var i, j : Integer;
      [*] = ;         var artOnly : Boolean = true;
      [*] = ;         var isArt : Boolean;
      [*] = ;         for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;            if (pSquad<>nil) and _ai_SquadIsArtillery(pSquad) then
      [*] = ;            isArt := true
      [*] = ;            else
      [*] = ;            artOnly := false;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if not artOnly then
      [*] = ;         begin
      [*] = ;            var batDist : Float;
      [*] = ;
      [*] = ;            if TArmy(pArmy).fSpec = gc_ai_armyspec_grenadier then
      [*] = ;            batDist := gc_ai_GreArmyBattleDist
      [*] = ;            else
      [*] = ;            batDist := gc_ai_ArmyBattleDist;
      [*] = ;
      [*] = ;            var enInd : Integer = -1;
      [*] = ;            var minDist : Float = gc_MaxFloat;
      [*] = ;            var aRegion : Integer = TArmy(pArmy).fRegion;
      [*] = ;
      [*] = ;            for i := TEnemyInfo(pEnInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var pArmyInfo : Pointer = TEnemyInfo(pEnInfo).armyInfos.Get(i);
      [*] = ;               if pArmyInfo<>nil then
      [*] = ;               if isArt or (TArmyInfo(pArmyInfo).infantry > 0) or (TArmyInfo(pArmyInfo).shooters > 0) or (TArmyInfo(pArmyInfo).cavalry > 0)
      [*] = ;               or (checkArt and ((TArmyInfo(pArmyInfo).cannons > 0) or (TArmyInfo(pArmyInfo).mortars > 0) or (TArmyInfo(pArmyInfo).mcannons > 0))) then
      [*] = ;               begin
      [*] = ;                  for j := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(j);
      [*] = ;                     var x : Float = TSquad(pSquad).fCurX;
      [*] = ;                     var z : Float = TSquad(pSquad).fCurZ;
      [*] = ;                     var dist : Float = _ai_GetDistanceToArmy(x, z, aRegion, pArmyInfo);
      [*] = ;                     if (dist >= 0) and (dist < minDist) then
      [*] = ;                     begin
      [*] = ;                        minDist := dist;
      [*] = ;                        enInd := i;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            if (minDist < batDist) and (enInd >= 0) then
      [*] = ;            begin
      [*] = ;               var pArmyInfo : Pointer = TEnemyInfo(pEnInfo).armyInfos.Get(enInd);
      [*] = ;               if pArmyInfo<>nil then
      [*] = ;               begin
      [*] = ;                  _misc_GetArmyInfoPosition(pArmyInfo, enX, enZ);
      [*] = ;                  Result := true;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_CheckArmyWaterBitva
      [*] = ;//
      [*] = ;function _ai_CheckArmyWaterBitva(pArmy : Pointer; var enX, enZ : Float) : Boolean;
      [*] = ;begin
      [*] = ;   Result := false;
      [*] = ;
      [*] = ;   if (pArmy<>nil) and _ai_ArmyCanAttack(pArmy) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var pEnInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;      if pEnInfo <> nil then
      [*] = ;      begin
      [*] = ;         var i, j : Integer;
      [*] = ;         var batDist : Float;
      [*] = ;
      [*] = ;         var enInd : Integer = -1;
      [*] = ;         var minDist : Float = gc_MaxFloat;
      [*] = ;         var aArea : Integer = TArmy(pArmy).fRegion;
      [*] = ;
      [*] = ;         for i := TEnemyInfo(pEnInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pArmyInfo : Pointer = TEnemyInfo(pEnInfo).armyInfos.Get(i);
      [*] = ;            if (pArmyInfo<>nil) and (TArmyInfo(pArmyInfo).waterCount > 0) then
      [*] = ;            begin
      [*] = ;               for j := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(j);
      [*] = ;                  if pSquad<>nil then
      [*] = ;                  begin
      [*] = ;                     var x : Float = TSquad(pSquad).fCurX;
      [*] = ;                     var z : Float = TSquad(pSquad).fCurZ;
      [*] = ;                     var goHnd : Integer = _unit_GetSquadUnit(pSquad);
      [*] = ;                     if (goHnd<>0) then
      [*] = ;                     begin
      [*] = ;                        var maxRad : Float = _unit_GetMaxAttackRadius(goHnd);
      [*] = ;
      [*] = ;                        var dist : Float = _ai_GetDistanceToArmy(x, z, aArea, pArmyInfo);
      [*] = ;                        if dist < gc_MaxFloat then
      [*] = ;                        begin
      [*] = ;                           dist := dist * gc_scangrid_size;
      [*] = ;                           if (dist < maxRad) and (dist < minDist) then
      [*] = ;                           begin
      [*] = ;                              minDist := dist;
      [*] = ;                              enInd := i;
      [*] = ;                              Result := true;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     ErrorLog('ProgressWarAI _unit_GetSquadUnit = 0');
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if enInd >= 0 then
      [*] = ;         begin
      [*] = ;            var pArmyInfo : Pointer = TEnemyInfo(pEnInfo).armyInfos.Get(enInd);
      [*] = ;            var enGridX : Float = (TArmyInfo(pArmyInfo).minX + TArmyInfo(pArmyInfo).maxX) / 2;
      [*] = ;            var enGridY : Float = (TArmyInfo(pArmyInfo).minY + TArmyInfo(pArmyInfo).maxY) / 2;
      [*] = ;            _misc_ScanGridIndicesToPos(enGridX, enGridY, enX, enZ);
      [*] = ;            Result := true;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_CheckMinArmyCreationAbility
      [*] = ;//
      [*] = ;function _ai_CheckMinArmyCreationAbility(plInd : Integer; var list : TIntegerList; checkMin, checkEssential, checkAvail : Boolean) : Integer;
      [*] = ;begin
      [*] = ;   var count : Integer = list.GetCount;
      [*] = ;   Result := gc_ai_armytype_none;
      [*] = ;
      [*] = ;   var arCount : array [0..gc_ai_maxarmytypes-1] of Integer;
      [*] = ;   var i : Integer;
      [*] = ;   for i := list.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var goHnd : Integer = list.Get(i);
      [*] = ;      var tag : Integer = _unit_GetTagStateByType(goHnd, gc_statetag_essential);
      [*] = ;      if (not checkEssential) or (tag = gc_statetag_essential_none) then
      [*] = ;      begin
      [*] = ;         var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;         var aType : Integer = _ai_GetUnitArmyType(goHnd);
      [*] = ;         var id : Integer = TObj(pObj).id;
      [*] = ;         var cid : Integer = TObj(pObj).cid;
      [*] = ;
      [*] = ;         if (aType <> gc_ai_armytype_none) and ((not checkAvail) or gPlayer[plInd].aiData.avUnits[cid][id]) then
      [*] = ;         arCount[aType] := arCount[aType] + 1;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   for i := 0 to gc_ai_maxarmytypes-1 do
      [*] = ;   begin
      [*] = ;      var minCount : Integer = 1;
      [*] = ;      if checkMin then
      [*] = ;      begin
      [*] = ;         if gPlayer[plInd].aiData.defenceStage then
      [*] = ;         minCount := gAIConst.defArmyMinCount[i]
      [*] = ;         else
      [*] = ;         minCount := gAIConst.armyMinCount[i];
      [*] = ;      end;
      [*] = ;
      [*] = ;      if arCount[i] >= minCount then
      [*] = ;      begin
      [*] = ;         Result := i;
      [*] = ;         break;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_CreateMinimalArmy
      [*] = ;//
      [*] = ;function _ai_CreateMinimalArmy(plInd, armyType, maxUnits : Integer; var list : TIntegerList; checkEssential, checkAvail : Boolean) : Pointer;
      [*] = ;begin
      [*] = ;   var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;
      [*] = ;   var i : Integer;
      [*] = ;   var pArmy : Pointer = nil;
      [*] = ;   var maxCount : Integer = maxUnits;
      [*] = ;   if maxUnits = 0 then
      [*] = ;   maxCount := gAIConst.armyMaxCount[armyType];
      [*] = ;
      [*] = ;   procedure AddSquad;
      [*] = ;   begin
      [*] = ;      var pSquad : Pointer = _player_WriteSquadNew(plHnd, -1, 0, 0, gPlayer[plInd].aiData.aiList, false, false);
      [*] = ;      if (pSquad<>nil) then
      [*] = ;      begin
      [*] = ;         TArmy(pArmy).Add(pSquad);
      [*] = ;         TSquad(pSquad).fType := armyType;
      [*] = ;      end;
      [*] = ;
      [*] = ;      gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;   end;
      [*] = ;
      [*] = ;   gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;
      [*] = ;   for i := list.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var goHnd : Integer = list.Get(i);
      [*] = ;      if (not checkEssential) or ((GetGameObjectStatesTagByHandle(goHnd) and gc_statetag_essential_none) <> 0) then
      [*] = ;      begin
      [*] = ;         var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;         var id : Integer = TObj(pObj).id;
      [*] = ;         var cid : Integer = TObj(pObj).cid;
      [*] = ;
      [*] = ;         if (_ai_GetUnitArmyType(goHnd) = armyType) and ((not checkAvail) or gPlayer[plInd].aiData.avUnits[cid][id]) then
      [*] = ;         begin
      [*] = ;            if pArmy = nil then
      [*] = ;            begin
      [*] = ;               pArmy := gPlayer[plInd].aiData.armyList.AddSetup(plInd);
      [*] = ;               if pArmy<>nil then
      [*] = ;               TArmy(pArmy).fSpec := _ai_GetUnitArmySpec(goHnd);
      [*] = ;            end;
      [*] = ;
      [*] = ;            gPlayer[plInd].aiData.aiList.Add(goHnd);
      [*] = ;            list.Delete(i);
      [*] = ;
      [*] = ;            var aiCount : Integer = gPlayer[plInd].aiData.aiList.GetCount;
      [*] = ;            if (pArmy<>nil) and (aiCount > 0) and (aiCount >= maxCount) then
      [*] = ;            begin
      [*] = ;               AddSquad;
      [*] = ;               break;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      if gPlayer[plInd].aiData.aiList.GetCount > 0 then
      [*] = ;      AddSquad;
      [*] = ;
      [*] = ;      var sqCount : Integer = TArmy(pArmy).fSquadList.GetCount;
      [*] = ;      if sqCount = 0 then
      [*] = ;      gPlayer[plInd].aiData.armyList.Remove(pArmy)
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;            if (pSquad<>nil) then
      [*] = ;            _player_SetSquadFormation(plHnd, TSquad(pSquad).fIndex, TSquad(pSquad).fFormation, false, false);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   Result := pArmy;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyAddSquad
      [*] = ;//
      [*] = ;procedure _ai_ArmyAddSquad(pArmy, pSquad : Pointer);
      [*] = ;begin
      [*] = ;   if (pArmy <> nil) and (pSquad <> nil) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;
      [*] = ;      // remove from old army
      [*] = ;      if TSquad(pSquad).fArmy >= 0 then
      [*] = ;      begin
      [*] = ;         var pOldArmy : Pointer = gPlayer[plInd].aiData.armyList.Get(TSquad(pSquad).fArmy);
      [*] = ;         if (pOldArmy<>nil) then
      [*] = ;         TArmy(pOldArmy).Remove(pSquad);
      [*] = ;      end;
      [*] = ;
      [*] = ;      if TSquad(pSquad).fWarType <> 0 then
      [*] = ;      begin
      [*] = ;         TArmy(pArmy).Add(pSquad);
      [*] = ;         TSquad(pSquad).fType := 0;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if TSquad(pSquad).fFormation >= 0 then
      [*] = ;      begin
      [*] = ;         TArmy(pArmy).Add(pSquad);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         var sqInd : array [0..gc_ai_maxarmytypes-1] of Integer;
      [*] = ;         var i, j : Integer;
      [*] = ;         for i := 0 to gc_ai_maxarmytypes-1 do
      [*] = ;         sqInd[i] := -1;
      [*] = ;
      [*] = ;         gTmpIntListFindUnits.Clear;
      [*] = ;         gTmpIntListFillSquad.Clear;
      [*] = ;         gTmpIntList.Clear;
      [*] = ;         var goCount : Integer = TSquad(pSquad).GetCount;
      [*] = ;         for i := goCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var goHnd : Integer = TSquad(pSquad).Get(i);
      [*] = ;            if (goHnd<>0) then
      [*] = ;            begin
      [*] = ;               var aType : Integer = _ai_GetUnitArmyType(goHnd);
      [*] = ;               if aType <> gc_ai_armytype_none then
      [*] = ;               begin
      [*] = ;                  var newSquad : Boolean;
      [*] = ;                  if sqInd[aType] >= 0 then
      [*] = ;                  begin
      [*] = ;                     var pSquad1 : Pointer = TArmy(pArmy).fSquadList.GetPointer(sqInd[aType]);
      [*] = ;                     if (pSquad1<>nil) and (TSquad(pSquad1).GetCount >= gAIConst.armyMaxCount[aType]) then
      [*] = ;                     begin
      [*] = ;                        _squad_SetCustomFormation(pSquad1, true);
      [*] = ;                        sqInd[aType] := -1;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if sqInd[aType] < 0 then
      [*] = ;                  begin
      [*] = ;                     var aSquadCount : Integer = TArmy(pArmy).fSquadList.GetCount;
      [*] = ;                     for j := 0 to aSquadCount-1 do
      [*] = ;                     begin
      [*] = ;                        var pSquad1 : Pointer = TArmy(pArmy).fSquadList.GetPointer(j);
      [*] = ;
      [*] = ;                        if (pSquad1<>nil) and (TSquad(pSquad1).fType = aType) and (TSquad(pSquad1).GetCount < gAIConst.armyMaxCount[aType]) and
      [*] = ;                        (TSquad(pSquad1).fWarType = 0) and (TSquad(pSquad1).fFormation < 0) then
      [*] = ;                        begin
      [*] = ;                           sqInd[aType] := j;
      [*] = ;                           break;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if sqInd[aType] < 0 then
      [*] = ;                  begin
      [*] = ;                     gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;                     var pNewSquad : Pointer = _player_WriteSquadNew(plHnd, -1, 0, 0, gPlayer[plInd].aiData.aiList, false, false);
      [*] = ;                     if pNewSquad <> nil then
      [*] = ;                     begin
      [*] = ;                        TArmy(pArmy).Add(pNewSquad);
      [*] = ;                        TSquad(pNewSquad).fType := aType;
      [*] = ;
      [*] = ;                        sqInd[aType] := TArmy(pArmy).fSquadList.GetCount-1;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if sqInd[aType] >= 0 then
      [*] = ;                  begin
      [*] = ;                     var pSquad1 : Pointer = TArmy(pArmy).fSquadList.GetPointer(sqInd[aType]);
      [*] = ;                     if pSquad1<>nil then
      [*] = ;                     begin
      [*] = ;                        TSquad(pSquad).Delete(i);
      [*] = ;                        _squad_AddUnit(pSquad1, goHnd, false);
      [*] = ;                        if (_net_IsOffline) or (_net_IsServer) then
      [*] = ;                        begin
      [*] = ;                           gTmpIntListFillSquad.Add(sqInd[aType]);
      [*] = ;                           gTmpIntList.Add(goHnd);
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            ErrorLog('_ai_ArmyAddSquad : goHnd=0');
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (gTmpIntListFillSquad.GetCount>0) then
      [*] = ;         begin
      [*] = ;            var prevsqind : Integer = -1;
      [*] = ;            _misc_SortIntegerLists2D(gTmpIntListFillSquad, gTmpIntList, True);
      [*] = ;            var count : Integer = gTmpIntListFillSquad.GetCount;
      [*] = ;            for i:=count-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var goHnd : Integer = gTmpIntList.Get(i);
      [*] = ;               var sqind : Integer = gTmpIntListFillSquad.Get(i);
      [*] = ;               if (prevsqind<0) then
      [*] = ;               prevsqind := sqind;
      [*] = ;
      [*] = ;               if (sqind=prevsqind) then
      [*] = ;               gTmpIntListFindUnits.Add(goHnd);
      [*] = ;
      [*] = ;               if (sqind<>prevsqind) or (i=0) then
      [*] = ;               begin
      [*] = ;                  var psquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(prevsqind);
      [*] = ;                  if (psquad<>nil) then
      [*] = ;                  _player_AddUnitsToSquad(plhnd, psquad, False, gTmpIntListFindUnits);
      [*] = ;                  gTmpIntListFindUnits.Clear;
      [*] = ;
      [*] = ;                  if (i=0) and (sqind<>prevsqind) then
      [*] = ;                  begin
      [*] = ;                     gTmpIntListFindUnits.Add(goHnd);
      [*] = ;                     psquad := TArmy(pArmy).fSquadList.GetPointer(sqind);
      [*] = ;                     if (psquad<>nil) then
      [*] = ;                     _player_AddUnitsToSquad(plhnd, psquad, False, gTmpIntListFindUnits);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               prevsqind := sqind;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         for i := 0 to gc_ai_maxarmytypes-1 do
      [*] = ;         if sqInd[i] >= 0 then
      [*] = ;         begin
      [*] = ;            var pSquad1 : Pointer = TArmy(pArmy).fSquadList.GetPointer(sqInd[i]);
      [*] = ;            if pSquad1<>nil then
      [*] = ;            _squad_SetCustomFormation(pSquad1, true);
      [*] = ;         end;
      [*] = ;
      [*] = ;         if TSquad(pSquad).GetCount = 0 then
      [*] = ;         begin
      [*] = ;            var sqInd : Integer = TSquad(pSquad).fIndex;
      [*] = ;            _misc_DisbandSquad(plHnd, sqInd, true);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            for i := TSquad(pSquad).fWidth-1 downto 0 do
      [*] = ;            for j := TSquad(pSquad).fHeight-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var goHnd : Integer = TSquad(pSquad).arGrid[i, j];
      [*] = ;               var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;               if pObj <> nil then
      [*] = ;               begin
      [*] = ;                  if TObj(pObj).squad <> TSquad(pSquad).fIndex then
      [*] = ;                  TSquad(pSquad).arGrid[i, j] := 0;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyMakeBattle
      [*] = ;//
      [*] = ;procedure _ai_ArmyMakeBattle(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      if _ai_IsWaterArmy(pArmy) then
      [*] = ;      TArmy(pArmy).fOrder.iType := gc_ai_armyorder_makewaterbattle
      [*] = ;      else
      [*] = ;      TArmy(pArmy).fOrder.iType := gc_ai_armyorder_makebattle;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_CreateWaterArmies
      [*] = ;//
      [*] = ;procedure _ai_CreateWaterArmies(plInd : Integer);
      [*] = ;begin
      [*] = ;   var i : Integer;
      [*] = ;   for i := 0 to gPlayer[plInd].squads.GetCount-1 do
      [*] = ;   begin
      [*] = ;      var pSquad : Pointer = gPlayer[plInd].squads.Get(i);
      [*] = ;      var goHnd : Integer = _unit_GetSquadUnit(pSquad);
      [*] = ;      if (pSquad<>nil) and (_squad_IsWaterSquad(pSquad)) and (TSquad(pSquad).fArmy < 0)
      [*] = ;      and (goHnd <> 0) and (_unit_GetUsage(goHnd) <> gc_obj_usage_fisher) then
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = _unit_GetSquadUnit(pSquad);
      [*] = ;         if (goHnd<>0) then
      [*] = ;         begin
      [*] = ;            var pArmy : Pointer = gPlayer[plInd].aiData.armyList.AddSetup(plInd);
      [*] = ;            if (pArmy<>nil) then
      [*] = ;            begin
      [*] = ;               TArmy(pArmy).Add(pSquad);
      [*] = ;               TArmy(pArmy).fSpec := _ai_GetUnitArmySpec(goHnd);
      [*] = ;               TSquad(pSquad).fType := _ai_GetUnitArmyType(goHnd);
      [*] = ;               _ai_ArmyAlign(pArmy);
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         ErrorLog('ProgressWarAI _ai_CreateWaterArmies _unit_GetSquadUnit = 0');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_GetNearestEnemyTower
      [*] = ;//
      [*] = ;function _ai_GetNearestEnemyTower(plInd : Integer; x, z : Float) : Integer;
      [*] = ;begin
      [*] = ;   var enHnd : Integer;
      [*] = ;   var enDist : Float = gc_MaxInt;
      [*] = ;
      [*] = ;   var pEnInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;   if pEnInfo <> nil then
      [*] = ;   begin
      [*] = ;      var i : Integer;
      [*] = ;      for i := TEnemyInfo(pEnInfo).towers.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pTower : Pointer = TEnemyInfo(pEnInfo).towers.Get(i);
      [*] = ;         if (pTower<>nil) then
      [*] = ;         begin
      [*] = ;            var tHnd : Integer = TTower(pTower).hnd;
      [*] = ;            var tX : Float = GetGameObjectPositionXByHandle(tHnd);
      [*] = ;            var tZ : Float = GetGameObjectPositionZByHandle(tHnd);
      [*] = ;            var dist : Float = Sqrt(Sqr(tX-x) + Sqr(tZ-z));
      [*] = ;            if dist < enDist then
      [*] = ;            begin
      [*] = ;               enHnd := tHnd;
      [*] = ;               enDist := dist;
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         ErrorLog('_ai_GetNearestEnemyTower : pTower=nil');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   Result := enHnd;
      [*] = ;end;
      [*] = ;
      [*] = ;{// _ai_ArmyBitvaLink
      [*] = ;//
      [*] = ;procedure _ai_ArmyBitvaLink(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;   var arch : Integer = _ai_ArmyHandleArchers(pArmy, 1000000);
      [*] = ;   var i, j : Integer;
      [*] = ;   var inBattle : Boolean;
      [*] = ;   var tooFar : Boolean = true;
      [*] = ;   gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;   gIntegerList.Clear;
      [*] = ;
      [*] = ;   if arch <> 0 then
      [*] = ;   gIntegerList.Add(arch);
      [*] = ;
      [*] = ;   for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;      for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;         var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;         var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;         if (not TObjProp(pObjProp).bArtillery) and (not TObjProp(pObjProp).bPriest) and (TObjProp(pObjProp).usage <> gc_obj_usage_archer) then
      [*] = ;         begin
      [*] = ;            var trgHnd : Integer = GetGameObjectSTOHandleByHandle(goHnd);
      [*] = ;
      [*] = ;            if trgHnd = 0 then
      [*] = ;            begin
      [*] = ;               TObj(pObj).bSearchEnemy := true;
      [*] = ;               trgHnd := _unit_DoSearchVictim(goHnd, true);
      [*] = ;            end;
      [*] = ;
      [*] = ;            if (trgHnd <> 0) and (TObj(pObj).orders[0].iType = gc_obj_order_type_attackobj) then
      [*] = ;            begin
      [*] = ;               inBattle := true;
      [*] = ;               gIntegerList.Add(trgHnd);
      [*] = ;               var goX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;               var goZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;               var trgX : Float = GetGameObjectPositionXByHandle(trgHnd);
      [*] = ;               var trgZ  : Float = GetGameObjectPositionZByHandle(trgHnd);
      [*] = ;               if VectorDistance(goX, 0, goZ, trgX, 0, trgZ) < TObjProp(pObjProp).searchRadius then
      [*] = ;               tooFar := false;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            gPlayer[plInd].aiData.aiList.Add(goHnd);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if tooFar then
      [*] = ;   inBattle := false;
      [*] = ;
      [*] = ;   if inBattle or (arch <> 0) then
      [*] = ;   begin
      [*] = ;      for i := gPlayer[plInd].aiData.aiList.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = gPlayer[plInd].aiData.aiList.Get(i);
      [*] = ;         var goX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;         var goZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;
      [*] = ;         var trgHnd : Integer = GetGameObjectSTOHandleByHandle(goHnd);
      [*] = ;         if trgHnd = 0 then
      [*] = ;         begin
      [*] = ;            var minDist : Float = gc_MaxInt;
      [*] = ;            var newHnd : Integer;
      [*] = ;            for j := gIntegerList.GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var hnd : Integer = gIntegerList.Get(j);
      [*] = ;               var x : Float = GetGameObjectPositionXByHandle(hnd);
      [*] = ;               var z : Float = GetGameObjectPositionZByHandle(hnd);
      [*] = ;               var dist : Float = VectorDistance(goX, 0, goZ, x, 0, z);
      [*] = ;               if dist < minDist then
      [*] = ;               begin
      [*] = ;                  newHnd := hnd;
      [*] = ;                  minDist := dist;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            if newHnd <> 0 then
      [*] = ;            _unit_OrderAttack(goHnd, newHnd, true, true, false);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   _ai_ArmyCancelOrders(plInd, pArmy);
      [*] = ;end;{}
      [*] = ;
      [*] = ;// _ai_ArmyBitvaLinkExt
      [*] = ;//
      [*] = ;procedure _ai_ArmyBitvaLinkExt(pArmy : Pointer; setPos : Boolean; x, z : Float);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var i, j : Integer;
      [*] = ;      var inBattle : Boolean;
      [*] = ;      var count : Integer;
      [*] = ;      var posX, posZ : Float;
      [*] = ;      var mode : Integer = -1;
      [*] = ;      var enHnd : Integer;
      [*] = ;
      [*] = ;      for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;         if pSquad<>nil then
      [*] = ;         for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var goHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;            var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;            if (pObj<>nil) then
      [*] = ;            begin
      [*] = ;               var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;               if (not TObjProp(pObjProp).bArtillery) and (not TObjProp(pObjProp).bPriest) and (TObjProp(pObjProp).usage <> gc_obj_usage_archer) and (TObjProp(pObjProp).usage <> gc_obj_usage_grenadier) then // (TObjProp(pObjProp).usage <> gc_obj_usage_grenadier) - experiment
      [*] = ;               begin
      [*] = ;                  var trgHnd : Integer = GetGameObjectSTOHandleByHandle(goHnd);
      [*] = ;
      [*] = ;                  if trgHnd = 0 then
      [*] = ;                  begin
      [*] = ;                     var curOrder : Integer = _unit_GetCurrentOrder(goHnd);
      [*] = ;                     if (curOrder >= 0) and (TObj(pObj).orders[curOrder].iType = gc_obj_order_type_attackobj) then
      [*] = ;                     trgHnd := TObj(pObj).orders[curOrder].info.trg;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if trgHnd = 0 then
      [*] = ;                  begin
      [*] = ;                     TObj(pObj).bSearchEnemy := true;
      [*] = ;                     trgHnd := _unit_DoSearchVictim(goHnd, false);
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if trgHnd <> 0 then
      [*] = ;                  begin
      [*] = ;                     // if target is cannon, then unit don't get order to attack it
      [*] = ;                     var kill : Boolean = (TObj(pObj).orders[0].iType = gc_obj_order_type_attackobj);
      [*] = ;                     if kill then
      [*] = ;                     mode := 0
      [*] = ;                     else
      [*] = ;                     begin
      [*] = ;                        var pTrgObjProp : Pointer = _unit_GetObjProp(trgHnd);
      [*] = ;                        if (pTrgObjProp<>nil) and TObjProp(pTrgObjProp).bCapture then // sometimes trgHnd is not unit. may be resource
      [*] = ;                        kill := true;
      [*] = ;
      [*] = ;                        if mode < 0 then
      [*] = ;                        begin
      [*] = ;                           enHnd := trgHnd;
      [*] = ;                           mode := 1;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;
      [*] = ;                     if kill then
      [*] = ;                     begin
      [*] = ;                        var trgX : Float = GetGameObjectPositionXByHandle(trgHnd);
      [*] = ;                        var trgZ : Float = GetGameObjectPositionZByHandle(trgHnd);
      [*] = ;                        posX := posX + trgX;
      [*] = ;                        posZ := posZ + trgZ;
      [*] = ;                        count := count+1;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if count > 0 then
      [*] = ;      begin
      [*] = ;         TArmy(pArmy).fOrder.tag := mode;
      [*] = ;
      [*] = ;         posX := posX / count;
      [*] = ;         posZ := posZ / count;
      [*] = ;
      [*] = ;         if mode = 0 then
      [*] = ;         begin
      [*] = ;            // attack enemy unit
      [*] = ;            _unit_SetArmyOrderTarget(pArmy, 0);
      [*] = ;            _ai_SetArmyPosition(pArmy, posX, posZ, 0, 0, gc_obj_order_move_mode_attack);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            // capture enemy unit
      [*] = ;            _unit_SetArmyOrderTarget(pArmy, enHnd);
      [*] = ;            _ai_SetArmyPosition(pArmy, posX, posZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         if setPos then
      [*] = ;         _ai_SetArmyPosition(pArmy, x, z, 0, 0, gc_obj_order_move_mode_attack)
      [*] = ;         else
      [*] = ;         _ai_ArmyCancelOrders(pArmy);
      [*] = ;      end;
      [*] = ;      // IMPORTANT CHANGE
      [*] = ;      var arch : Integer = _ai_ArmyHandleArchers(pArmy, 1000000);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyBitvaProgress
      [*] = ;//
      [*] = ;procedure _ai_ArmyBitvaProgress(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var i, j : Integer;
      [*] = ;      if (TArmy(pArmy).fOrder.tag = 1) and (TArmy(pArmy).fOrder.targetUID <> 0) then
      [*] = ;      begin
      [*] = ;         var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;         var trgUID : Integer = TArmy(pArmy).fOrder.targetUID;
      [*] = ;         var trgHnd : Integer = GetGameObjectHandleByUniqueId(trgUID);
      [*] = ;         if (trgHnd<>0) then
      [*] = ;         begin
      [*] = ;            var pTrgObj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;            if (pTrgObj <> nil) and ((gPlayer[plInd].enemyPlMask and gPlayer[TObj(pTrgObj).pl].myplmask) = 0) then
      [*] = ;            _unit_SetArmyOrderTarget(pArmy, 0);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         _unit_SetArmyOrderTarget(pArmy, 0);
      [*] = ;      end;
      [*] = ;
      [*] = ;      if GetGameTime - TArmy(pArmy).fLastBattleTime > gc_ai_BitvaInterval then
      [*] = ;      begin
      [*] = ;         var x, z : Float;
      [*] = ;         var cancel : Boolean;
      [*] = ;
      [*] = ;         if _ai_IsWaterArmy(pArmy) then
      [*] = ;         cancel := not _ai_CheckArmyWaterBitva(pArmy, x, z)
      [*] = ;         else
      [*] = ;         cancel := not _ai_CheckArmyBitva(pArmy, false, x, z);
      [*] = ;
      [*] = ;         if cancel then
      [*] = ;         _ai_ArmyCancelOrders(pArmy);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyBitva
      [*] = ;//
      [*] = ;procedure _ai_ArmyBitva(pArmy : Pointer; posX, posZ : Float);
      [*] = ;begin
      [*] = ;   // TODO: maybe there's necessary to check last battle time
      [*] = ;   //if(tmtmt-LastBitvaTime<64||(AOrder&&AOrder->ALink==&A_BitvaLink))return;
      [*] = ;
      [*] = ;   if (pArmy<>nil) and (not TArmy(pArmy).fSpecialOrder) and (TArmy(pArmy).fOrder.iType <> gc_ai_armyorder_bitva) then
      [*] = ;   begin
      [*] = ;      var i, j : Integer;
      [*] = ;      var bitva : Boolean;
      [*] = ;      for [MAIN]i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;         if pSquad<>nil then
      [*] = ;         for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var goHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;            var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;            if (pObj<>nil) and (not TObj(pObj).bDead) then
      [*] = ;            begin
      [*] = ;               bitva := true;
      [*] = ;               break(MAIN);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if bitva then
      [*] = ;      begin
      [*] = ;         for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;            if pSquad<>nil then
      [*] = ;            for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var goHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;               var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;               if (pObj<>nil) then
      [*] = ;               TObj(pObj).bsearchenemy := True;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         TArmy(pArmy).fOrder.iType := gc_ai_armyorder_bitva;
      [*] = ;         TArmy(pArmy).fOrder.tag := 0;
      [*] = ;         TArmy(pArmy).fLastBattleTime := GetGameTime;
      [*] = ;         _ai_ArmyBitvaLinkExt(pArmy, true, posX, posZ);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (TArmy(pArmy).fOrder.iType <> gc_ai_armyorder_sabotage) and (TArmy(pArmy).fOrder.iType <> gc_ai_armyorder_agressor) then
      [*] = ;      TArmy(pArmy).fOrder.iType := gc_ai_armyorder_none;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_GetNextZone
      [*] = ;//
      [*] = ;function _ai_GetNextZone(stZone, endZone : Integer; water, reduce : Boolean) : Integer;
      [*] = ;begin
      [*] = ;   Result := -1;
      [*] = ;   if stZone = endZone then
      [*] = ;   Result := stZone
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      if TopologyGetZonesPath(stZone, endZone) then
      [*] = ;      begin
      [*] = ;         var endX, endZ : Float;
      [*] = ;         if reduce then
      [*] = ;         begin
      [*] = ;            var tpCount : Integer = TopologyGetZoneTrackPointsCountByIndex(endZone);
      [*] = ;            if tpCount > 1 then
      [*] = ;            begin
      [*] = ;               TopologyGetZoneTrackPointCoordsByIndex(endZone, tpCount-2, endX, endZ);
      [*] = ;               Result := TopologyGetZoneIndex(endX, endZ);
      [*] = ;            end
      [*] = ;            else
      [*] = ;            Result := stZone;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            var dist : Float;
      [*] = ;            if water then
      [*] = ;            dist := gc_ai_WaterApproachDist
      [*] = ;            else
      [*] = ;            dist := gc_ai_ApproachDist;
      [*] = ;
      [*] = ;            if _misc_TopZonePathLerp(endZone, dist, true, endX, endZ) then
      [*] = ;            Result := TopologyGetZoneIndex(endX, endZ);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_GetNextGreCell
      [*] = ;//
      [*] = ;function _ai_GetNextGreCell(pArmy : Pointer) : Integer;
      [*] = ;begin
      [*] = ;   Result := -1;
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      var myZone : Integer = TArmy(pArmy).fTopZone;
      [*] = ;      if IsInRangeInt(myZone, 0, TopologyGetZonesCount-1) then
      [*] = ;      begin
      [*] = ;         var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;         var pEnemyInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;         if pEnemyInfo <> nil then
      [*] = ;         begin
      [*] = ;            var i, j : Integer;
      [*] = ;            var unitCount : Integer;
      [*] = ;            for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;               if pSquad<>nil then
      [*] = ;               unitCount := unitCount + TSquad(pSquad).GetCount;
      [*] = ;            end;
      [*] = ;
      [*] = ;            // find attractive zones
      [*] = ;            var enMask : Integer = TEnemyInfo(pEnemyInfo).enemyMask;
      [*] = ;            var plIndices : array [0..gc_MaxPlayerCount-1] of Integer;
      [*] = ;            var plCount : Integer;
      [*] = ;            var team : Integer = gPlayer[plInd].team;
      [*] = ;            for i := 0 to gc_MaxPlayerCount-1 do
      [*] = ;            if (i <> plInd) and gPlayer[i].bExists and (gPlayer[i].myPlMask and enMask <> 0) then
      [*] = ;            begin
      [*] = ;               plIndices[plCount] := i;
      [*] = ;               plCount := plCount+1;
      [*] = ;            end;
      [*] = ;
      [*] = ;            var pBuffer : Pointer = TopologyGetZoneBufferByIndex(myZone);
      [*] = ;            if (pBuffer<>nil) then
      [*] = ;            begin
      [*] = ;               for i := 0 to plCount-1 do
      [*] = ;               if TTopZone(pBuffer).attract[plIndices[i]] > 0 then
      [*] = ;               begin
      [*] = ;                  Result := myZone;
      [*] = ;                  exit;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            var found : Boolean;
      [*] = ;
      [*] = ;            for i := TopologyGetZonesCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var pBuffer : Pointer = TopologyGetZoneBufferByIndex(i);
      [*] = ;               if (pBuffer<>nil) then
      [*] = ;               begin
      [*] = ;                  var isAttr : Boolean;
      [*] = ;
      [*] = ;                  for j := 0 to plCount-1 do
      [*] = ;                  if TTopZone(pBuffer).attract[plIndices[j]] > 0 then
      [*] = ;                  begin
      [*] = ;                     isAttr := true;
      [*] = ;                     found := true;
      [*] = ;                     break;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  TopologySetZoneNeedPathByIndex(i, isAttr);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            // find zones with small count of enemy units
      [*] = ;            for i := TEnemyInfo(pEnemyInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var pArmyInfo : Pointer = TEnemyInfo(pEnemyInfo).armyInfos.Get(i);
      [*] = ;               if (pArmyInfo<>nil) and (TArmyInfo(pArmyInfo).landCount > 0) and (TArmyInfo(pArmyInfo).cannons + TArmyInfo(pArmyInfo).mortars + TArmyInfo(pArmyInfo).mcannons + TArmyInfo(pArmyInfo).towers = 0) and
      [*] = ;               (TArmyInfo(pArmyInfo).force > 30) and (TArmyInfo(pArmyInfo).force < unitCount * gc_ai_GrenadierTargetFactor) then
      [*] = ;               begin
      [*] = ;                  var minX : Integer = TArmyInfo(pArmyInfo).minX;
      [*] = ;                  var maxX : Integer = TArmyInfo(pArmyInfo).maxX;
      [*] = ;                  var minY : Integer = TArmyInfo(pArmyInfo).minY;
      [*] = ;                  var maxY : Integer = TArmyInfo(pArmyInfo).maxY;
      [*] = ;
      [*] = ;                  var scanX : Float = (TArmyInfo(pArmyInfo).minX + TArmyInfo(pArmyInfo).maxX) / 2;
      [*] = ;                  var scanY : Float = (TArmyInfo(pArmyInfo).minY + TArmyInfo(pArmyInfo).maxY) / 2;
      [*] = ;                  var x, z : Float;
      [*] = ;
      [*] = ;                  _misc_ScanGridIndicesToPos(scanX, scanY, x, z);
      [*] = ;                  var topZone : Integer = TopologyGetZoneIndex(x, z);
      [*] = ;                  if topZone >=0 then
      [*] = ;                  begin
      [*] = ;                     found := true;
      [*] = ;                     TopologySetZoneNeedPathByIndex(topZone, true);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            if found then
      [*] = ;            begin
      [*] = ;               //var nearZone : Integer = TopologyGetWeightPathFromZone(TArmy(pArmy).fTopZone, true);
      [*] = ;               var danger : Float;
      [*] = ;               var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;               var nearZone : Integer = _player_GetTopologyDangerPathFromZone(plHnd, TArmy(pArmy).fTopZone, true, danger);
      [*] = ;               if nearZone >= 0 then
      [*] = ;               begin
      [*] = ;                  var endX, endZ : Float;
      [*] = ;                  if _misc_TopZonePathLerp(nearZone, gc_ai_ApproachDist, false, endX, endZ) then
      [*] = ;                  Result := TopologyGetZoneIndex(endX, endZ);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_GetNextSafeZone
      [*] = ;//
      [*] = ;function _ai_GetNextSafeZone(plHnd, stZone, endZone : Integer) : Integer;
      [*] = ;begin
      [*] = ;   if IsInRangeInt(stZone, 0, TopologyGetZonesCount-1) and IsInRangeInt(endZone, 0, TopologyGetZonesCount-1) then
      [*] = ;   begin
      [*] = ;      var i : Integer;
      [*] = ;      for i := TopologyGetZonesCount-1 downto 0 do
      [*] = ;      TopologySetZoneNeedPathByIndex(i, false);
      [*] = ;
      [*] = ;      TopologySetZoneNeedPathByIndex(endZone, true);
      [*] = ;
      [*] = ;      var danger : Float;
      [*] = ;      var nearZone : Integer = _player_GetTopologyDangerPathFromZone(plHnd, stZone, true, danger);
      [*] = ;      if nearZone >= 0 then
      [*] = ;      begin
      [*] = ;         var endX, endZ : Float;
      [*] = ;         if _misc_TopZonePathLerp(nearZone, gc_ai_ApproachDist, false, endX, endZ) then
      [*] = ;         Result := TopologyGetZoneIndex(endX, endZ);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      Result := -1;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   Result := -1;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyCanAttackUnit
      [*] = ;//
      [*] = ;function _ai_ArmyCanAttackUnit(pArmy : Pointer; trgHnd : Integer) : Boolean;
      [*] = ;begin
      [*] = ;   var i, j : Integer;
      [*] = ;   Result := false;
      [*] = ;   for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;      if pSquad<>nil then
      [*] = ;      for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;         if _unit_CanKillIncludeAttMask(goHnd, trgHnd) then
      [*] = ;         begin
      [*] = ;            Result := true;
      [*] = ;            exit;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_FindNearestWall
      [*] = ;//
      [*] = ;procedure _ai_FindNearestWall(pArmy : Pointer; var zone, wallHnd : Integer);
      [*] = ;begin
      [*] = ;   zone := -1;
      [*] = ;   wallHnd := 0;
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      var myX : Float = TArmy(pArmy).fX;
      [*] = ;      var myZ : Float = TArmy(pArmy).fZ;
      [*] = ;      var myZone : Integer = TArmy(pArmy).fTopZone;
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var myIsland : Integer = _misc_GetIsland(myX, myZ);
      [*] = ;
      [*] = ;      var pInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;      if pInfo <> nil then
      [*] = ;      begin
      [*] = ;         var i, j, k : Integer;
      [*] = ;         // get path to all zones
      [*] = ;         for i := TopologyGetZonesCount-1 downto 0 do
      [*] = ;         TopologySetZoneNeedPathByIndex(i, true);
      [*] = ;
      [*] = ;         TopologyGetPathToZone(myZone);
      [*] = ;
      [*] = ;         // mark zones reachable by my army
      [*] = ;         gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;         gPlayer[plInd].aiData.aiList.Add(myZone);
      [*] = ;         _misc_TopologyWave(gPlayer[plInd].aiData.aiList, true, true, false);
      [*] = ;
      [*] = ;         // mark zones reachable by enemy armies
      [*] = ;         var minDist : Integer = gc_MaxInt;
      [*] = ;         var resZone : Integer = -1;
      [*] = ;         var nearHnd : Integer;
      [*] = ;         gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;         for i := TEnemyInfo(pInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pEnInfo : Pointer = TEnemyInfo(pInfo).armyInfos.Get(i);
      [*] = ;            if (pEnInfo<>nil) and (TArmyInfo(pEnInfo).landCount > 0) then
      [*] = ;            begin
      [*] = ;               var enX, enZ : Float;
      [*] = ;               _misc_GetArmyInfoPosition(pEnInfo, enX, enZ);
      [*] = ;               var enIsland : Integer = _misc_GetIsland(enX, enZ);
      [*] = ;               if enIsland = myIsland then
      [*] = ;               begin
      [*] = ;                  var zone : Integer = TopologyGetZoneIndex(enX, enZ);
      [*] = ;                  if TopologyGetZoneCheckedByIndex(zone) then
      [*] = ;                  begin
      [*] = ;                     var tpCount : Integer = TopologyGetZoneTrackPointsCountByIndex(zone);
      [*] = ;                     if tpCount < minDist then
      [*] = ;                     begin
      [*] = ;                        minDist := tpCount;
      [*] = ;                        resZone := zone;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  gPlayer[plInd].aiData.aiList.Add(zone);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if resZone < 0 then
      [*] = ;         begin
      [*] = ;            _misc_TopologyWave(gPlayer[plInd].aiData.aiList, false, false, true);
      [*] = ;
      [*] = ;            // determine walls need to be crushed to reach enemy armies
      [*] = ;            var arCount : Integer;
      [*] = ;            var zones : array [0.. gc_MaxBuilderCount-1] of Integer;
      [*] = ;            var wAttack : array [0..1] of Integer;
      [*] = ;
      [*] = ;            while gPlayer[plInd].aiData.aiList.GetCount > 0 do
      [*] = ;            begin
      [*] = ;               resZone := -1;
      [*] = ;               nearHnd := 0;
      [*] = ;               minDist := gc_MaxInt;
      [*] = ;               gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;
      [*] = ;               for i := gWallSystem.GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var pCluster : Pointer = gWallSystem.Get(i);
      [*] = ;                  for j := TWallCluster(pCluster).cells.GetCount-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     var pWall : Pointer = TWallCluster(pCluster).cells.GetPointer(j);
      [*] = ;                     var wHnd : Integer = TWallCell(pWall).goHnd;
      [*] = ;                     if wHnd <> 0 then
      [*] = ;                     begin
      [*] = ;                        var wx, wy, wz : Float;
      [*] = ;                        GetGameObjectAbsolutePositionByHandle(wHnd, wx, wy, wz);
      [*] = ;
      [*] = ;                        var pObj : Pointer = _unit_GetTObj(wHnd);
      [*] = ;
      [*] = ;                        var ind : Integer = -1;
      [*] = ;                        case gObjProp[TObj(pObj).cid, TObj(pObj).id].usage of
      [*] = ;                           gc_obj_usage_weakwall : ind := 0;
      [*] = ;                           gc_obj_usage_hardwall : ind := 1;
      [*] = ;                        end;
      [*] = ;
      [*] = ;                        var canAttack : Boolean;
      [*] = ;                        if ind >= 0 then
      [*] = ;                        begin
      [*] = ;                           case wAttack[ind] of
      [*] = ;                              -1: canAttack := false;
      [*] = ;                              1 : canAttack := true;
      [*] = ;                              0 : begin
      [*] = ;                                 canAttack := _ai_ArmyCanAttackUnit(pArmy, wHnd);
      [*] = ;                                 if canAttack then
      [*] = ;                                 wAttack[ind] := 1
      [*] = ;                                 else
      [*] = ;                                 wAttack[ind] := -1;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;
      [*] = ;                        if canAttack then
      [*] = ;                        begin
      [*] = ;                           var wCheck, wnCheck : Boolean;
      [*] = ;                           var wPath, wnPath : Boolean;
      [*] = ;
      [*] = ;                           var bCount : Integer = gCustomBuildPointsWall[TObj(pObj).wallVariation].builderCount;
      [*] = ;                           var bMinDist : Integer = gc_MaxInt;
      [*] = ;                           var bNearZone : Integer = -1;
      [*] = ;                           arCount := 0;
      [*] = ;                           for k := 0 to bCount-1 do
      [*] = ;                           begin
      [*] = ;                              var bx : Float = wx + gCustomBuildPointsWall[TObj(pObj).wallVariation].builderPoints[k].x;
      [*] = ;                              var bz : Float = wz + gCustomBuildPointsWall[TObj(pObj).wallVariation].builderPoints[k].y;
      [*] = ;                              var bZone : Integer = TopologyGetZoneIndex(bx, bz);
      [*] = ;                              if bZone >= 0 then
      [*] = ;                              begin
      [*] = ;                                 if TopologyGetZoneCheckedByIndex(bZone) then
      [*] = ;                                 begin
      [*] = ;                                    wCheck := true;
      [*] = ;                                    var dist : Integer = TopologyGetZoneTrackPointsCountByIndex(bZone);
      [*] = ;                                    if dist < bMinDist then
      [*] = ;                                    begin
      [*] = ;                                       bMinDist := dist;
      [*] = ;                                       bNearZone := bZone;
      [*] = ;                                    end;
      [*] = ;                                 end
      [*] = ;                                 else
      [*] = ;                                 begin
      [*] = ;                                    wnCheck := true;
      [*] = ;
      [*] = ;                                    if TopologyGetZoneNeedPathByIndex(bZone) then
      [*] = ;                                    wPath := true
      [*] = ;                                    else
      [*] = ;                                    begin
      [*] = ;                                       zones[arCount] := bZone;
      [*] = ;                                       arCount := arCount+1;
      [*] = ;                                       wnPath := true;
      [*] = ;                                    end;
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;
      [*] = ;                           if wPath and wnPath then
      [*] = ;                           begin
      [*] = ;                              // inner wall to reach enemy army
      [*] = ;                              for k := 0 to arCount-1 do
      [*] = ;                              begin
      [*] = ;                                 var zone : Integer = zones[k];
      [*] = ;                                 if not TopologyGetZoneNeedPathByIndex(zone) then
      [*] = ;                                 begin
      [*] = ;                                    TopologySetZoneNeedPathByIndex(zone, true);
      [*] = ;                                    gPlayer[plInd].aiData.aiList.Add(zone);
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;
      [*] = ;                           if wCheck and wPath then
      [*] = ;                           begin
      [*] = ;                              if bMinDist < minDist then
      [*] = ;                              begin
      [*] = ;                                 minDist := bMinDist;
      [*] = ;                                 resZone := bNearZone;
      [*] = ;                                 nearHnd := wHnd;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               _misc_TopologyWave(gPlayer[plInd].aiData.aiList, false, false, true);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         zone := resZone;
      [*] = ;         wallHnd := nearHnd;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_GetTopDanger
      [*] = ;//
      [*] = ;function _ai_GetTopDanger(plInd, topZone : Integer; dist : Float) : Integer;
      [*] = ;begin
      [*] = ;   var enForce : Integer;
      [*] = ;   var pEnemyInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;   if pEnemyInfo <> nil then
      [*] = ;   begin
      [*] = ;      var myX, myZ : Float;
      [*] = ;      TopologyGetZoneCenterByIndex(topZone, myX, myZ);
      [*] = ;      var i : Integer;
      [*] = ;      for i := TEnemyInfo(pEnemyInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pArmyInfo : Pointer = TEnemyInfo(pEnemyInfo).armyInfos.Get(i);
      [*] = ;         if TArmyInfo(pArmyInfo).landCount > 0 then
      [*] = ;         begin
      [*] = ;            {var minX : Float = TArmyInfo(pArmyInfo).minX;
      [*] = ;            var minY : Float = TArmyInfo(pArmyInfo).minY;
      [*] = ;            var maxX : Float = TArmyInfo(pArmyInfo).maxX;
      [*] = ;            var maxY : Float = TArmyInfo(pArmyInfo).maxY;
      [*] = ;
      [*] = ;            if (VectorDistance(myX, 0, myZ, minX, 0, minY) < dist) or
      [*] = ;            (VectorDistance(myX, 0, myZ, minX, 0, maxY) < dist) or
      [*] = ;            (VectorDistance(myX, 0, myZ, maxX, 0, minY) < dist) or
      [*] = ;            (VectorDistance(myX, 0, myZ, maxX, 0, maxY) < dist) then
      [*] = ;            enForce := enForce + TArmyInfo(pArmyInfo).force;}
      [*] = ;
      [*] = ;            var region : Integer = TopologyGetZoneRegionByIndex(topZone);
      [*] = ;            var aDist : Float = _ai_GetDistanceToArmy(myX, myZ, region, pArmyInfo);
      [*] = ;            if aDist < dist then
      [*] = ;            enForce := enForce + TArmyInfo(pArmyInfo).force;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   Result := enForce;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_FindNearestEnemyZoneExt
      [*] = ;//
      [*] = ;function _ai_FindNearestEnemyZoneExt(plInd, minForce : Integer; x, z : Float; var artList : TIntegerList; defence : Boolean) : Integer;
      [*] = ;begin
      [*] = ;   Result := -1;
      [*] = ;   if IsInRangeInt(plInd, 0, gc_MaxPlayerCount-1) then
      [*] = ;   begin
      [*] = ;      var pEnemyInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;      if pEnemyInfo <> nil then
      [*] = ;      begin
      [*] = ;         var myZone : Integer = TopologyGetZoneIndex(x, z);
      [*] = ;         if myZone >=0 then
      [*] = ;         begin
      [*] = ;            var artCount : Integer = artList.GetCount;
      [*] = ;            var isArt : Boolean = (artCount > 0);
      [*] = ;
      [*] = ;            var myPlMask : Integer = gPlayer[plInd].myPlMask;
      [*] = ;            var myEnPlMask : Integer = gPlayer[plInd].enemyPlMask;
      [*] = ;            var armyCount : Integer = TEnemyInfo(pEnemyInfo).armyInfos.GetCount;
      [*] = ;            var i, j, k : Integer;
      [*] = ;            gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;            for i := 0 to armyCount-1 do
      [*] = ;            begin
      [*] = ;               var pArmyInfo : Pointer = TEnemyInfo(pEnemyInfo).armyInfos.Get(i);
      [*] = ;               if TArmyInfo(pArmyInfo).landCount > 0 then
      [*] = ;               begin
      [*] = ;                  //if (TArmyInfo(pArmyInfo).force >= minForce) then
      [*] = ;                  if (TArmyInfo(pArmyInfo).force >= minForce) or (TArmyInfo(pArmyInfo).cannons > 0) or
      [*] = ;                  (TArmyInfo(pArmyInfo).mortars > 0) or (TArmyInfo(pArmyInfo).mcannons > 0) then
      [*] = ;                  begin
      [*] = ;                     var aScanX : Float = (TArmyInfo(pArmyInfo).minX + TArmyInfo(pArmyInfo).maxX) / 2;
      [*] = ;                     var aScanY : Float = (TArmyInfo(pArmyInfo).minY + TArmyInfo(pArmyInfo).maxY) / 2;
      [*] = ;                     var aX, aY : Float;
      [*] = ;                     _misc_ScanGridIndicesToPos(aScanX, aScanY, aX, aY);
      [*] = ;                     var armyZone : Integer = TopologyGetZoneIndex(aX, aY);
      [*] = ;                     if armyZone >= 0 then
      [*] = ;                     begin
      [*] = ;                        var add : Boolean = (not isArt);
      [*] = ;                        for j := artCount-1 downto 0 do
      [*] = ;                        begin
      [*] = ;                           var goHnd : Integer = artList.Get(j);
      [*] = ;                           var trgHnd : Integer;
      [*] = ;                           var trgX, trgZ : Float;
      [*] = ;                           if _unit_SearchEnemyLongRangeArtillery(goHnd, aX, aY, false, trgHnd, trgX, trgZ, True) then
      [*] = ;                           begin
      [*] = ;                              add := true;
      [*] = ;                              break;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;
      [*] = ;                        if add then
      [*] = ;                        gPlayer[plInd].aiData.aiList.Add(armyZone);
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            var topZone : Integer = _misc_GetNearestTopZone(gPlayer[plInd].aiData.aiList, myZone);
      [*] = ;            if topZone < 0 then
      [*] = ;            begin
      [*] = ;               if not isArt then
      [*] = ;               begin
      [*] = ;                  for i := TopologyGetZonesCount-1 downto 0 do
      [*] = ;                  TopologySetZoneNeedPathByIndex(i, false);
      [*] = ;
      [*] = ;                  for i := 0 to gc_scangrid_countx-1 do
      [*] = ;                  for j := 0 to gc_scangrid_county-1 do
      [*] = ;                  begin
      [*] = ;                     if gScanGrid[i, j].fPlMask and myEnPlMask <> 0 then
      [*] = ;                     begin
      [*] = ;                        for k := gScanGrid[i, j].GetCount-1 downto 0 do
      [*] = ;                        begin
      [*] = ;                           var goHnd : Integer = gScanGrid[i, j].Get(k);
      [*] = ;                           var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                           var pl : Integer = TObj(pObj).pl;
      [*] = ;                           if gPlayer[pl].bExists and (gPlayer[pl].myPlMask and myEnPlMask <> 0) and
      [*] = ;                           (_unit_GetTagStateByType(goHnd, gc_statetag_essential) = gc_statetag_essential_none) then
      [*] = ;                           begin
      [*] = ;                              var pObjProp : Pointer = gObjProp[TObj(pObj).cid, TObj(pObj).id];
      [*] = ;                              if ((not TObjProp(pObjProp).bBuilding) and (TObjProp(pObjProp).media = gc_obj_media_land) and (not TObjProp(pObjProp).bWall))
      [*] = ;                              or TObjProp(pObjProp).bCapture or (TObjProp(pObjProp).bBuilding and (not TObj(pObj).bBuilt)) then
      [*] = ;                              begin
      [*] = ;                                 var x : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;                                 var z : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;                                 var topZone : Integer = TopologyGetZoneIndex(x, z);
      [*] = ;                                 if topZone >= 0 then
      [*] = ;                                 TopologySetZoneNeedPathByIndex(topZone, true);
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  TopologyGetPathToZone(myZone);
      [*] = ;
      [*] = ;                  var minDist : Integer = gc_MaxInt;
      [*] = ;                  var destZone : Integer = -1;
      [*] = ;                  for i := TopologyGetZonesCount-1 downto 0 do
      [*] = ;                  if TopologyGetZoneNeedPathByIndex(i) then
      [*] = ;                  begin
      [*] = ;                     var tpCount : Integer = TopologyGetZoneTrackPointsCountByIndex(i);
      [*] = ;                     if IsInRangeInt(tpCount, 1, minDist-1) then
      [*] = ;                     begin
      [*] = ;                        minDist := tpCount;
      [*] = ;                        destZone := i;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  Result := destZone;
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               Result := topZone;
      [*] = ;               if defence then
      [*] = ;               begin
      [*] = ;                  var cX, cZ : Float;
      [*] = ;                  TopologyGetZoneCenterByIndex(topZone, cX, cZ);
      [*] = ;                  var scanX, scanZ : Integer;
      [*] = ;                  _misc_PosToScanGridIndices(cX, cZ, scanX, scanZ);
      [*] = ;                  var cScanX, cScanZ : Integer;
      [*] = ;                  _misc_PosToScanGridIndices(gPlayer[plInd].aiData.centerX, gPlayer[plInd].aiData.centerZ, cScanX, cScanZ);
      [*] = ;
      [*] = ;                  if VectorDistance(scanX, 0, scanZ, cScanX, 0, cScanZ) > 16 then
      [*] = ;                  begin
      [*] = ;                     var rad : Integer = 4;
      [*] = ;                     var minX : Integer = Max(0, cScanX-rad);
      [*] = ;                     var maxX : Integer = Min(gc_scangrid_countx, cScanX+rad);
      [*] = ;                     var minY : Integer = Max(0, cScanZ-rad);
      [*] = ;                     var maxY : Integer = Min(gc_scangrid_countx, cScanZ+rad);
      [*] = ;
      [*] = ;                     var resX : Integer = minX + Floor(RandomExt * (maxX+1-minX));
      [*] = ;                     var resY : Integer = minY + Floor(RandomExt * (maxY+1-minY));
      [*] = ;
      [*] = ;                     var posX, posZ : Float;
      [*] = ;                     _misc_ScanGridIndicesToPos(resX, resY, posX, posZ);
      [*] = ;                     var endZone : Integer = TopologyGetZoneIndex(posX, posZ);
      [*] = ;                     Result := endZone;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_FindNearestEnemyZone
      [*] = ;//
      [*] = ;function _ai_FindNearestEnemyZone(plInd, minForce : Integer; x, z : Float; defence : Boolean) : Integer;
      [*] = ;begin
      [*] = ;   gIntegerList.Clear;
      [*] = ;   Result := _ai_FindNearestEnemyZoneExt(plInd, minForce, x, z, gIntegerList, defence);
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_FindNearestWaterEnemyZone
      [*] = ;//
      [*] = ;function _ai_FindNearestWaterEnemyZone(plInd, minForce : Integer; x, z : Float) : Integer;
      [*] = ;begin
      [*] = ;   Result := -1;
      [*] = ;   if IsInRangeInt(plInd, 0, gc_MaxPlayerCount-1) then
      [*] = ;   begin
      [*] = ;      var pEnemyInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;      if pEnemyInfo <> nil then
      [*] = ;      begin
      [*] = ;         var myZone : Integer = TopologyGetZoneIndex(x, z);
      [*] = ;         if myZone >=0 then
      [*] = ;         begin
      [*] = ;            var myPlMask : Integer = gPlayer[plInd].myPlMask;
      [*] = ;            var myEnPlMask : Integer = gPlayer[plInd].enemyPlMask;
      [*] = ;            var armyCount : Integer = TEnemyInfo(pEnemyInfo).armyInfos.GetCount;
      [*] = ;            var i, j, k : Integer;
      [*] = ;            gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;            for i := 0 to armyCount-1 do
      [*] = ;            begin
      [*] = ;               var pArmyInfo : Pointer = TEnemyInfo(pEnemyInfo).armyInfos.Get(i);
      [*] = ;               if (TArmyInfo(pArmyInfo).waterCount > 0) and (TArmyInfo(pArmyInfo).force >= minForce) then
      [*] = ;               begin
      [*] = ;                  var armyZone : Integer = _ai_GetArmyInfoTopZone(pArmyInfo);
      [*] = ;                  if armyZone >= 0 then
      [*] = ;                  gPlayer[plInd].aiData.aiList.Add(armyZone);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            Result := _misc_GetNearestTopZone(gPlayer[plInd].aiData.aiList, myZone);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_GetArtilleryEnemyZone
      [*] = ;//
      [*] = ;function _ai_GetArtilleryEnemyZone(pArmy : Pointer; x, z : Float) : Integer;
      [*] = ;begin
      [*] = ;   Result := 0;
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      // TODO: not finished. Write code for walls
      [*] = ;      var i, j : Integer;
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      gIntegerList.Clear;
      [*] = ;      for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;         if pSquad<>nil then
      [*] = ;         begin
      [*] = ;            var isArt : Boolean = _ai_SquadIsArtillery(pSquad);
      [*] = ;
      [*] = ;            if (TSquad(pSquad).GetCount > 0) and isArt then
      [*] = ;            begin
      [*] = ;               var goHnd : Integer = TSquad(pSquad).Get(0);
      [*] = ;               if (goHnd<>0) then
      [*] = ;               begin
      [*] = ;                  var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;                  if (pObjProp<>nil) then
      [*] = ;                  begin
      [*] = ;                     var usage : Integer = TObjProp(pObjProp).usage;
      [*] = ;
      [*] = ;                     var inList : Boolean;
      [*] = ;                     for j := gIntegerList.GetCount-1 downto 0 do
      [*] = ;                     begin
      [*] = ;                        var goHnd1 : Integer = gIntegerList.Get(j);
      [*] = ;                        var pObjProp1 : Pointer = _unit_GetObjProp(goHnd1);
      [*] = ;                        if (pObjProp1<>nil) and (TObjProp(pObjProp1).usage = usage) then
      [*] = ;                        begin
      [*] = ;                           inList := true;
      [*] = ;                           break;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;
      [*] = ;                     if not inList then
      [*] = ;                     gIntegerList.Add(goHnd);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      var needTower : Boolean;
      [*] = ;      for i := gIntegerList.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = gIntegerList.Get(i);
      [*] = ;         if (goHnd<>0) then
      [*] = ;         begin
      [*] = ;            var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;            if (pObjProp<>nil) and ((TObjProp(pObjProp).usage = gc_obj_usage_cannon) or (TObjProp(pObjProp).usage = gc_obj_usage_supermortar)) then
      [*] = ;            begin
      [*] = ;               needTower := true;
      [*] = ;               break;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      var enZone : Integer = -1;
      [*] = ;      var towerZone : Integer = -1;
      [*] = ;      if needTower then
      [*] = ;      begin
      [*] = ;         var enHnd : Integer = _ai_GetNearestEnemyTower(plInd, x, z);
      [*] = ;         if enHnd <> 0 then
      [*] = ;         begin
      [*] = ;            var enX : Float = GetGameObjectPositionXByHandle(enHnd);
      [*] = ;            var enZ : Float = GetGameObjectPositionZByHandle(enHnd);
      [*] = ;            towerZone := TopologyGetZoneIndex(enX, enZ);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      var armyZone : Integer = _ai_FindNearestEnemyZoneExt(plInd, 30, x, z, gIntegerList, false);
      [*] = ;
      [*] = ;      if (towerZone >= 0) and (armyZone >= 0) then
      [*] = ;      begin
      [*] = ;         gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;         gPlayer[plInd].aiData.aiList.Add(towerZone);
      [*] = ;         gPlayer[plInd].aiData.aiList.Add(armyZone);
      [*] = ;         var myZone : Integer = TopologyGetZoneIndex(x, z);
      [*] = ;         if myZone >= 0 then
      [*] = ;         enZone := _misc_GetNearestTopZone(gPlayer[plInd].aiData.aiList, myZone);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if towerzone >= 0 then
      [*] = ;      enZone := towerZone
      [*] = ;      else
      [*] = ;      enZone := armyZone;
      [*] = ;
      [*] = ;      Result := enZone;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_GetArmyDanger
      [*] = ;//
      [*] = ;function _ai_GetArmyDanger(pArmy : Pointer; posX, posZ, dirX, dirZ : Float) : Float;
      [*] = ;begin
      [*] = ;   var maxDanger : Float;
      [*] = ;   if (pArmy <> nil) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var team : Integer = gPlayer[plInd].team;
      [*] = ;      var pEnInfo : Pointer = _misc_GetTeamEnemyInfo(team);
      [*] = ;      if pEnInfo <> nil then
      [*] = ;      begin
      [*] = ;         var aSquadCount : Integer = TArmy(pArmy).fSquadList.GetCount;
      [*] = ;         var aCols : Integer = MathCeil(Sqrt(aSquadCount));
      [*] = ;         var aRows : Integer = MathCeil(aSquadCount/aCols);
      [*] = ;         var dirY : Float;
      [*] = ;         if VectorLength(dirX, dirY, dirZ) = 0 then
      [*] = ;         begin
      [*] = ;            dirX := posX - TArmy(pArmy).fCurX;
      [*] = ;            dirZ := posZ - TArmy(pArmy).fCurZ;
      [*] = ;            if VectorLength(dirX, dirY, dirZ) = 0 then
      [*] = ;            begin
      [*] = ;               dirX := 1;
      [*] = ;               dirZ := 0;
      [*] = ;            end
      [*] = ;         end;
      [*] = ;
      [*] = ;         VectorNormalize(dirX, dirY, dirZ);
      [*] = ;
      [*] = ;         var hDirX : Float = dirX;
      [*] = ;         var hDirY : Float;
      [*] = ;         var hDirZ : Float = dirZ;
      [*] = ;         VectorRotateY(hDirX, hDirY, hDirZ, 90);
      [*] = ;
      [*] = ;         var i : Integer;
      [*] = ;         for i := 0 to aSquadCount-1 do
      [*] = ;         begin
      [*] = ;            var pSquad : Pointer = TArmy(pArmy).GetSquad(i);
      [*] = ;            if pSquad<>nil then
      [*] = ;            begin
      [*] = ;               var col : Integer = i mod aCols;
      [*] = ;               var row : Integer = i div aCols;
      [*] = ;               var offDir : Float = ((aRows-1)/2 - row) * TArmy(pArmy).fOffsetRow;
      [*] = ;               var offHDir : Float = ((aCols-1)/2 - col) * TArmy(pArmy).fOffsetCol;
      [*] = ;
      [*] = ;               var x : Float = posX + offHDir * hDirX + offDir * dirX;
      [*] = ;               var z : Float = posZ + offHDir * hDirZ + offDir * dirZ;
      [*] = ;
      [*] = ;               var topQuadTree : Integer = TopologyGetTopologyQuadTree;
      [*] = ;               var topPrior : Integer = TopologyGetTopologyPriority;
      [*] = ;               if not TraceLineQuadTree(posX, posZ, x, z, topPrior, topQuadTree) then
      [*] = ;               begin
      [*] = ;                  x := posX;
      [*] = ;                  z := posZ;
      [*] = ;               end;
      [*] = ;
      [*] = ;               var gridX, gridZ : Integer;
      [*] = ;               if _misc_PosToScanGridIndices(x, z, gridX, gridZ) then
      [*] = ;               begin
      [*] = ;                  if TEnemyInfo(pEnInfo).towerMap[gridX, gridZ] > maxDanger then
      [*] = ;                  maxDanger := TEnemyInfo(pEnInfo).towerMap[gridX, gridZ];
      [*] = ;               end
      [*] = ;               else
      [*] = ;               ErrorLog('_ai_GetArmyDanger : pos out of range');
      [*] = ;
      [*] = ;               if (gridX >= 2) and (gridZ >= 2) and (TEnemyInfo(pEnInfo).towerMap[gridX-2, gridZ-2] > maxDanger) then
      [*] = ;               maxDanger := TEnemyInfo(pEnInfo).towerMap[gridX-2, gridZ-2];
      [*] = ;
      [*] = ;               if (gridX >= 2) and (gridZ < gc_scangrid_county-2) and (TEnemyInfo(pEnInfo).towerMap[gridX-2, gridZ+2] > maxDanger) then
      [*] = ;               maxDanger := TEnemyInfo(pEnInfo).towerMap[gridX-2, gridZ+2];
      [*] = ;
      [*] = ;               if (gridX < gc_scangrid_countx-2) and (gridZ >= 2) and (TEnemyInfo(pEnInfo).towerMap[gridX+2, gridZ-2] > maxDanger) then
      [*] = ;               maxDanger := TEnemyInfo(pEnInfo).towerMap[gridX+2, gridZ-2];
      [*] = ;
      [*] = ;               if (gridX < gc_scangrid_countx-2) and (gridZ < gc_scangrid_county-2) and (TEnemyInfo(pEnInfo).towerMap[gridX+2, gridZ+2] > maxDanger) then
      [*] = ;               maxDanger := TEnemyInfo(pEnInfo).towerMap[gridX+2, gridZ+2];
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   Result := maxDanger;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_SendToMostSafePosition
      [*] = ;//
      [*] = ;procedure _ai_SendToMostSafePosition(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var curZone : Integer = TArmy(pArmy).fCurTopZone;
      [*] = ;      if IsInRangeInt(curZone, 0, TopologyGetZonesCount-1) then
      [*] = ;      begin
      [*] = ;         var x, z : Float;
      [*] = ;         TopologyGetZoneCenterByIndex(curZone, x, z);
      [*] = ;         var minDan : Float = _ai_GetArmyDanger(pArmy, x, z, 0, 0);
      [*] = ;
      [*] = ;         var newZone : Integer = -1;
      [*] = ;
      [*] = ;         var conCount : Integer = TopologyGetZoneConnectionsCountByIndex(curZone);
      [*] = ;         var i : Integer;
      [*] = ;         for i := 0 to conCount-1 do
      [*] = ;         begin
      [*] = ;            var conInd : Integer = TopologyGetZoneConnectionByIndex(curZone, i);
      [*] = ;            var conDist : Integer = TopologyGetZoneConnectionDistByIndex(curZone, i);
      [*] = ;
      [*] = ;            if conDist <= gc_ai_SafePositionDist then
      [*] = ;            begin
      [*] = ;               if conInd >= 0 then
      [*] = ;               begin
      [*] = ;                  var x, z : Float;
      [*] = ;                  TopologyGetZoneCenterByIndex(conInd, x, z);
      [*] = ;                  var dan : Float = _ai_GetArmyDanger(pArmy, x, z, 0, 0);
      [*] = ;                  if dan < minDan then
      [*] = ;                  begin
      [*] = ;                     minDan := dan;
      [*] = ;                     newZone := conInd;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            break;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if newZone >= 0 then
      [*] = ;         begin
      [*] = ;            var x, z : Float;
      [*] = ;            TopologyGetZoneCenterByIndex(newZone, x, z);
      [*] = ;            _ai_SetArmyPosition(pArmy, x, z, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyTooStrong
      [*] = ;//
      [*] = ;function _ai_ArmyTooStrong(pArmy : Pointer) : Boolean;
      [*] = ;begin
      [*] = ;   Result := False;
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var pEnemyInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;      if pEnemyInfo <> nil then
      [*] = ;      begin
      [*] = ;         if (TArmy(pArmy).fSquadList.GetCount > 3) and (TArmy(pArmy).fForce > TEnemyInfo(pEnemyInfo).maxArmyForce) then
      [*] = ;         Result := true;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyCanMerge
      [*] = ;//
      [*] = ;function _ai_ArmyCanMerge(pArmy : Pointer) : Boolean;
      [*] = ;begin
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      if _ai_IsWaterArmy(pArmy) then
      [*] = ;      Result := true
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         Result := (not _ai_ArmyTooStrong(pArmy));
      [*] = ;         if Result then
      [*] = ;         begin
      [*] = ;            var sqCount : Integer = TArmy(pArmy).fSquadList.GetCount;
      [*] = ;            if sqCount = 1 then
      [*] = ;            Result := true
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               var cols : Integer = MathCeil(Sqrt(sqCount));
      [*] = ;               var rows : Integer = MathCeil(sqCount/cols);
      [*] = ;
      [*] = ;               var height : Float = (rows-1)/2 * TArmy(pArmy).fOffsetRow;
      [*] = ;               var width : Float = (cols-1)/2 * TArmy(pArmy).fOffsetCol;
      [*] = ;               var maxDist : Float = Sqrt(Sqr(height) + Sqr(width));
      [*] = ;
      [*] = ;               var i : Integer;
      [*] = ;               for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;                  if pSquad<>nil then
      [*] = ;                  begin
      [*] = ;                     var sqX : Float = TSquad(pSquad).fCurX;
      [*] = ;                     var sqZ : Float = TSquad(pSquad).fCurZ;
      [*] = ;                     var armyX : Float = TArmy(pArmy).fSquadsCurX;
      [*] = ;                     var armyZ : Float = TArmy(pArmy).fSquadsCurZ;
      [*] = ;                     var dist : Float = VectorDistance(TSquad(pSquad).fCurX, 0, TSquad(pSquad).fCurZ, TArmy(pArmy).fSquadsCurX, 0, TArmy(pArmy).fSquadsCurZ);
      [*] = ;                     if dist > 2 * maxDist then
      [*] = ;                     begin
      [*] = ;                        Result := false;
      [*] = ;                        break;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   Result := false;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyJoin
      [*] = ;//
      [*] = ;procedure _ai_ArmyJoin(pMainArmy, pArmy : Pointer; setPos : Boolean);
      [*] = ;begin
      [*] = ;   if (pMainArmy <> nil) and (pArmy <> nil) and (TArmy(pMainArmy).fPlIndex = TArmy(pArmy).fPlIndex) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pMainArmy).fPlIndex;
      [*] = ;      var pEnemyInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;      if pEnemyInfo <> nil then
      [*] = ;      begin
      [*] = ;         var i : Integer;
      [*] = ;         for i := TArmy(pArmy).fSquadList.GetCount -1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;            if (pSquad<>nil) then
      [*] = ;            _ai_ArmyAddSquad(pMainArmy, pSquad);
      [*] = ;         end;
      [*] = ;
      [*] = ;         var ind : Integer = TArmy(pArmy).fIndex;
      [*] = ;         gPlayer[plInd].aiData.armyList.Delete(ind);
      [*] = ;
      [*] = ;         // find position for merged army
      [*] = ;         var posX, posZ, dirX, dirZ : Float;
      [*] = ;         _ai_GetArmyCentralPosAndDir(pMainArmy, posX, posZ, dirX, dirZ);
      [*] = ;
      [*] = ;         if (not _ai_IsWaterArmy(pMainArmy)) and (not _ai_IsWaterArmy(pArmy)) then
      [*] = ;         begin
      [*] = ;            var cenX : Float = gPlayer[plInd].aiData.centerX;
      [*] = ;            var cenZ : Float = gPlayer[plInd].aiData.centerZ;
      [*] = ;            var cenRegion : Integer = _misc_GetRegionByPos(cenX, cenZ);
      [*] = ;            var cenDist : Float = VectorDistance(cenX, 0, cenZ, posX, 0, posZ);
      [*] = ;            if IsInRange(cenDist, gc_epsilon, gc_ai_MergeArmyCityDist) then
      [*] = ;            begin
      [*] = ;               var cenDirX : Float = posX - cenX;
      [*] = ;               var cenDirZ : Float = posZ - cenZ;
      [*] = ;               var minAngle : Float = gc_MaxInt;
      [*] = ;               var enInd : Integer = -1;
      [*] = ;               var enX, enZ : Float;
      [*] = ;
      [*] = ;               var danger : Boolean;
      [*] = ;               for i := TEnemyInfo(pEnemyInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var pArmyInfo : Pointer = TEnemyInfo(pEnemyInfo).armyInfos.Get(i);
      [*] = ;                  if TArmyInfo(pArmyInfo).landCount > 0 then
      [*] = ;                  begin
      [*] = ;                     var dist : Float = _ai_GetDistanceToArmy(cenX, cenZ, cenRegion, pArmyInfo);
      [*] = ;                     if dist * gc_scangrid_size < gc_ai_CityDangerDist then
      [*] = ;                     begin
      [*] = ;                        danger := true;
      [*] = ;                        break;
      [*] = ;                     end;
      [*] = ;
      [*] = ;                     var enGridX : Float = (TArmyInfo(pArmyInfo).minX + TArmyInfo(pArmyInfo).maxX) / 2;
      [*] = ;                     var enGridY : Float = (TArmyInfo(pArmyInfo).minY + TArmyInfo(pArmyInfo).maxY) / 2;
      [*] = ;                     var x, z : Float;
      [*] = ;                     _misc_ScanGridIndicesToPos(enGridX, enGridY, x, z);
      [*] = ;
      [*] = ;                     var enDirX : Float = x - cenX;
      [*] = ;                     var enDirZ : Float = z - cenZ;
      [*] = ;
      [*] = ;                     if VectorLength(enDirX, 0, enDirZ) > gc_epsilon then
      [*] = ;                     begin
      [*] = ;                        var angle : Float = VectorAngle(cenDirX, 0, cenDirZ, enDirX, 0, enDirZ);
      [*] = ;                        if angle < minAngle then
      [*] = ;                        begin
      [*] = ;                           minAngle := angle;
      [*] = ;                           enInd := i;
      [*] = ;                           enX := x;
      [*] = ;                           enZ := z;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;
      [*] = ;               if (not danger) and (enInd >= 0) then
      [*] = ;               _misc_VectorLerpDist(posX, posZ, enX, enZ, gc_ai_MergeApproachDist, posX, posZ);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if setPos then
      [*] = ;         _ai_SetArmyPosition(pMainArmy, posX, posZ, dirX, dirZ, gc_obj_order_move_mode_default);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_CreateArmy
      [*] = ;//
      [*] = ;function _ai_CreateArmy(plInd : Integer; var list : TIntegerList; checkEssential : Boolean) : Pointer;
      [*] = ;begin
      [*] = ;   var armyType : Integer;
      [*] = ;   var pMainArmy : Pointer = nil;
      [*] = ;   while (armyType >= 0) and (list.GetCount > 0) do
      [*] = ;   begin
      [*] = ;      armyType := _ai_CheckMinArmyCreationAbility(plInd, list, false, checkEssential, false);
      [*] = ;      if armyType >= 0 then
      [*] = ;      begin
      [*] = ;         var pNewArmy : Pointer = _ai_CreateMinimalArmy(plInd, armyType, 0, list, checkEssential, false);
      [*] = ;         if pNewArmy <> nil then
      [*] = ;         begin
      [*] = ;            if pMainArmy = nil then
      [*] = ;            pMainArmy := pNewArmy
      [*] = ;            else
      [*] = ;            _ai_ArmyJoin(pMainArmy, pNewArmy, false);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         break;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   Result := pMainArmy;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyMakeLandBattleLink
      [*] = ;//
      [*] = ;procedure _ai_ArmyMakeLandBattleLink(pArmy : Pointer; active, findWall : Boolean);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      //Log('_ai_ArmyMakeLandBattleLink for [' + IntToStr(TArmy(pArmy).fIndex) + '] army of player ' + IntToStr(plInd));
      [*] = ;      var pEnemyInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;      if pEnemyInfo = nil then
      [*] = ;      exit;
      [*] = ;
      [*] = ;      var enHnd : Integer;
      [*] = ;      if not gbool_peacemode then
      [*] = ;      enHnd := _ai_ArmyHandleArchers(pArmy, 18 * gc_ai_ArcherArrayFactor);
      [*] = ;
      [*] = ;      if (enHnd <> 0) then
      [*] = ;      begin
      [*] = ;         var pObjProp : Pointer = _unit_GetObjProp(enHnd);
      [*] = ;         if (pObjProp<>nil) and (not TObjProp(pObjProp).bBuilding) then
      [*] = ;         begin
      [*] = ;            var x, y, z : Float;
      [*] = ;            GetGameObjectAbsolutePositionByHandle(enHnd, x, y, z);
      [*] = ;            _ai_ArmyBitva(pArmy, x, z);
      [*] = ;         end;
      [*] = ;         exit;
      [*] = ;      end;
      [*] = ;
      [*] = ;      var isArt : Boolean;
      [*] = ;      var isArtOnly : Boolean = true;
      [*] = ;      var isCannon, isMortar, isHowitzer : Boolean;
      [*] = ;      var i, j : Integer;
      [*] = ;      for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;         if pSquad<>nil then
      [*] = ;         begin
      [*] = ;            var sqType : Integer = TSquad(pSquad).fType;
      [*] = ;
      [*] = ;            case sqType of
      [*] = ;               gc_ai_armytype_cannon : begin
      [*] = ;                  isArt := true;
      [*] = ;                  isCannon := true;
      [*] = ;               end;
      [*] = ;               gc_ai_armytype_mortar : begin
      [*] = ;                  isArt := true;
      [*] = ;                  isHowitzer := true;
      [*] = ;               end;
      [*] = ;               gc_ai_armytype_supermortar : begin
      [*] = ;                  isArt := true;
      [*] = ;                  isMortar := true;
      [*] = ;               end;
      [*] = ;               else
      [*] = ;               isArtOnly := false;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      // TODO: not finished
      [*] = ;      // _ai_Army_SendGrenadiers(pArmy);
      [*] = ;
      [*] = ;      var minArmyDist : Integer = 10000;
      [*] = ;      var nearArmyZone : Integer = -1;
      [*] = ;
      [*] = ;      _ai_ArmyAlign(pArmy);
      [*] = ;
      [*] = ;      var myZone : Integer = TArmy(pArmy).fTopZone;
      [*] = ;
      [*] = ;      // check if can merge with another army
      [*] = ;      if (myZone >= 0) and _ai_ArmyCanMerge(pArmy) then
      [*] = ;      begin
      [*] = ;         var army, i : Integer;
      [*] = ;         var sqCount : Integer = TArmy(pArmy).fSquadList.GetCount;
      [*] = ;
      [*] = ;         for army := gPlayer[plInd].aiData.armyList.GetCount-1 downto 0 do
      [*] = ;         if army <> TArmy(pArmy).fIndex then
      [*] = ;         begin
      [*] = ;            var pArmy1 : Pointer = gPlayer[plInd].aiData.armyList.Get(army);
      [*] = ;            if pArmy1<>nil then
      [*] = ;            begin
      [*] = ;               var sqCount1 : Integer = TArmy(pArmy1).fSquadList.GetCount;
      [*] = ;               if (TArmy(pArmy).fSpec = TArmy(pArmy1).fSpec) and (not TArmy(pArmy1).fSpecialOrder) and (TArmy(pArmy1).fOrder.iType<>gc_ai_armyorder_buildmine) and //Suvorov do not merge mine builders
      [*] = ;               (TArmy(pArmy1).fTopZone >= 0) and _ai_ArmyCanMerge(pArmy1) then
      [*] = ;               begin
      [*] = ;                  var dist : Integer = TopologyGetZonesGraphDist(myZone, TArmy(pArmy1).fTopZone, gc_ai_MaxNearestArmyDist);
      [*] = ;                  if dist >= 0 then
      [*] = ;                  begin
      [*] = ;                     if dist <= gc_ai_MergeArmyDist then
      [*] = ;                     begin
      [*] = ;                        _ai_ArmyJoin(pArmy, pArmy1, true);
      [*] = ;                        exit;
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     if sqCount1 >= sqCount then
      [*] = ;                     begin
      [*] = ;                        if dist < minArmyDist then
      [*] = ;                        begin
      [*] = ;                           minArmyDist := dist;
      [*] = ;                           nearArmyZone := TArmy(pArmy1).fTopZone;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  {var dist : Integer = TopologyGetZonesGraphDist(myZone, TArmy(pArmy1).fTopZone, gc_ai_MergeArmyDist);
      [*] = ;                  if dist >= 0 then
      [*] = ;                  begin
      [*] = ;                     _ai_ArmyJoin(pArmy, pArmy1, true);
      [*] = ;                     exit;
      [*] = ;                  end;}
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      var myForce : Float = _ai_GetArmyForce(pArmy);
      [*] = ;      var uCount : Integer;
      [*] = ;      for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;         if pSquad<>nil then
      [*] = ;         uCount := uCount + TSquad(pSquad).GetCount;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if (uCount > 200) or (myForce > 3800) then
      [*] = ;      myForce := 100000;
      [*] = ;
      [*] = ;      var rMin, enDist : Float;
      [*] = ;
      [*] = ;      if TArmy(pArmy).fSpec = gc_ai_armyspec_grenadier then
      [*] = ;      begin
      [*] = ;         rMin := gc_ai_GreArmyBattleDist;
      [*] = ;         enDist := 3;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         rMin := gc_ai_ArmyBattleDist;
      [*] = ;         enDist := 7;
      [*] = ;      end;
      [*] = ;
      [*] = ;      rMin := rMin * gc_ai_ScanGridFactor;
      [*] = ;      enDist := enDist * gc_ai_ScanGridFactor;
      [*] = ;
      [*] = ;      var enForce : Integer;
      [*] = ;      var maxForce : Integer = -1;
      [*] = ;      var maxForceX, maxForceY : Float;
      [*] = ;      var batX, batZ : Float;
      [*] = ;
      [*] = ;      var myX : Float = TArmy(pArmy).fX;
      [*] = ;      var myZ : Float = TArmy(pArmy).fZ;
      [*] = ;      var myRegion : Integer = TArmy(pArmy).fRegion;
      [*] = ;      var myScanX, myScanZ : Integer;
      [*] = ;      _misc_PosToScanGridIndices(myX, myZ, myScanX, myScanZ);
      [*] = ;
      [*] = ;      for i := TEnemyInfo(pEnemyInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pArmyInfo : Pointer = TEnemyInfo(pEnemyInfo).armyInfos.Get(i);
      [*] = ;         if (pArmyInfo<>nil)and (TArmyInfo(pArmyInfo).landCount > 0) then
      [*] = ;         begin
      [*] = ;            var dist : Float = _ai_GetDistanceToArmy(myX, myZ, myRegion, pArmyInfo);
      [*] = ;            if dist <= enDist then
      [*] = ;            begin
      [*] = ;               var curForce : Integer = TArmyInfo(pArmyInfo).force;
      [*] = ;               if curForce > maxForce then
      [*] = ;               begin
      [*] = ;                  maxForce := curForce;
      [*] = ;                  maxForceX := (TArmyInfo(pArmyInfo).minX + TArmyInfo(pArmyInfo).maxX) / 2;
      [*] = ;                  maxForceY := (TArmyInfo(pArmyInfo).minY + TArmyInfo(pArmyInfo).maxY) / 2;
      [*] = ;               end;
      [*] = ;               enForce := enForce + curForce;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      var cenX : Float = gPlayer[plInd].aiData.centerX;
      [*] = ;      var cenZ : Float = gPlayer[plInd].aiData.centerZ;
      [*] = ;      var cScanX, cScanZ : Integer;
      [*] = ;      _misc_PosToScanGridIndices(cenX, cenZ, cScanX, cScanZ);
      [*] = ;      var cenDist : Float = VectorDistance(maxForceX, 0, maxForceY, cScanX, 0, cScanZ);
      [*] = ;
      [*] = ;      if cenDist < 10 * gc_ai_ScanGridFactor then
      [*] = ;      myForce := myForce * 8
      [*] = ;      else
      [*] = ;      if cenDist < 25 * gc_ai_ScanGridFactor then
      [*] = ;      myForce := myForce * 2;
      [*] = ;
      [*] = ;      // need to retreat
      [*] = ;      if IsInRangeInt(myZone, 0, TopologyGetZonesCount-1) and (not isArtOnly) and (enForce > 0) and (enForce > myForce) then
      [*] = ;      begin
      [*] = ;         //Log('Retreat! EnForce = ' + IntToStr(enForce) + ', MyForce = ' + IntToStr(myForce));
      [*] = ;         var tzCount : Integer = TopologyGetZonesCount;
      [*] = ;         for i := 0 to tzCount-1 do
      [*] = ;         TopologySetZoneNeedPathByIndex(i, false);
      [*] = ;
      [*] = ;         var danDist : Float = VectorDistance(myScanX, 0, myScanZ, maxForceX, 0, maxForceY);
      [*] = ;         for i := gPlayer[plInd].aiData.armyList.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pArmy1 : Pointer = gPlayer[plInd].aiData.armyList.Get(i);
      [*] = ;            if (pArmy1<>nil) then
      [*] = ;            begin
      [*] = ;               var scanX, scanZ : Integer;
      [*] = ;               _misc_PosToScanGridIndices(TArmy(pArmy1).fX, TArmy(pArmy1).fZ, scanX, scanZ);
      [*] = ;
      [*] = ;               if (TArmy(pArmy1).fTopZone >= 0) and ((scanX <> myScanX) or (scanZ <> myScanZ)) then
      [*] = ;               begin
      [*] = ;                  var dist : Float = VectorDistance(scanX, 0, scanZ, maxForceX, 0, maxForceY);
      [*] = ;                  if dist > danDist then
      [*] = ;                  TopologySetZoneNeedPathByIndex(TArmy(pArmy1).fTopZone, true);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         var cityTopZone : Integer = TopologyGetZoneIndex(gPlayer[plInd].aiData.centerX, gPlayer[plInd].aiData.centerZ);
      [*] = ;         if cityTopZone >= 0 then
      [*] = ;         TopologySetZoneNeedPathByIndex(cityTopZone, true);
      [*] = ;
      [*] = ;         TopologyGetPathToZone(myZone);
      [*] = ;
      [*] = ;         var minDist : Float = gc_MaxInt;
      [*] = ;         var finTopZone : Integer = -1;
      [*] = ;         // find top zone to retreat
      [*] = ;         for i := gPlayer[plInd].aiData.armyList.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pArmy1 : Pointer = gPlayer[plInd].aiData.armyList.Get(i);
      [*] = ;            if (pArmy1<>nil) then
      [*] = ;            begin
      [*] = ;               var topZone : Integer = TArmy(pArmy1).fTopZone;
      [*] = ;               if TopologyGetZoneNeedPathByIndex(topZone) then
      [*] = ;               begin
      [*] = ;                  var dist : Integer = TopologyGetZoneTrackPointsCountByIndex(topZone);
      [*] = ;                  if (topZone <> myZone) and (topZone <> cityTopZone) and (dist > 0) and (dist < minDist) then
      [*] = ;                  begin
      [*] = ;                     finTopZone := topZone;
      [*] = ;                     minDist := dist;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         // retreat to city center
      [*] = ;         if finTopZone < 0 then
      [*] = ;         finTopZone := cityTopZone;
      [*] = ;
      [*] = ;         if (finTopZone >= 0) and (finTopZone < TopologyGetZonesCount) and (finTopZone <> myZone) then
      [*] = ;         begin
      [*] = ;            var zoneX, zoneZ : Float;
      [*] = ;            TopologyGetZoneCenterByIndex(finTopZone, zoneX, zoneZ);
      [*] = ;            //Log('Army retreat final position is (' + FloatToStr(zoneX) + ', ' + FloatToStr(zoneZ) + ') zone center');
      [*] = ;            // experiment
      [*] = ;            var endX, endZ : Float;
      [*] = ;            if _misc_TopZonePathLerp(finTopZone, gc_ai_ApproachDist, true, endX, endZ) then
      [*] = ;            begin
      [*] = ;               var gridX, gridZ : Integer;
      [*] = ;               if (_misc_PosToScanGridIndices(endX, endZ, gridX, gridZ)) then
      [*] = ;               begin
      [*] = ;                  var delayCount : Integer = _ai_GetArmyDelayCount(pArmy);
      [*] = ;                  if (not isArt) or ((TEnemyInfo(pEnemyInfo).towerMap[gridX, gridZ]=0) or (delayCount=0)) then
      [*] = ;                  begin
      [*] = ;                     //Log('Army [' + IntToStr(TArmy(pArmy).fIndex) + '] retreat to (' + FloatToStr(endX) + ', ' + FloatToStr(endZ) + ') zone center');
      [*] = ;                     _ai_SetArmyPosition(pArmy, endX, endZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                     exit;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if active then
      [*] = ;      begin
      [*] = ;         var destZone : Integer = -1;
      [*] = ;
      [*] = ;         if isArtOnly then
      [*] = ;         begin
      [*] = ;            //destZone := _ai_GetArtilleryEnemyZone(pArmy, myX, myZ);
      [*] = ;            destZone := nearArmyZone;
      [*] = ;
      [*] = ;            if destZone >= 0 then
      [*] = ;            begin
      [*] = ;               var nextZone : Integer = _ai_GetNextZone(myZone, destZone, false, false);
      [*] = ;               if (nextZone >= 0) and (nextZone <> myZone) then
      [*] = ;               begin
      [*] = ;                  var nextX, nextZ : Float;
      [*] = ;                  TopologyGetZoneCenterByIndex(nextZone, nextX, nextZ);
      [*] = ;                  _ai_SetArmyPosition(pArmy, nextX, nextZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            var bTrgFound : Boolean;
      [*] = ;            var fastExit : Boolean;
      [*] = ;            var wallHnd : Integer;
      [*] = ;
      [*] = ;            // check for nearest target
      [*] = ;            if (TArmy(pArmy).fSpec = gc_ai_armyspec_grenadier) {or isArtOnly} then
      [*] = ;            begin
      [*] = ;               // TODO: Faust code:
      [*] = ;               {var nextZone : Integer = _ai_GetNextGreCell(plInd, pArmy);
      [*] = ;               if nextZone >= 0 then
      [*] = ;               begin
      [*] = ;                  var endX, endZ : Float;
      [*] = ;                  TopologyGetZoneCenterByIndex(nextZone, endX, endZ);
      [*] = ;                  _ai_SetArmyPosition(plHnd, pArmy, endX, endZ, 0, 0);
      [*] = ;                  exit;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  if findWall then
      [*] = ;                  destZone := _ai_FindNearestWall(pArmy);
      [*] = ;
      [*] = ;                  if destZone < 0 then
      [*] = ;                  begin
      [*] = ;                     destZone := _ai_FindNearestEnemyZone(plInd, 30, myX, myZ, gPlayer[plInd].aiData.defenceStage);
      [*] = ;                     if destZone < 0 then
      [*] = ;                     fastExit := true;
      [*] = ;                  end;
      [*] = ;               end;}
      [*] = ;               // TODO: Gec code (grenadiers = archers)
      [*] = ;               begin
      [*] = ;                  if findWall then
      [*] = ;                  begin
      [*] = ;                     _ai_FindNearestWall(pArmy, destZone, wallHnd);
      [*] = ;                     _unit_SetArmyOrderTargetExt(pArmy, wallHnd, destZone);
      [*] = ;                     //TArmy(pArmy).fOrder.iType := gc_ai_armyorder_attackwall;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if destZone < 0 then
      [*] = ;                  begin
      [*] = ;                     gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;                     for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;                     begin
      [*] = ;                        var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;                        if (pSquad<>nil) then
      [*] = ;                        for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;                        begin
      [*] = ;                           var goHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;                           gPlayer[plInd].aiData.aiList.Add(goHnd);
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;
      [*] = ;                     var greCount : Integer = gPlayer[plInd].aiData.aiList.GetCount;
      [*] = ;                     if greCount > 0 then
      [*] = ;                     begin
      [*] = ;                        var trgHnd : Integer = _ai_GetBestVictimForArchers(plInd, myZone, myX, myZ, 10);
      [*] = ;
      [*] = ;                        if trgHnd <> 0 then
      [*] = ;                        begin
      [*] = ;                           for i := gPlayer[plInd].aiData.aiList.GetCount-1 downto 0 do
      [*] = ;                           begin
      [*] = ;                              var goHnd : Integer = gPlayer[plInd].aiData.aiList.Get(i);
      [*] = ;                              var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                              if (pobj<>nil) then
      [*] = ;                              begin
      [*] = ;                                 TObj(pObj).bSearchEnemy := true;
      [*] = ;                                 _unit_OrderAttack(goHnd, trgHnd, true, true, True); // Need iLockTrg (last bool)
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;
      [*] = ;                           bTrgFound := True;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                     if (not bTrgFound) then
      [*] = ;                     begin
      [*] = ;                        var nextZone : Integer = _ai_GetNextGreCell(pArmy);
      [*] = ;                        if (nextZone >= 0) then
      [*] = ;                        begin
      [*] = ;                           var endX, endZ : Float;
      [*] = ;                           TopologyGetZoneCenterByIndex(nextZone, endX, endZ);
      [*] = ;                           _ai_SetArmyPosition(pArmy, endX, endZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                           exit;
      [*] = ;                        end;
      [*] = ;
      [*] = ;                        destZone := _ai_FindNearestEnemyZone(plInd, 30, myX, myZ, gPlayer[plInd].aiData.defenceStage);
      [*] = ;                        if destZone < 0 then
      [*] = ;                        fastExit := true;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               // army is not grenadiers
      [*] = ;               destZone := _ai_FindNearestEnemyZone(plInd, 30, myX, myZ, gPlayer[plInd].aiData.defenceStage);
      [*] = ;               if destZone < 0 then
      [*] = ;               begin
      [*] = ;                  if findWall then
      [*] = ;                  begin
      [*] = ;                     _ai_FindNearestWall(pArmy, destZone, wallHnd);
      [*] = ;                     _unit_SetArmyOrderTargetExt(pArmy, wallHnd, destZone);
      [*] = ;                     //TArmy(pArmy).fOrder.iType := gc_ai_armyorder_attackwall;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if destZone < 0 then
      [*] = ;                  fastExit := true;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            if fastExit then
      [*] = ;            begin
      [*] = ;               if (not isCannon) and (not isMortar) then
      [*] = ;               begin
      [*] = ;                  var dan : Float = _ai_GetArmyDanger(pArmy, TArmy(pArmy).fCurX, TArmy(pArmy).fCurZ, 1, 0);
      [*] = ;                  if dan > 2 then
      [*] = ;                  _ai_SendToMostSafePosition(pArmy);
      [*] = ;               end;
      [*] = ;
      [*] = ;               exit;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            if (not bTrgFound) then
      [*] = ;            begin
      [*] = ;               if destZone < 0 then
      [*] = ;               _ai_ArmyClearOrder(pArmy)
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  var nextZone : Integer = _ai_GetNextZone(myZone, destZone, false, false);
      [*] = ;                  if nextZone < 0 then
      [*] = ;                  begin
      [*] = ;                     if (not isCannon) and (not isMortar) then
      [*] = ;                     begin
      [*] = ;                        var dan : Float = _ai_GetArmyDanger(pArmy, TArmy(pArmy).fCurX, TArmy(pArmy).fCurZ, 1, 0);
      [*] = ;                        if dan > 2 then
      [*] = ;                        _ai_SendToMostSafePosition(pArmy);
      [*] = ;                     end;
      [*] = ;
      [*] = ;                     exit;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     // in original cossacks there was a check for next zone danger, but due to mistake in GetTopDanger function
      [*] = ;                     // this condition was always false
      [*] = ;                     {var dang : Integer = _ai_GetTopDanger(plInd, nextZone, enDist);
      [*] = ;                     if _ai_GetTopDanger(plInd, nextZone, enDist) > myForce then
      [*] = ;                     begin
      [*] = ;                        if nearArmyZone >= 0 then
      [*] = ;                        begin
      [*] = ;                           if nextZone <> nearArmyZone then
      [*] = ;                           nextZone := _ai_GetNextZone(myZone, nearArmyZone);
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        exit;
      [*] = ;                     end;}
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if (not isCannon) and (not isMortar) then
      [*] = ;                  begin
      [*] = ;                     var dan : Float = _ai_GetArmyDanger(pArmy, TArmy(pArmy).fCurX, TArmy(pArmy).fCurZ, 1, 0);
      [*] = ;                     if dan > 1 then
      [*] = ;                     begin
      [*] = ;                        _ai_SendToMostSafePosition(pArmy);
      [*] = ;                        exit;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if nextZone >= 0 then
      [*] = ;                  begin
      [*] = ;                     if nextZone <> myZone then
      [*] = ;                     begin
      [*] = ;                        var nextX, nextZ : Float;
      [*] = ;                        TopologyGetZoneCenterByIndex(nextZone, nextX, nextZ);
      [*] = ;
      [*] = ;                        var dan : Float = _ai_GetArmyDanger(pArmy, nextX, nextZ, 0, 0);
      [*] = ;                        if (not isCannon) and (not isMortar) and (dan > 2) then
      [*] = ;                        begin
      [*] = ;                           if RandomExt < 4096/32768 then
      [*] = ;                           _ai_ArmyAlign(pArmy);
      [*] = ;
      [*] = ;                           exit;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        _ai_SetArmyPosition(pArmy, nextX, nextZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     if wallHnd <> 0 then
      [*] = ;                     begin
      [*] = ;                        var x, y, z : Float;
      [*] = ;                        GetGameObjectAbsolutePositionByHandle(wallHnd, x, y, z);
      [*] = ;                        gIntegerList.Clear;
      [*] = ;                        _player_OrderUnitsToAttack(plInd, wallHnd, gIntegerList, TArmy(pArmy).fSquadList, true, false);
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyMakeWaterBattleLink
      [*] = ;//
      [*] = ;procedure _ai_ArmyMakeWaterBattleLink(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var i, army : Integer;
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var myZone : Integer = TArmy(pArmy).fTopZone;
      [*] = ;      if (myZone >= 0) and _ai_ArmyCanMerge(pArmy) then
      [*] = ;      begin
      [*] = ;         for army := gPlayer[plInd].aiData.armyList.GetCount-1 downto 0 do
      [*] = ;         if army <> TArmy(pArmy).fIndex then
      [*] = ;         begin
      [*] = ;            var pArmy1 : Pointer = gPlayer[plInd].aiData.armyList.Get(army);
      [*] = ;            if (pArmy1<>nil) and (TArmy(pArmy).fSpec = TArmy(pArmy1).fSpec) and (TArmy(pArmy1).fTopZone >= 0) and _ai_ArmyCanMerge(pArmy1) then
      [*] = ;            begin
      [*] = ;               var dist : Integer = TopologyGetZonesGraphDist(myZone, TArmy(pArmy1).fTopZone, gc_ai_MergeWaterArmyDist);
      [*] = ;               if dist >= 0 then
      [*] = ;               begin
      [*] = ;                  _ai_ArmyJoin(pArmy, pArmy1, true);
      [*] = ;                  exit;
      [*] = ;               end
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      // attack behaviour
      [*] = ;      var myX : Float = TArmy(pArmy).fX;
      [*] = ;      var myZ : Float = TArmy(pArmy).fZ;
      [*] = ;
      [*] = ;      var destZone : Integer = _ai_FindNearestWaterEnemyZone(plInd, 30, myX, myZ);
      [*] = ;      if destZone < 0 then
      [*] = ;      destZone := _ai_FindNearestWaterEnemyZone(plInd, 0, myX, myZ);
      [*] = ;
      [*] = ;      if destZone < 0 then
      [*] = ;      _ai_ArmyClearOrder(pArmy)
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         var nextZone : Integer = _ai_GetNextZone(myZone, destZone, true, false);
      [*] = ;         if (nextZone >= 0) and (nextZone <> myZone) then
      [*] = ;         begin
      [*] = ;            var nextX, nextZ : Float;
      [*] = ;            TopologyGetZoneCenterByIndex(nextZone, nextX, nextZ);
      [*] = ;            _ai_SetArmyPosition(pArmy, nextX, nextZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyMakeBattleLink
      [*] = ;//
      [*] = ;procedure _ai_ArmyMakeBattleLink(pArmy : Pointer; active, findWall : Boolean);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) and (TArmy(pArmy).fSquadList.GetCount > 0) then
      [*] = ;   begin
      [*] = ;      if _ai_IsWaterArmy(pArmy) then
      [*] = ;      _ai_ArmyMakeWaterBattleLink(pArmy)
      [*] = ;      else
      [*] = ;      _ai_ArmyMakeLandBattleLink(pArmy, active, findWall);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyDiversia
      [*] = ;//
      [*] = ;procedure _ai_ArmyDiversia(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      TArmy(pArmy).fOrder.iType := gc_ai_armyorder_sabotage;
      [*] = ;      TArmy(pArmy).fSpecialOrder := true;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyDiversiaLink
      [*] = ;//
      [*] = ;procedure _ai_ArmyDiversiaLink(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var pEnemyInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;      if gbool_peacemode or (pEnemyInfo = nil) or (TArmy(pArmy).fSquadList.GetCount = 0) then
      [*] = ;      exit;
      [*] = ;
      [*] = ;      _ai_ArmyAlign(pArmy);
      [*] = ;      var myZone : Integer = TArmy(pArmy).fTopZone;
      [*] = ;      if IsInRangeInt(myZone, 0, TopologyGetZonesCount-1) then
      [*] = ;      begin
      [*] = ;         var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(0);
      [*] = ;         if (pSquad<>nil) then
      [*] = ;         begin
      [*] = ;            var myForce : Integer = TSquad(pSquad).GetCount;
      [*] = ;            var fear : Boolean;
      [*] = ;            var nearDist : Float = gc_MaxInt;
      [*] = ;            var enX, enY : Integer;
      [*] = ;
      [*] = ;            // search for enemy army
      [*] = ;            var i : Integer;
      [*] = ;            for i := TEnemyInfo(pEnemyInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var pAInfo : Pointer = TEnemyInfo(pEnemyInfo).armyInfos.Get(i);
      [*] = ;               if (pAInfo<>nil) and (TArmyInfo(pAInfo).landCount > 0) then
      [*] = ;               begin
      [*] = ;                  var enForce : Integer = TArmyInfo(pAInfo).cavalry {+ TArmyInfo(pAInfo).mortars} + TArmyInfo(pAInfo).infantry + TArmyInfo(pAInfo).shooters;
      [*] = ;                  var enIndX : Float = (TArmyInfo(pAInfo).minX + TArmyInfo(pAInfo).maxX) / 2;
      [*] = ;                  var enIndY : Float = (TArmyInfo(pAInfo).minY + TArmyInfo(pAInfo).maxY) / 2;
      [*] = ;                  var x, y : Float;
      [*] = ;                  _misc_ScanGridIndicesToPos(enIndX, enIndY, x, y);
      [*] = ;                  var dist : Float = VectorDistance(TArmy(pArmy).fCurX, 0, TArmy(pArmy).fCurZ, x, 0, y);
      [*] = ;                  if (dist < 30) and (enForce > myForce) then
      [*] = ;                  fear := true
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     if (dist < 400) and (enForce <= myForce) and (dist < nearDist) then
      [*] = ;                     begin
      [*] = ;                        nearDist := dist;
      [*] = ;                        enX := Floor(enIndX);
      [*] = ;                        enY := Floor(enIndY);
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            var enHnd : Integer;
      [*] = ;            var orEnHnd : Integer;
      [*] = ;            if (not fear) and (nearDist < gc_MaxInt) then
      [*] = ;            begin
      [*] = ;               var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(0);
      [*] = ;               if (pSquad <> nil) and (TSquad(pSquad).GetCount > 0) then
      [*] = ;               begin
      [*] = ;                  var goHnd : Integer = TSquad(pSquad).Get(0);
      [*] = ;                  enHnd := _unit_SearchEnemyInCell(goHnd, enX, enY, 4);
      [*] = ;                  orEnHnd := enHnd;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            if enHnd <> 0 then
      [*] = ;            begin
      [*] = ;               var pEnObj : Pointer = _unit_GetTObj(enHnd);
      [*] = ;               if (pEnObj <> nil) and (not TObj(pEnObj).bDead) then
      [*] = ;               begin
      [*] = ;                  var enX : Float = GetGameObjectPositionXByHandle(enHnd);
      [*] = ;                  var enZ : Float = GetGameObjectPositionZByHandle(enHnd);
      [*] = ;                  var enZone : Integer = TopologyGetZoneIndex(enX, enZ);
      [*] = ;                  if enZone >= 0 then
      [*] = ;                  begin
      [*] = ;                     var nextZone : Integer;
      [*] = ;                     if enZone = myZone then
      [*] = ;                     nextZone := enZone
      [*] = ;                     else
      [*] = ;                     nextZone := _ai_GetNextSafeZone(plHnd, myZone, enZone);
      [*] = ;
      [*] = ;                     if nextZone = myZone then
      [*] = ;                     begin
      [*] = ;                        // TODO: check object for capture
      [*] = ;                        //var pEnObjBase : Pointer = gPlayer[TObj(pEnObj).pl].objBase[TObj(pEnObj).cid][TObj(pEnObj).id];
      [*] = ;                        //if TObjBase(pEnObjBase).bCapture {and check_capture_function} then
      [*] = ;                        // else
      [*] = ;                        _ai_ArmyBitvaLinkExt(pArmy, true, enX, enZ);
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     begin
      [*] = ;                        var nextX, nextZ : Float;
      [*] = ;                        TopologyGetZoneCenterByIndex(nextZone, nextX, nextZ);
      [*] = ;                        _ai_SetArmyPosition(pArmy, nextX, nextZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               orEnHnd := 0;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            orEnHnd := 0;
      [*] = ;
      [*] = ;            if orEnHnd = 0 then
      [*] = ;            begin
      [*] = ;               var minDist : Integer = gc_MaxInt;
      [*] = ;               var aInd : Integer = -1;
      [*] = ;               var enZone : Integer = -1;
      [*] = ;               var i : Integer;
      [*] = ;               for i := TEnemyInfo(pEnemyInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var pAInfo : Pointer = TEnemyInfo(pEnemyInfo).armyInfos.Get(i);
      [*] = ;                  if (pAInfo<>nil) and (TArmyInfo(pAInfo).landCount > 0) then
      [*] = ;                  begin
      [*] = ;                     var enForce : Integer = TArmyInfo(pAInfo).cavalry + TArmyInfo(pAInfo).infantry + TArmyInfo(pAInfo).shooters;
      [*] = ;                     var enIndX : Float = (TArmyInfo(pAInfo).minX + TArmyInfo(pAInfo).maxX) / 2;
      [*] = ;                     var enIndY : Float = (TArmyInfo(pAInfo).minY + TArmyInfo(pAInfo).maxY) / 2;
      [*] = ;                     var x, y : Float;
      [*] = ;                     _misc_ScanGridIndicesToPos(enIndX, enIndY, x, y);
      [*] = ;                     var dist : Float = VectorDistance(TArmy(pArmy).fCurX, 0, TArmy(pArmy).fCurZ, x, 0, y);
      [*] = ;                     if (enForce > 6) and (dist < 40) and (dist < minDist) then
      [*] = ;                     begin
      [*] = ;                        dist := minDist;
      [*] = ;                        aInd := i;
      [*] = ;                        enZone := TopologyGetZoneIndex(x, y);
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;
      [*] = ;               if (aInd >= 0) and (enZone >= 0) then
      [*] = ;               begin
      [*] = ;                  // retreat to safe position
      [*] = ;                  for i := TopologyGetZonesCount-1 downto 0 do
      [*] = ;                  TopologySetZoneNeedPathByIndex(i, false);
      [*] = ;
      [*] = ;                  for i := TopologyGetZoneNeighboursCountByIndex(myZone)-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     var neiZone : Integer = TopologyGetZoneNeighbourByIndex(myZone, i);
      [*] = ;                     TopologySetZoneNeedPathByIndex(neiZone, true);
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  TopologyGetPathToZone(enZone);
      [*] = ;
      [*] = ;                  var len : Integer = -1;
      [*] = ;                  var zone : Integer = -1;
      [*] = ;                  for i := TopologyGetZoneNeighboursCountByIndex(myZone)-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     var neiZone : Integer = TopologyGetZoneNeighbourByIndex(myZone, i);
      [*] = ;                     var tpCount : Integer = TopologyGetZoneTrackPointsCountByIndex(neiZone);
      [*] = ;                     if tpCount > len then
      [*] = ;                     begin
      [*] = ;                        len := tpCount;
      [*] = ;                        zone := neiZone;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  if zone >= 0 then
      [*] = ;                  begin
      [*] = ;                     var nextZone : Integer = _ai_GetNextZone(myZone, zone, false, false);
      [*] = ;                     if nextZone >= 0 then
      [*] = ;                     begin
      [*] = ;                        var nextX, nextZ : Float;
      [*] = ;                        TopologyGetZoneCenterByIndex(nextZone, nextX, nextZ);
      [*] = ;                        _ai_SetArmyPosition(pArmy, nextX, nextZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_CheckDiversants
      [*] = ;//
      [*] = ;procedure _ai_CheckDiversants(plInd : Integer);
      [*] = ;begin
      [*] = ;   var count : Integer;
      [*] = ;   var i : Integer;
      [*] = ;   // TODO: not finished. When water will appear need to modify code
      [*] = ;   //Brigade* AGRESS=&CT->Agressors;
      [*] = ;   //if(CT->MyIsland!=CITY[0].MyIsland)AGRESS=&CT->Settlers;
      [*] = ;
      [*] = ;   for i := gPlayer[plInd].aiData.agressors.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var goHnd : Integer = gPlayer[plInd].aiData.agressors.Get(i);
      [*] = ;      if _unit_GetTagStateByType(goHnd, gc_statetag_essential) = gc_statetag_essential_none then
      [*] = ;      begin
      [*] = ;         var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;         if (pObjProp<>nil) and (TObjProp(pObjProp).usage = gc_obj_usage_fasthorse) then
      [*] = ;         count := count+1;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   var aCount : Integer = _ai_GetDiverArmiesCount(plInd);
      [*] = ;   if (aCount < gc_ai_MaxDiverArmies) and (count > gAIConst.maxDivers[aCount]) then
      [*] = ;   begin
      [*] = ;      var pArmy : Pointer = _ai_CreateMinimalArmy(plInd, gc_ai_armytype_fasthorse, 0, gPlayer[plInd].aiData.agressors, true, true);
      [*] = ;      if (pArmy<>nil) then
      [*] = ;      begin
      [*] = ;         TArmy(pArmy).fSpec := gc_ai_armyspec_sabotage;
      [*] = ;         _ai_ArmyDiversia(pArmy);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_EnemyDistToCenter
      [*] = ;//
      [*] = ;function _ai_EnemyDistToCenter(plInd : Integer) : Float;
      [*] = ;begin
      [*] = ;   var pEnemyInfo : Pointer = _misc_GetPlayerEnemyInfo(plInd);
      [*] = ;   var minDist : Float = -1;
      [*] = ;   if pEnemyInfo <> nil then
      [*] = ;   begin
      [*] = ;      var i : Integer;
      [*] = ;      for i := TEnemyInfo(pEnemyInfo).armyInfos.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pArmyInfo : Pointer = TEnemyInfo(pEnemyInfo).armyInfos.Get(i);
      [*] = ;         if (pArmyInfo<>nil) and (TArmyInfo(pArmyInfo).landCount > 0) then
      [*] = ;         begin
      [*] = ;            var gridX : Float = (TArmyInfo(pArmyInfo).minX + TArmyInfo(pArmyInfo).maxX) / 2;
      [*] = ;            var gridY : Float = (TArmyInfo(pArmyInfo).minY + TArmyInfo(pArmyInfo).maxY) / 2;
      [*] = ;            var x, y : Float;
      [*] = ;            _misc_ScanGridIndicesToPos(gridX, gridY, x, y);
      [*] = ;            var dist : Float = VectorDistance(x, 0, y, gPlayer[plInd].aiData.centerX, 0, gPlayer[plInd].aiData.centerZ);
      [*] = ;            if (minDist < 0) or (dist < minDist) then
      [*] = ;            minDist := dist;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   Result := minDist;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_SmartGamer
      [*] = ;//
      [*] = ;procedure _ai_SmartGamer(plInd : Integer);
      [*] = ;begin
      [*] = ;   const cMobilizationMinDist = 50;
      [*] = ;   var nearestEnemyDist : Float = _ai_EnemyDistToCenter(plInd);
      [*] = ;   if (nearestEnemyDist>=0) and (nearestEnemyDist<cMobilizationMinDist) then
      [*] = ;   begin
      [*] = ;      var i : Integer;
      [*] = ;      for i:=gPlayer[plInd].aiData.freewarriors.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = gPlayer[plInd].aiData.freewarriors.Get(i);
      [*] = ;         if (goHnd<>0) then
      [*] = ;         gPlayer[plInd].aiData.agressors.Add(goHnd);
      [*] = ;         gPlayer[plInd].aiData.freewarriors.Delete(i);
      [*] = ;      end;
      [*] = ;      for i:=gPlayer[plInd].aiData.defenders.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = gPlayer[plInd].aiData.defenders.Get(i);
      [*] = ;         if (goHnd<>0) then
      [*] = ;         gPlayer[plInd].aiData.agressors.Add(goHnd);
      [*] = ;         gPlayer[plInd].aiData.defenders.Delete(i);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if gPlayer[plInd].aiData.landBattle then
      [*] = ;   begin
      [*] = ;      var armyType : Integer = _ai_CheckMinArmyCreationAbility(plInd, gPlayer[plInd].aiData.agressors, true, true, true);
      [*] = ;      if armyType <> gc_ai_armytype_none then
      [*] = ;      begin
      [*] = ;         var pArmy : Pointer = _ai_CreateMinimalArmy(plInd, armyType, 0, gPlayer[plInd].aiData.agressors, true, true);
      [*] = ;         if (pArmy<>nil) then
      [*] = ;         begin
      [*] = ;            _ai_ArmyAlign(pArmy);
      [*] = ;            _ai_ArmyMakeBattle(pArmy);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if gPlayer[plInd].aiData.waterBattle then
      [*] = ;   begin
      [*] = ;      _ai_CreateWaterArmies(plInd);
      [*] = ;   end;
      [*] = ;
      [*] = ;   if gPlayer[plInd].aiData.makeDiversion then
      [*] = ;   _ai_CheckDiversants(plInd);
      [*] = ;end;
      [*] = ;
      [*] = ;//_ai_GetArmyDistanceToObject
      [*] = ;//
      [*] = ;function _ai_GetArmyDistanceToObject(pArmy : Pointer; targetHnd : Integer; var armyposx : Float; var armyposz : Float) : Float;
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var aposx, aposz : Float;
      [*] = ;      var unitCount : Integer;
      [*] = ;      var targetx : Float = GetGameObjectPositionXByHandle(targetHnd);
      [*] = ;      var targetz : Float = GetGameObjectPositionZByHandle(targetHnd);
      [*] = ;      var i, j : Integer;
      [*] = ;      for i:=TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;         if pSquad<>nil then
      [*] = ;         for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var unitHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;            aposx := aposx + GetGameObjectPositionXByHandle(unitHnd);
      [*] = ;            aposz := aposz + GetGameObjectPositionZByHandle(unitHnd);
      [*] = ;            unitCount := unitCount+1;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      if unitCount>0 then
      [*] = ;      begin
      [*] = ;         aposx := aposx/unitCount;
      [*] = ;         aposz := aposz/unitCount;
      [*] = ;      end;
      [*] = ;      armyposx := aposx;
      [*] = ;      armyposz := aposz;
      [*] = ;      Result := VectorDistance(targetx, 0, targetz, aposx, 0, aposz);
      [*] = ;   end
      [*] = ;   else
      [*] = ;   Result := 0;
      [*] = ;   //log('Army dist: '+FloatToStr(result));
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_DisbandMineSettlers
      [*] = ;//
      [*] = ;procedure _ai_DisbandMineSettlers(plind : Integer; pArmy : Pointer; updateLists : boolean);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var i, j : Integer;
      [*] = ;      var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;      if updateLists then
      [*] = ;      begin
      [*] = ;         gIntegerList.Clear;
      [*] = ;         for i:= TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;            if pSquad<>nil then
      [*] = ;            for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var unitHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;               var pObj : Pointer = _unit_GetTObj(unitHnd);
      [*] = ;               if (pobj<>nil) and (gObjProp[TObj(pObj).cid][TObj(pObj).id].usage <> gc_obj_usage_peasant) then
      [*] = ;               begin
      [*] = ;                  gIntegerList.Add(unitHnd);
      [*] = ;                  gPlayer[plind].aidata.freewarriors.Add(unitHnd);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         //give order go home:
      [*] = ;         if (gIntegerList.GetCount>0) then
      [*] = ;         //_player_OrderSelectedUnitsToAttackPoint(plHnd, gIntegerList, gPlayer[plind].aidata.centerx, gPlayer[plind].aidata.centerz, true);
      [*] = ;         _player_SetUnitsPosition(plInd, gc_obj_order_move_mode_attack, gIntegerList, gPlayer[plind].aidata.centerx, gPlayer[plind].aidata.centerz, 0, 0);
      [*] = ;      end;
      [*] = ;      //disbanding:
      [*] = ;      var sqCount : Integer = TArmy(pArmy).fSquadList.GetCount;
      [*] = ;      for i := 0 to sqCount-1 do
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;         if (pSquad<>nil) then
      [*] = ;         begin
      [*] = ;            var sqInd : Integer = TSquad(pSquad).fIndex;
      [*] = ;            _misc_DisbandSquad(plHnd, sqInd, true);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ProgressMineBuilders
      [*] = ;//
      [*] = ;procedure _ai_ProgressMineBuilders(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   //checking can build first:
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var oreUID : Integer = TArmy(pArmy).fOrder.targetUID;
      [*] = ;      if (oreUID<>0) then
      [*] = ;      begin
      [*] = ;         var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;         var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;         var oreHnd : Integer = GetGameObjectHandleByUniqueId(oreUID);
      [*] = ;         if (oreHnd<>0) then
      [*] = ;         begin
      [*] = ;            var mineX : Float = GetGameObjectPositionXByHandle(oreHnd);
      [*] = ;            var mineZ : Float = GetGameObjectPositionZByHandle(oreHnd);
      [*] = ;            var armyposx, armyposz : Float;
      [*] = ;            var pres : Pointer = _res_GetTRes(oreHnd);
      [*] = ;            if pres=nil then
      [*] = ;            _ai_DisbandMineSettlers(plind, pArmy, true)
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               var resType : Integer = TRes(pres).itype;
      [*] = ;               if _ai_GetArmyDistanceToObject(pArmy, oreHnd, armyposx, armyposz)>5 then
      [*] = ;               begin
      [*] = ;                  var stZone, endZone : Integer;
      [*] = ;                  stZone := TopologyGetZoneIndex(armyposx, armyposz);
      [*] = ;                  endZone := TopologyGetZoneIndex(mineX, mineZ);
      [*] = ;                  if TopologyGetZonesPath(stZone, endZone) then
      [*] = ;                  begin
      [*] = ;                     var endX, endZ : Float;
      [*] = ;                     if _misc_TopZonePathLerp(endZone, 20, true, endX, endZ) then
      [*] = ;                     _ai_SetArmyPosition(pArmy, endX, endZ, 0, 0, gc_obj_order_move_mode_default)
      [*] = ;                     else
      [*] = ;                     _ai_SetArmyPosition(pArmy, mineX-1, mineZ-1, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     var newOreHnd : Integer = _ai_GetClosestOrePlaceWithRes(plind, restype);
      [*] = ;                     if (newOreHnd<>0) then
      [*] = ;                     _unit_SetArmyOrderTarget(pArmy, newOreHnd)
      [*] = ;                     else
      [*] = ;                     //no more mines, go home...
      [*] = ;                     //gPlayer[plind].aiData.armyList.delete(TArmy(pArmy).fIndex);
      [*] = ;                     _ai_DisbandMineSettlers(plind, pArmy, true);
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  var isFree : Boolean = True;
      [*] = ;                  _misc_MakeListObjectsInRadius(minex, minez, 1, false, (1 shl gc_obj_material_building), 0, True, gIntegerList);
      [*] = ;                  var i, j : Integer;
      [*] = ;                  for i:=gIntegerList.GetCount-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     var bldHnd : Integer = gIntegerList.Get(i);
      [*] = ;                     var pobj : Pointer = _unit_GetTObj(bldHnd);
      [*] = ;                     if (pobj<>nil) and (gObjProp[TObj(pobj).cid][TObj(pobj).id].usage=gc_obj_usage_mine) then
      [*] = ;                     begin
      [*] = ;                        //pos check
      [*] = ;                        var bldX : Float = GetGameObjectPositionXByHandle(bldHnd);
      [*] = ;                        var bldZ : Float = GetGameObjectPositionZByHandle(bldHnd);
      [*] = ;                        if VectorDistance(minex, 0, minez, bldX, 0, bldZ) < gc_epsilon+0.5 then
      [*] = ;                        begin
      [*] = ;                           isFree := False;
      [*] = ;                           break;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;                  if (not isFree) then
      [*] = ;                  begin
      [*] = ;                     //looking for a new mine:
      [*] = ;                     var newOreHnd : Integer = _ai_GetClosestOrePlaceWithRes(plind, restype);
      [*] = ;                     if (newOreHnd<>0) then
      [*] = ;                     begin
      [*] = ;                        _unit_SetArmyOrderTarget(pArmy, newOreHnd);
      [*] = ;                        //var posx : Float = GetGameObjectPositionXByHandle(newOreHnd);
      [*] = ;                        //var posz : Float = GetGameObjectPositionZByHandle(newOreHnd);
      [*] = ;                        //_ai_SetArmyPosition(plHnd, pArmy, posX, posZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     begin
      [*] = ;                        //no more mines, go home...
      [*] = ;                        //gPlayer[plind].aiData.armyList.delete(TArmy(pArmy).fIndex);
      [*] = ;                        _ai_DisbandMineSettlers(plind, pArmy, true);
      [*] = ;                     end;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     var index : Integer = _ai_GetFreeBuildingIndex(plind);
      [*] = ;                     if (index<>-1) then
      [*] = ;                     begin
      [*] = ;                        var cid : Integer = TArmy(pArmy).fOrder.tag;
      [*] = ;                        var id : Integer = TArmy(pArmy).fOrder.tag2;
      [*] = ;                        var miscPlHnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;                        var sid : String = gObjProp[cid][id].sid;
      [*] = ;                        var posx : Float = GetGameObjectPositionXByHandle(oreHnd);
      [*] = ;                        var posz : Float = GetGameObjectPositionZByHandle(oreHnd);
      [*] = ;                        var goHnd : Integer = _player_CreateConstructionDummyBySID(miscPlHnd, cid, sid, posx, posz);
      [*] = ;                        if (goHnd<>0) then
      [*] = ;                        begin
      [*] = ;                           var canplace : Boolean = _misc_CanPlaceBuilding(goHnd, plind, gObjProp[cid][id].usage, posx, posz, 0, 0.26, 0, -1, True);
      [*] = ;                           GameObjectDestroyByHandle(goHnd);
      [*] = ;                           if (canplace) then
      [*] = ;                           begin
      [*] = ;                              gPlayer[plind].aidata.ailist.Clear;
      [*] = ;                              for i:=TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;                              begin
      [*] = ;                                 var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;                                 if pSquad<>nil then
      [*] = ;                                 for j:=TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;                                 begin
      [*] = ;                                    var unitHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;                                    var pObj : Pointer = _unit_GetTObj(unitHnd);
      [*] = ;                                    if (pObj<>nil) and (gObjProp[TObj(pObj).cid][TObj(pObj).id].usage=gc_obj_usage_peasant) then
      [*] = ;                                    gPlayer[plind].aidata.ailist.add(unitHnd);
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                              var trgHnd : Integer = _player_ConstructBuildingList(plHnd, cid, sid, posx, posz, gPlayer[plind].aidata.ailist, false, True);
      [*] = ;                              //log('targetHnd='+inttostr(trghnd)+' sid='+sid+' cid:'+inttostr(cid)+' id:'+inttostr(id)+ ' plInd:'+inttostr(plind));
      [*] = ;                              if (trgHnd<>0) then
      [*] = ;                              begin
      [*] = ;                                 //sending defenders and removing army:
      [*] = ;                                 var pDefBuilding : Pointer = gPlayer[plInd].aiData.defbuildings.GetByHandle(trgHnd);
      [*] = ;                                 gIntegerList.Clear;
      [*] = ;                                 for i:= TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;                                 begin
      [*] = ;                                    var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;                                    if pSquad<>nil then
      [*] = ;                                    for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;                                    begin
      [*] = ;                                       var unitHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;                                       var pObj : Pointer = _unit_GetTObj(unitHnd);
      [*] = ;                                       if (pObj<>nil) and (gObjProp[TObj(pObj).cid][TObj(pObj).id].usage <> gc_obj_usage_peasant) and (not gObjProp[TObj(pObj).cid][TObj(pObj).id].bmercenary) then
      [*] = ;                                       begin
      [*] = ;                                          if (pDefBuilding<>nil) then
      [*] = ;                                          begin
      [*] = ;                                             gPlayer[plind].aidata.guards.Add(unitHnd);
      [*] = ;                                             TAiProtectedBuilding(pDefBuilding).defenders.Add(unitHnd);
      [*] = ;                                             gIntegerList.Add(unitHnd);
      [*] = ;                                          end
      [*] = ;                                          else
      [*] = ;                                          ErrorLog('ProgressWarAI : pDefBuilding=nil');
      [*] = ;                                       end;
      [*] = ;                                    end;
      [*] = ;                                 end;
      [*] = ;                                 if (gIntegerList.GetCount>0) then
      [*] = ;                                 _player_OrderUnitsToGuard(plHnd, gIntegerList, trgHnd, True);
      [*] = ;                                 //removing army:
      [*] = ;                                 _ai_DisbandMineSettlers(plind, pArmy, false);
      [*] = ;                                 gPlayer[plind].aidata.buildprojects[index].SetupBuildingProject(cid, id, sid, posx, posz, 0);
      [*] = ;                                 gPlayer[plind].aidata.buildprojects[index].gohnd := trgHnd;
      [*] = ;                                 gPlayer[plind].aidata.buildprojects[index].placefound := true;
      [*] = ;                                 gPlayer[plind].aidata.buildprojects[index].minpeasants := 5;
      [*] = ;                                 gPlayer[plind].aidata.buildprojects[index].maxpeasants := 5;
      [*] = ;                                 gPlayer[plind].aidata.unbuildhouses.Add(trgHnd);
      [*] = ;                                 if gPlayer[plind].aidata.ailist.GetCount>0 then
      [*] = ;                                 begin
      [*] = ;                                    gPlayer[plind].aidata.buildprojects[index].peasantscalled := True;
      [*] = ;                                    gPlayer[plind].aidata.buildprojects[index].npeasantscalled := gPlayer[plind].aidata.ailist.GetCount;
      [*] = ;                                 end;
      [*] = ;                              end
      [*] = ;                              else
      [*] = ;                              begin
      [*] = ;                                 var mmask : Integer = (1 shl gc_obj_material_body) or (1 shl gc_obj_material_iron);
      [*] = ;                                 _misc_MakeListObjectsInRadius(posx, posz, 3, true, mmask, 0, True, gIntegerList);
      [*] = ;                                 var i : Integer;
      [*] = ;                                 for i:=gIntegerList.GetCount-1 downto 0 do
      [*] = ;                                 begin
      [*] = ;                                    var unitHnd : Integer = gIntegerList.Get(i);
      [*] = ;                                    var pobj : Pointer = _unit_GetTObj(unitHnd);
      [*] = ;                                    if (pobj<>nil) then
      [*] = ;                                    begin
      [*] = ;                                       if (TObj(pobj).pl=plind) and (not _unit_IsRunAway(unitHnd)) then
      [*] = ;                                       _unit_DoRunAway(unitHnd, posx, posz);
      [*] = ;                                    end;
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                           end
      [*] = ;                           else
      [*] = ;                           begin
      [*] = ;                              var mmask : Integer = (1 shl gc_obj_material_body) or (1 shl gc_obj_material_iron);
      [*] = ;                              _misc_MakeListObjectsInRadius(posx, posz, 3, true, mmask, 0, True, gIntegerList);
      [*] = ;                              var i : Integer;
      [*] = ;                              for i:=gIntegerList.GetCount-1 downto 0 do
      [*] = ;                              begin
      [*] = ;                                 var unitHnd : Integer = gIntegerList.Get(i);
      [*] = ;                                 var pobj : Pointer = _unit_GetTObj(unitHnd);
      [*] = ;                                 if (pobj<>nil) then
      [*] = ;                                 begin
      [*] = ;                                    if (TObj(pobj).pl=plind) and (not _unit_IsRunAway(unitHnd)) then
      [*] = ;                                    _unit_DoRunAway(unitHnd, posx, posz);
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         //_unit_SetArmyOrderTarget(pArmy, 0);
      [*] = ;         _ai_DisbandMineSettlers(plind, pArmy, true); //mine is deleted, disband settlers
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ProgressDefenders
      [*] = ;//
      [*] = ;procedure _ai_ProgressDefenders(plInd : Integer);
      [*] = ;begin
      [*] = ;   //if ((gPlayer[plind].aidata.defenders.GetCount>0) or ((gPlayer[plind].aidata.freewarriors.GetCount>5) and (gPlayer[plind].aidata.guards.GetCount<gc_ai_max_guards)) then
      [*] = ;   begin
      [*] = ;      var i : Integer;
      [*] = ;      var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;      //getting most undefended building
      [*] = ;      var buildIndex : Integer = -1;
      [*] = ;      var minStr : Integer;
      [*] = ;      gPlayer[plind].aidata.guardsCount := 0;
      [*] = ;      gPlayer[plind].aidata.guardsTotal := 0;
      [*] = ;      for i:=0 to gPlayer[plind].aidata.defbuildings.GetCount-1 do
      [*] = ;      begin
      [*] = ;         var pDefBuilding : Pointer = gPlayer[plind].aidata.defbuildings.Get(i);
      [*] = ;         if (pDefBuilding<>nil) then
      [*] = ;         begin
      [*] = ;            var curStr : Integer = TAiProtectedBuilding(pDefBuilding).aipriority-TAiProtectedBuilding(pDefBuilding).defenders.GetCount;
      [*] = ;            if ((TAiProtectedBuilding(pDefBuilding).aipriority>0) and (TAiProtectedBuilding(pDefBuilding).defenders.GetCount=0)) then
      [*] = ;            inc(gPlayer[plind].aidata.guardsCount, 1);
      [*] = ;            if curStr<>0 then curStr := TAiProtectedBuilding(pDefBuilding).aipriority*2-TAiProtectedBuilding(pDefBuilding).defenders.GetCount;
      [*] = ;            if curStr > minStr then
      [*] = ;            begin
      [*] = ;               minStr:=curStr;
      [*] = ;               gPlayer[plind].aidata.guardsTotal := gPlayer[plind].aidata.guardsTotal+curStr;
      [*] = ;               buildIndex := i;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      //sending defenders:
      [*] = ;      if buildIndex<>-1 then
      [*] = ;      begin
      [*] = ;         var pDefBuilding : Pointer = gPlayer[plind].aidata.defbuildings.Get(buildIndex);
      [*] = ;         if (pDefBuilding<>nil) then
      [*] = ;         begin
      [*] = ;            var reqDefenders : Integer = TAiProtectedBuilding(pDefBuilding).aipriority - TAiProtectedBuilding(pDefBuilding).defenders.GetCount;
      [*] = ;            var reqDef : Integer = Min(reqDefenders, gPlayer[plind].aidata.defenders.GetCount);
      [*] = ;            gIntegerList.Clear;
      [*] = ;            for i:=gPlayer[plind].aidata.defenders.GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var goHnd : Integer = gPlayer[plind].aidata.defenders.Get(i);
      [*] = ;               if (goHnd<>0) then
      [*] = ;               begin
      [*] = ;                  var essentialtag : Integer = GetGameObjectStatesTagByHandle(goHnd) and gc_statetag_essential;
      [*] = ;                  if (essentialtag=gc_statetag_essential_none) then
      [*] = ;                  begin
      [*] = ;                     var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                     if bai or (not bai and (pobj<>nil) and (TObj(pobj).squad<0)) then
      [*] = ;                     begin
      [*] = ;                        gPlayer[plind].aidata.guards.Add(goHnd);
      [*] = ;                        TAiProtectedBuilding(pDefBuilding).defenders.Add(goHnd);
      [*] = ;                        gIntegerList.Add(goHnd);
      [*] = ;                        gPlayer[plind].aidata.defenders.Delete(i);
      [*] = ;                        if gIntegerList.GetCount>=reqDef then
      [*] = ;                        break;
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     gPlayer[plind].aidata.defenders.Delete(i);
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               ErrorLog('_ai_ProgressDefenders : goHnd=0');
      [*] = ;            end;
      [*] = ;            if (reqDefenders>=1) and (gIntegerList.GetCount=0) and (gPlayer[plind].aidata.freewarriors.GetCount>5) then
      [*] = ;            begin
      [*] = ;               reqDefenders := Min(reqDefenders, gPlayer[plind].aidata.freewarriors.GetCount);
      [*] = ;               for i:=gPlayer[plind].aidata.freewarriors.GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var goHnd : Integer = gPlayer[plind].aidata.freewarriors.get(i);
      [*] = ;                  if (goHnd<>0) then
      [*] = ;                  begin
      [*] = ;                     var essentialtag : Integer = GetGameObjectStatesTagByHandle(goHnd) and gc_statetag_essential;
      [*] = ;                     if (essentialtag=gc_statetag_essential_none) then
      [*] = ;                     begin
      [*] = ;                        var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                        if bai or (not bai and (pobj<>nil) and (TObj(pobj).squad<0)) then
      [*] = ;                        begin
      [*] = ;                           gPlayer[plind].aidata.guards.Add(goHnd);
      [*] = ;                           TAiProtectedBuilding(pDefBuilding).defenders.Add(goHnd);
      [*] = ;                           gIntegerList.Add(goHnd);
      [*] = ;                           gPlayer[plind].aidata.freewarriors.Delete(i);
      [*] = ;                           if gIntegerList.GetCount>=reqDefenders then
      [*] = ;                           break;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        gPlayer[plind].aidata.freewarriors.Delete(i);
      [*] = ;                     end;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  ErrorLog('_ai_ProgressDefenders : goHnd=0');
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            if (gIntegerList.GetCount>0) then
      [*] = ;            _player_OrderUnitsToGuard(plHnd, gIntegerList, TAiProtectedBuilding(pDefBuilding).targetHnd, True);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyMakeAgressorBattle
      [*] = ;//
      [*] = ;procedure _ai_ArmyMakeAgressorBattle(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      TArmy(pArmy).fOrder.iType := gc_ai_armyorder_agressor;
      [*] = ;      TArmy(pArmy).fSpecialOrder := true;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyAgressorLink
      [*] = ;//
      [*] = ;procedure _ai_ArmyAgressorLink(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var myX : Float = TArmy(pArmy).fCurX;
      [*] = ;      var myZ : Float = TArmy(pArmy).fCurZ;
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var enZone : Integer = _ai_FindNearestEnemyZone(plInd, 0, myX, myZ, true);
      [*] = ;
      [*] = ;      if enZone >= 0 then
      [*] = ;      begin
      [*] = ;         var myZone : Integer = TArmy(pArmy).fCurTopZone;
      [*] = ;         if IsInRangeInt(myZone, 0, TopologyGetZonesCount-1) then
      [*] = ;         begin
      [*] = ;            var nextZone : Integer;
      [*] = ;            if myZone = enZone then
      [*] = ;            nextZone := myZone
      [*] = ;            else
      [*] = ;            nextZone := _ai_GetNextZone(myZone, enZone, false, false);
      [*] = ;
      [*] = ;            if nextZone >=0 then
      [*] = ;            begin
      [*] = ;               if nextZone = myZone then
      [*] = ;               begin
      [*] = ;                  var enX, enZ : Float;
      [*] = ;                  TopologyGetZoneCenterByIndex(enZone, enX, enZ);
      [*] = ;                  _ai_ArmyBitva(pArmy, enX, enZ);
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  var x, z : Float;
      [*] = ;                  TopologyGetZoneCenterByIndex(nextZone, x, z);
      [*] = ;                  _ai_SetArmyPosition(pArmy, x, z, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_CreateAgressorArmy
      [*] = ;//
      [*] = ;function _ai_CreateAgressorArmy(plInd, unitCount : Integer; var list : TIntegerList) : Pointer;
      [*] = ;begin
      [*] = ;   if list.GetCount >= unitCount then
      [*] = ;   begin
      [*] = ;      gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;      var i : Integer;
      [*] = ;      var curCount : Integer;
      [*] = ;
      [*] = ;      for i := list.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = list.Get(i);
      [*] = ;         if _unit_GetTagStateByType(goHnd, gc_statetag_essential) = gc_statetag_essential_none then
      [*] = ;         begin
      [*] = ;            gPlayer[plInd].aiData.aiList.Add(goHnd);
      [*] = ;            list.Delete(i);
      [*] = ;            curCount := curCount+1;
      [*] = ;
      [*] = ;            if curCount = unitCount then
      [*] = ;            break;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if curCount < unitCount then
      [*] = ;      begin
      [*] = ;         for i := gPlayer[plInd].aiData.aiList.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var goHnd : Integer = gPlayer[plInd].aiData.aiList.Get(i);
      [*] = ;            list.Add(goHnd);
      [*] = ;         end;
      [*] = ;         Result := nil;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;         var pSquad : Pointer = _player_WriteSquadNew(plHnd, -1, 0, 0, gPlayer[plInd].aiData.aiList, false, false);
      [*] = ;         if (pSquad<>nil) then
      [*] = ;         begin
      [*] = ;            var pArmy : Pointer = gPlayer[plInd].aiData.armyList.AddSetup(plInd);
      [*] = ;            TSquad(pSquad).fType := gc_ai_armytype_none;
      [*] = ;            TArmy(pArmy).Add(pSquad);
      [*] = ;            TArmy(pArmy).fSpec := gc_ai_armyspec_agressor;
      [*] = ;            Result := pArmy;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            ErrorLog('ProgressWarAI : pSquad=nil : check place 1');
      [*] = ;            Result := nil;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   Result := nil;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_SendAgressors
      [*] = ;//
      [*] = ;procedure _ai_SendAgressors(plInd : Integer);
      [*] = ;begin
      [*] = ;   if (not gbool_peacemode) and (gPlayer[plInd].aiData.agressorsSent = 0) and (gPlayer[plInd].aiData.agressors.GetCount >= gc_ai_AgressorsCount) then
      [*] = ;   begin
      [*] = ;      gPlayer[plInd].aiData.agressorsSent := gPlayer[plInd].aiData.agressorsSent + 1;
      [*] = ;      var pArmy : Pointer = _ai_CreateAgressorArmy(plInd, gc_ai_AgressorsCount, gPlayer[plInd].aiData.agressors);
      [*] = ;      if pArmy <> nil then
      [*] = ;      _ai_ArmyMakeAgressorBattle(pArmy);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyDisband
      [*] = ;//
      [*] = ;procedure _ai_ArmyDisband(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;      var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;      var i : Integer;
      [*] = ;
      [*] = ;      for i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;         if pSquad<>nil then
      [*] = ;         begin
      [*] = ;            var sqInd : Integer = TSquad(pSquad).fIndex;
      [*] = ;            _misc_DisbandSquad(plHnd, sqInd, true);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_CreateOfficerSquad
      [*] = ;//
      [*] = ;procedure _ai_CreateOfficerSquad(offHnd, drumHnd : Integer);
      [*] = ;begin
      [*] = ;   if (offHnd <> 0) and (drumHnd <> 0) then
      [*] = ;   begin
      [*] = ;      var plHnd : Integer = GetGameObjectPlayerHandleByHandle(offHnd);
      [*] = ;      var plInd : Integer = GetPlayerIndexByHandle(plHnd);
      [*] = ;      var pObj : Pointer = _unit_GetTObj(offHnd);
      [*] = ;      if (pObj<>nil) then
      [*] = ;      begin
      [*] = ;         var newcid : Integer = TObj(pObj).cid;
      [*] = ;         var newid : Integer = -1;
      [*] = ;         var offsid : String = GetGameObjectBaseNameByHandle(offHnd);
      [*] = ;         var offInd : Integer = _country_GetOfficersIndexByOfficerSID(newcid, offsid);
      [*] = ;         if offInd >= 0 then
      [*] = ;         begin
      [*] = ;            var i, j : Integer;
      [*] = ;            var arUnits : array [0..gc_country_maxformationunitcount-1] of array [0..1] of Integer; // 0 - unit id, 1 - unit count
      [*] = ;
      [*] = ;            // find most compact formation
      [*] = ;            var formType : Integer = -1;
      [*] = ;            var min : Integer = gc_MaxInt;
      [*] = ;            for i := 0 to gc_country_maxformationcount-1 do
      [*] = ;            if gCountry[newcid].officers[offInd].formations[i].masks[0] <> '' then
      [*] = ;            begin
      [*] = ;               var formInd : Integer = _misc_GetFormationIndexBySID(gCountry[newcid].officers[offInd].formations[i].masks[0]);
      [*] = ;               if formInd >= 0 then
      [*] = ;               begin
      [*] = ;                  var w : Integer = gFormation[formInd].width;
      [*] = ;                  var h : Integer = gFormation[formInd].height;
      [*] = ;                  if w+h < min then
      [*] = ;                  begin
      [*] = ;                     min := w+h;
      [*] = ;                     formType := i;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            if formType >= 0 then
      [*] = ;            begin
      [*] = ;               var arCount : Integer;
      [*] = ;               for i := 0 to gc_country_maxformationunitcount-1 do
      [*] = ;               begin
      [*] = ;                  var sid : String = gCountry[newcid].officers[offInd].units[i];
      [*] = ;                  var id : Integer = _unit_ConvertObjSIDToID(newcid, gCountry[newcid].officers[offInd].units[i]);
      [*] = ;                  if id > 0 then
      [*] = ;                  begin
      [*] = ;                     gPlayer[plInd].aiData.avUnits[newcid][id] := false;
      [*] = ;
      [*] = ;                     arUnits[arCount][0] := id;
      [*] = ;                     arCount := arCount+1;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;
      [*] = ;               // count all suitable units
      [*] = ;               var agrCount : Integer = gPlayer[plInd].aiData.agressors.GetCount;
      [*] = ;               for i := 0 to agrCount-1 do
      [*] = ;               begin
      [*] = ;                  var goHnd : Integer = gPlayer[plInd].aiData.agressors.Get(i);
      [*] = ;                  if (goHnd<>0) then
      [*] = ;                  begin
      [*] = ;                     var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                     if (pObj<>nil) and (TObj(pObj).cid = newcid) and (_unit_GetTagStateByType(goHnd, gc_statetag_essential) = gc_statetag_essential_none) then
      [*] = ;                     begin
      [*] = ;                        var id1 : Integer = TObj(pObj).id;
      [*] = ;                        var arInd : Integer = -1;
      [*] = ;                        for j := 0 to arCount-1 do
      [*] = ;                        if arUnits[j][0] = id1 then
      [*] = ;                        begin
      [*] = ;                           arInd := j;
      [*] = ;                           break;
      [*] = ;                        end;
      [*] = ;
      [*] = ;                        if arInd >= 0 then
      [*] = ;                        arUnits[arInd][1] := arUnits[arInd][1]+1;
      [*] = ;                     end;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  ErrorLog('ProgressWarAI: _ai_CreateOfficerSquad : count all suitable units : hnd=0');
      [*] = ;               end;
      [*] = ;
      [*] = ;               var maxCount : Integer;
      [*] = ;               for i := 0 to arCount-1 do
      [*] = ;               begin
      [*] = ;                  if arUnits[i][1] > maxCount then
      [*] = ;                  begin
      [*] = ;                     maxCount := arUnits[i][1];
      [*] = ;                     newid := arUnits[i][0];
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;
      [*] = ;               var needCount : Integer;
      [*] = ;               var needFormInd : Integer;
      [*] = ;               // choose formation with maximum of possible units
      [*] = ;               for i := gc_country_maxofficersformationmask-1 downto 0{1} do
      [*] = ;               begin
      [*] = ;                  if gCountry[newcid].officers[offInd].formations[formType].masks[i] <> '' then
      [*] = ;                  begin
      [*] = ;                     var formSID : String = gCountry[newcid].officers[offInd].formations[formType].masks[i];
      [*] = ;                     var formInd : Integer = _misc_GetFormationIndexBySID(gCountry[newcid].officers[offInd].formations[formType].masks[i]);
      [*] = ;                     if (gFormation[formInd].countUnits > 0) and (maxCount >= gFormation[formInd].countUnits) then
      [*] = ;                     begin
      [*] = ;                        needCount := gFormation[formInd].countUnits;
      [*] = ;                        needFormInd := formInd;
      [*] = ;                        break
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;
      [*] = ;               // squad can be created
      [*] = ;               var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;               if (newid > 0) and (needCount > 0) then
      [*] = ;               begin
      [*] = ;                  gPlayer[plInd].aiData.aiList.Clear;
      [*] = ;                  var count : Integer;
      [*] = ;                  for i := agrCount-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     var goHnd : Integer = gPlayer[plInd].aiData.agressors.Get(i);
      [*] = ;                     var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                     if (pObj<>nil) and (TObj(pObj).cid = newcid) and (TObj(pObj).id = newid) and (_unit_GetTagStateByType(goHnd, gc_statetag_essential) = gc_statetag_essential_none) then
      [*] = ;                     begin
      [*] = ;                        gPlayer[plInd].aiData.aiList.Add(goHnd);
      [*] = ;                        gPlayer[plInd].aiData.agressors.Delete(i);
      [*] = ;
      [*] = ;                        count := count+1;
      [*] = ;                        if count = needCount then
      [*] = ;                        begin
      [*] = ;                           var armyType : Integer = _ai_GetArmyTypeByUsage(gObjProp[newcid][newid].usage);
      [*] = ;
      [*] = ;                           var pArmy : Pointer = gPlayer[plInd].aiData.armyList.AddSetup(plInd);
      [*] = ;                           if (pArmy<>nil) then
      [*] = ;                           begin
      [*] = ;                              TArmy(pArmy).fSpec := _ai_GetUnitArmySpec(goHnd);
      [*] = ;
      [*] = ;                              var pSquad : Pointer = _player_WriteSquadNew(plHnd, needFormInd, offHnd, drumHnd, gPlayer[plInd].aiData.aiList, false, true);
      [*] = ;                              if (pSquad<>nil) then
      [*] = ;                              begin
      [*] = ;                                 TSquad(pSquad).fType := armyType;
      [*] = ;                                 TArmy(pArmy).Add(pSquad);
      [*] = ;
      [*] = ;                                 var cx : Float = gPlayer[plInd].aiData.centerX;
      [*] = ;                                 var cz : Float = gPlayer[plInd].aiData.centerZ;
      [*] = ;
      [*] = ;                                 _ai_SetArmyPosition(pArmy, cx, cz, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                              end
      [*] = ;                              else
      [*] = ;                              ErrorLog('ProgressWarAI : pSquad=nil : check place 2');
      [*] = ;                           end;
      [*] = ;
      [*] = ;                           break;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_TryCreateOfficerSquad
      [*] = ;//
      [*] = ;procedure _ai_TryCreateOfficerSquad(plInd : Integer);
      [*] = ;begin
      [*] = ;   var i, j : Integer;
      [*] = ;
      [*] = ;   for i := 0 to gc_MaxCountryCount-1 do
      [*] = ;   for j := 0 to gc_country_maxmembers-1 do
      [*] = ;   gPlayer[plInd].aiData.avUnits[i][j] := true;
      [*] = ;
      [*] = ;   for i := 0 to gc_MaxCountryCount-1 do
      [*] = ;   begin
      [*] = ;      if (gPlayer[plInd].aiData.officer17[i] <> 0) and (gPlayer[plInd].aiData.drummer17[i] <> 0) then
      [*] = ;      _ai_CreateOfficerSquad(gPlayer[plInd].aiData.officer17[i], gPlayer[plInd].aiData.drummer17[i]);
      [*] = ;
      [*] = ;      if (gPlayer[plInd].aiData.officer18[i] <> 0) and (gPlayer[plInd].aiData.drummer18[i] <> 0) then
      [*] = ;      _ai_CreateOfficerSquad(gPlayer[plInd].aiData.officer18[i], gPlayer[plInd].aiData.drummer18[i]);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyTransportLink
      [*] = ;//
      [*] = ;procedure _ai_ArmyTransportLink(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      if TArmy(pArmy).fSpec = gc_ai_armyspec_transport then
      [*] = ;      begin
      [*] = ;         if TArmy(pArmy).fOrder.iType = gc_ai_armyorder_transport then
      [*] = ;         begin
      [*] = ;            var plInd : Integer = TArmy(pArmy).fPlIndex;
      [*] = ;            var moveCount : Integer = _ai_GetArmyMoveCount(pArmy);
      [*] = ;            var goHnd : Integer = _ai_GetArmyUnit(pArmy);
      [*] = ;            var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;
      [*] = ;            if pObj <> nil then
      [*] = ;            case TArmy(pArmy).fOrder.tag of
      [*] = ;               gc_ai_armyorder_transport_findunits : begin
      [*] = ;                  if moveCount = 0 then
      [*] = ;                  begin
      [*] = ;                     var portHnd : Integer = _misc_GetNearestPort(plInd, TArmy(pArmy).fX, TArmy(pArmy).fZ);
      [*] = ;                     if portHnd <> 0 then
      [*] = ;                     begin
      [*] = ;                        var x : Float = GetGameObjectPositionXByHandle(portHnd);
      [*] = ;                        var z : Float = GetGameObjectPositionZByHandle(portHnd);
      [*] = ;                        var landing : Integer = _ai_TestArmyBrushPosition(pArmy, x, z, 0, 0, false);
      [*] = ;
      [*] = ;                        if landing > 0 then
      [*] = ;                        begin
      [*] = ;                           _ai_SetArmyBrushPosition(pArmy, x, z, 0, 0);
      [*] = ;                           TArmy(pArmy).fOrder.tag := gc_ai_armyorder_transport_moor;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           if _ai_FindMoorPosition(pArmy, x, z) then
      [*] = ;                           TArmy(pArmy).fOrder.tag := gc_ai_armyorder_transport_moor;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_transport_moor : begin
      [*] = ;                  // check target position for another ferries presence
      [*] = ;                  var cFerryRadius : Float = 2;
      [*] = ;                  var cSearchRadius : Float = 10;
      [*] = ;                  var tpCount : Integer = GetGameObjectTrackPointCountByHandle(goHnd);
      [*] = ;                  var tarX, tarY, tarZ : Float;
      [*] = ;                  GetGameObjectTrackPointCoordsByIndexByHandle(goHnd, tpCount-1, tarX, tarY, tarZ);
      [*] = ;
      [*] = ;                  var i, j : Integer;
      [*] = ;                  for[MAIN] i := gPlayer[plInd].aiData.armyList.GetCount-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     var pArmy1 : Pointer = gPlayer[plInd].aiData.armyList.Get(i);
      [*] = ;                     if (pArmy1 <> pArmy) and _ai_IsArmyTransport(pArmy1) and (TArmy(pArmy1).fOrder.tag = gc_ai_armyorder_transport_loadunits) then
      [*] = ;                     begin
      [*] = ;                        var goHnd1 : Integer = _ai_GetArmyUnit(pArmy1);
      [*] = ;                        var pObj1 : Pointer = _unit_GetTObj(goHnd1);
      [*] = ;                        if TObj(pObj1).bRally then
      [*] = ;                        begin
      [*] = ;                           var goX : Float = GetGameObjectPositionXByHandle(goHnd1);
      [*] = ;                           var goZ : Float = GetGameObjectPositionZByHandle(goHnd1);
      [*] = ;                           if VectorDistance(tarX, 0, tarZ, goX, 0, goZ) < cFerryRadius then
      [*] = ;                           begin
      [*] = ;                              var island : Integer = _misc_GetIsland(TObj(pObj1).rallyX, TObj(pObj1).rallyY);
      [*] = ;                              var j : Integer;
      [*] = ;                              for j := 0 to 10 do
      [*] = ;                              begin
      [*] = ;                                 var newX : Float = goX + cSearchRadius * (2*random-1);
      [*] = ;                                 var newZ : Float = goZ + cSearchRadius * (2*random-1);
      [*] = ;                                 if _misc_GetIsland(newX, newZ) = island then
      [*] = ;                                 begin
      [*] = ;                                    _ai_SetArmyBrushPosition(pArmy, newX, newZ, 0, 0);
      [*] = ;                                    break(MAIN);
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_transport_loadunits : begin
      [*] = ;                  if TObj(pObj).bRally then
      [*] = ;                  begin
      [*] = ;                     var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;                     var pObjInside : Pointer = _misc_GetObjectArgData(goHnd, gc_argunit_inside);
      [*] = ;                     if TObj(pObj).insideReserved = 0 then
      [*] = ;                     begin
      [*] = ;                        if TIntegerList(pObjInside).GetCount = 0 then
      [*] = ;                        _ai_TransportPickArmy(goHnd)
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           var island : Integer = _misc_GetIsland(TObj(pObj).rallyX, TObj(pObj).rallyY);
      [*] = ;                           if _ai_TransportLandUnits(goHnd, island) then
      [*] = ;                           TArmy(pArmy).fOrder.tag := gc_ai_armyorder_transport_landunits;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  TArmy(pArmy).fOrder.tag := gc_ai_armyorder_transport_findunits;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_transport_landunits : begin
      [*] = ;                  if moveCount = 0 then
      [*] = ;                  begin
      [*] = ;                     var tpCount : Integer = GetGameObjectTrackPointCountByHandle(goHnd);
      [*] = ;                     var myX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;                     var myZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;                     var stX, stY, stZ : Float;
      [*] = ;                     GetGameObjectTrackPointCoordsByIndexByHandle(goHnd, 0, stX, stY, stZ);
      [*] = ;                     var endX, endY, endZ : Float;
      [*] = ;                     GetGameObjectTrackPointCoordsByIndexByHandle(goHnd, tpCount-1, endX, endY, endZ);
      [*] = ;
      [*] = ;                     if (VectorDistance(myX, 0, myZ, endX, 0, endZ) < VectorDistance(myX, 0, myZ, stX, 0, stZ))
      [*] = ;                     and (_unit_GetTagStateByType(goHnd, gc_statetag_move) = gc_statetag_move_idle) then
      [*] = ;                     begin
      [*] = ;                        var mw : Integer = GetMapWidth;
      [*] = ;                        var mh : Integer = GetMapHeight;
      [*] = ;                        if TObj(pObj).bRally and IsInRange(TObj(pObj).rallyX, -mw/2, mw/2) and IsInRange(TObj(pObj).rallyY, -mh/2, mh/2) then
      [*] = ;                        begin
      [*] = ;                           var pObjInside : Pointer = _misc_GetObjectArgData(goHnd, gc_argunit_inside);
      [*] = ;                           gIntegerList.Clear;
      [*] = ;                           var j : Integer;
      [*] = ;                           var count : Integer = TIntegerList(pObjInside).GetCount;
      [*] = ;
      [*] = ;                           for j := 0 to count-1 do
      [*] = ;                           gIntegerList.Add(TIntegerList(pObjInside).Get(j));
      [*] = ;
      [*] = ;                           _unit_DoUnitsGoOutside(gIntegerList, false, true);
      [*] = ;                           var pNewArmy : Pointer = _ai_CreateArmy(plInd, gIntegerList, false);
      [*] = ;                           if (pNewArmy<>nil) then
      [*] = ;                           TArmy(pNewArmy).fActive := false
      [*] = ;                           else
      [*] = ;                           ErrorLog('ProgressWarAI: transport can''t create army of loaded units');
      [*] = ;
      [*] = ;                           TArmy(pArmy).fOrder.tag := gc_ai_armyorder_transport_unloadunits;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           var angle : Float = GetGameObjectSTArrowAngleByHandle(goHnd);
      [*] = ;                           var dirX : Float = 1;
      [*] = ;                           var dirY, dirZ : Float;
      [*] = ;                           VectorRotateY(dirX, dirY, dirZ, angle);
      [*] = ;
      [*] = ;                           if not _unit_TestTransportPosition(goHnd, myX, myZ, dirX, dirZ) then
      [*] = ;                           _ai_FindMoorPosition(pArmy, endX, endZ);
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_transport_unloadunits : begin
      [*] = ;                  var pObjInside : Pointer = _misc_GetObjectArgData(goHnd, gc_argunit_inside);
      [*] = ;                  var count : Integer = TIntegerList(pObjInside).GetCount;
      [*] = ;                  if TIntegerList(pObjInside).GetCount = 0 then
      [*] = ;                  begin
      [*] = ;                     GameObjectDelayExecuteStateByHandle(goHnd, 'SailToDestroy', gc_unit_SailToDestroyTime);
      [*] = ;                     TArmy(pArmy).fOrder.tag := gc_ai_armyorder_transport_waitdestroy;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_transport_destroy : begin
      [*] = ;                  if moveCount = 0 then
      [*] = ;                  begin
      [*] = ;                     var tpCount : Integer = GetGameObjectTrackPointCountByHandle(goHnd);
      [*] = ;                     var myX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;                     var myZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;                     var x, y, z : Float;
      [*] = ;                     GetGameObjectTrackPointCoordsByIndexByHandle(goHnd, tpCount-1, x, y, z);
      [*] = ;                     var eps : Float = GetGameObjectEpsilonDistanceByHandle(goHnd);
      [*] = ;                     if (VectorDistance(myX, 0, myZ, x, 0, z) < eps) and (_unit_GetTagStateByType(goHnd, gc_statetag_move) = gc_statetag_move_idle) then
      [*] = ;                     _unit_SetTagStates(goHnd, gc_statetag_essential_death)
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_transport_waitloadunits, gc_ai_armyorder_transport_waitdestroy : begin
      [*] = ;               end;
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  TArmy(pArmy).fOrder.tag := gc_ai_armyorder_transport_findunits;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         TArmy(pArmy).fOrder.iType := gc_ai_armyorder_transport;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      TArmy(pArmy).fOrder.iType := gc_ai_armyorder_none;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ArmyAttackWallLink
      [*] = ;//
      [*] = ;procedure _ai_ArmyAttackWallLink(pArmy : Pointer);
      [*] = ;begin
      [*] = ;   if (pArmy<>nil) then
      [*] = ;   begin
      [*] = ;      var moveCount : Integer = _ai_GetArmyMoveCount(pArmy);
      [*] = ;      if moveCount = 0 then
      [*] = ;      begin
      [*] = ;         var i, j : Integer;
      [*] = ;         var uid : Integer = TArmy(pArmy).fOrder.targetUID;
      [*] = ;         var wHnd : Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;         if wHnd <> 0 then
      [*] = ;         begin
      [*] = ;            var approach : Boolean;
      [*] = ;            var artCount : Integer;
      [*] = ;            var artSqCount : Integer;
      [*] = ;            var nullDelay : Integer;
      [*] = ;            var artX, artZ : Float;
      [*] = ;
      [*] = ;            var wX : Float = GetGameObjectPositionXByHandle(wHnd);
      [*] = ;            var wZ : Float = GetGameObjectPositionZByHandle(wHnd);
      [*] = ;
      [*] = ;            for [MAIN]i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;               if (pSquad<>nil) and (_ai_SquadIsArtillery(pSquad)) then
      [*] = ;               begin
      [*] = ;                  artSqCount := artSqCount+1;
      [*] = ;                  artX := artX + TSquad(pSquad).fCurX;
      [*] = ;                  artZ := artZ + TSquad(pSquad).fCurZ;
      [*] = ;
      [*] = ;                  for j := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     artCount := artCount+1;
      [*] = ;                     var goHnd : Integer = TSquad(pSquad).Get(j);
      [*] = ;                     var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                     var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;                     var pObjBase : Pointer = _unit_GetObjBase(goHnd);
      [*] = ;                     if IsInRange(TObj(pObj).attackDelay, 0, 1) then
      [*] = ;                     nullDelay := nullDelay+1;
      [*] = ;
      [*] = ;                     var goX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;                     var goZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;                     var dist : Float = VectorDistance(wX, 0, wZ, goX, 0, goZ);
      [*] = ;
      [*] = ;                     var minR : Float = TObjProp(pObjProp).minattackradius;
      [*] = ;                     var maxR : Float = TObjBase(pObjBase).weapon[0].radiusMax;
      [*] = ;                     var rad : Float = minR + (maxR - minR) * TObj(pObj).artDistFactor;
      [*] = ;
      [*] = ;                     if (dist > rad+1) then
      [*] = ;                     approach := true;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            if artCount = 0 then
      [*] = ;            approach := true;
      [*] = ;
      [*] = ;            if approach then
      [*] = ;            begin
      [*] = ;               if (not _ai_ArmyCheckWallAttack(pArmy)) then
      [*] = ;               begin
      [*] = ;                  var myZone : Integer = TArmy(pArmy).fCurTopZone;
      [*] = ;                  var tZone : Integer = TArmy(pArmy).fOrder.tag;
      [*] = ;                  var zone : Integer = _ai_GetNextZone(myZone, tZone, false, true);
      [*] = ;                  if zone <> myZone then
      [*] = ;                  begin
      [*] = ;                     var nextX, nextZ : Float;
      [*] = ;                     TopologyGetZoneCenterByIndex(zone, nextX, nextZ);
      [*] = ;                     _ai_SetArmyPosition(pArmy, nextX, nextZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                  end
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               if (artCount > 0) then
      [*] = ;               begin
      [*] = ;                  if nullDelay < artCount then
      [*] = ;                  TArmy(pArmy).fLastArtTime := GetGameTime
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     if GetGameTime - TArmy(pArmy).fLastArtTime >= 3 then
      [*] = ;                     begin
      [*] = ;                        artX := artX / artSqCount;
      [*] = ;                        artZ := artZ / artSqCount;
      [*] = ;
      [*] = ;                        gPtrList.Clear;
      [*] = ;                        for [MAIN]i := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;                        begin
      [*] = ;                           var pSquad : Pointer = TArmy(pArmy).fSquadList.GetPointer(i);
      [*] = ;                           if (pSquad<>nil) and (not _ai_SquadIsArtillery(pSquad)) then
      [*] = ;                           gPtrList.AddPointer(pSquad);
      [*] = ;                        end;
      [*] = ;
      [*] = ;                        var cx, cz : Float;
      [*] = ;                        _ai_SetSquadListPosition(gPtrList, artX, artZ, 0, 0, cx, cz, gc_obj_order_move_mode_default);
      [*] = ;
      [*] = ;                        TArmy(pArmy).fLastArtTime := GetGameTime;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// _ai_ExecuteArmies
      [*] = ;//
      [*] = ;procedure _ai_ExecuteArmies(plInd : Integer);
      [*] = ;begin
      [*] = ;   var armyCount : Integer = gPlayer[plInd].aiData.armyList.GetCount;
      [*] = ;   var bEnemyExists : Boolean = _ai_IsEnemiesExists(plInd);
      [*] = ;   var i, j, k : Integer;
      [*] = ;   for i := armyCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var pArmy : Pointer = gPlayer[plInd].aiData.armyList.Get(i);
      [*] = ;      if (pArmy <> nil) and TArmy(pArmy).fActive then
      [*] = ;      begin
      [*] = ;         var moveCount : Integer = _ai_GetArmyMoveCount(pArmy);
      [*] = ;         var attackCount : Integer = _ai_GetArmyAttackCount(pArmy);
      [*] = ;         var unitCount : Integer = _ai_GetArmyUnitsCount(pArmy);
      [*] = ;
      [*] = ;         var goHnd : Integer = _ai_GetArmyUnit(pArmy);
      [*] = ;         if _unit_GetUsage(goHnd) = gc_obj_usage_fisher then
      [*] = ;         gPlayer[plInd].aiData.armyList.Delete(i)
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            case TArmy(pArmy).fOrder.iType of
      [*] = ;               gc_ai_armyorder_makebattle: begin
      [*] = ;                  if (bEnemyExists) then
      [*] = ;                  begin
      [*] = ;                     var enX, enZ : Float;
      [*] = ;                     var bitva : Boolean;
      [*] = ;                     if gbool_peacemode then
      [*] = ;                     begin
      [*] = ;                        if TArmy(pArmy).fOnEnemyLand {and (moveCount = 0)} then
      [*] = ;                        begin
      [*] = ;                           var stZone : Integer = TArmy(pArmy).fTopZone;
      [*] = ;                           var curZone : Integer = TArmy(pArmy).fCurTopZone;
      [*] = ;                           var endZone : Integer = TopologyGetZoneIndex(gPlayer[plInd].aiData.centerX, gPlayer[plInd].aiData.centerZ);
      [*] = ;                           var zone : Integer = _ai_GetNextZone(stZone, endZone, false, false);
      [*] = ;                           if zone < 0 then
      [*] = ;                           zone := _ai_GetNextZone(curZone, endZone, false, false);
      [*] = ;
      [*] = ;                           if zone >= 0 then
      [*] = ;                           begin
      [*] = ;                              var x, z : Float;
      [*] = ;                              TopologyGetZoneCenterByIndex(zone, x, z);
      [*] = ;                              _ai_SetArmyPosition(pArmy, x, z, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     bitva := _ai_CheckArmyBitva(pArmy, true, enX, enZ);
      [*] = ;
      [*] = ;                     if bitva then
      [*] = ;                     _ai_ArmyBitva(pArmy, enX, enZ)
      [*] = ;                     else
      [*] = ;                     begin
      [*] = ;                        var wAttack : Boolean;
      [*] = ;                        if attackCount = 0 then
      [*] = ;                        wAttack := _ai_ArmyCheckWallAttack(pArmy);
      [*] = ;                        // condition for attackCount is needed for armies with artillery only - process army only if at least one
      [*] = ;                        // cannon doesn't have target
      [*] = ;                        if (not wAttack) and (moveCount = 0) and (attackCount < unitCount) then
      [*] = ;                        begin
      [*] = ;                           var active : Boolean = (not gbool_peacemode);
      [*] = ;                           _ai_ArmyMakeBattleLink(pArmy, active, true);
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_attackwall: begin
      [*] = ;                  if bEnemyExists then
      [*] = ;                  begin
      [*] = ;                     var wHnd : Integer = GetGameObjectHandleByUniqueId(TArmy(pArmy).fOrder.targetUID);
      [*] = ;                     var customName : String = GetGameObjectCustomNameByHandle(wHnd);
      [*] = ;                     var needAttack : Boolean = true;
      [*] = ;                     if wHnd = 0 then
      [*] = ;                     begin
      [*] = ;                        _ai_ArmyClearOrder(pArmy);
      [*] = ;                        needAttack := false;
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     needAttack := not _unit_WallIsOneRegion(wHnd);
      [*] = ;
      [*] = ;                     if needAttack then
      [*] = ;                     _ai_ArmyAttackWallLink(pArmy)
      [*] = ;                     else
      [*] = ;                     begin
      [*] = ;                        // target wall is destroyed
      [*] = ;                        TArmy(pArmy).fOrder.iType := gc_ai_armyorder_makebattle;
      [*] = ;                        _unit_SetArmyOrderTarget(pArmy, 0);
      [*] = ;                        _ai_ArmyMakeLandBattleLink(pArmy, true, true);
      [*] = ;
      [*] = ;                        for j := TArmy(pArmy).fSquadList.GetCount-1 downto 0 do
      [*] = ;                        begin
      [*] = ;                           var pSquad : Pointer = TArmy(pArmy).GetSquad(j);
      [*] = ;                           if (pSquad<>nil) and (_ai_SquadIsArtillery(pSquad)) then
      [*] = ;                           begin
      [*] = ;                              for k := TSquad(pSquad).GetCount-1 downto 0 do
      [*] = ;                              begin
      [*] = ;                                 var goHnd : Integer = TSquad(pSquad).Get(k);
      [*] = ;                                 _unit_ResetArtParams(goHnd);
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_makewaterbattle: begin
      [*] = ;                  if (bEnemyExists) then
      [*] = ;                  begin
      [*] = ;                     var bitva : Boolean;
      [*] = ;                     var enX, enZ : Float;
      [*] = ;                     if not gbool_peacemode then
      [*] = ;                     bitva := _ai_CheckArmyWaterBitva(pArmy, enX, enZ);
      [*] = ;
      [*] = ;                     if bitva then
      [*] = ;                     _ai_ArmyBitva(pArmy, enX, enZ)
      [*] = ;                     else
      [*] = ;                     if moveCount = 0 then
      [*] = ;                     _ai_ArmyMakeWaterBattleLink(pArmy);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_buildmine : begin
      [*] = ;                  if moveCount = 0 then
      [*] = ;                  _ai_ProgressMineBuilders(pArmy);
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_bitva : begin
      [*] = ;                  if gbool_peacemode then
      [*] = ;                  TArmy(pArmy).fOrder.iType := gc_ai_armyorder_none
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     if (moveCount = 0) and (attackCount = 0) then
      [*] = ;                     _ai_ArmyBitvaLinkExt(pArmy, false, 0, 0)
      [*] = ;                     else
      [*] = ;                     _ai_ArmyBitvaProgress(pArmy);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_sabotage : begin
      [*] = ;                  if (not gbool_peacemode) and (moveCount = 0) then
      [*] = ;                  _ai_ArmyDiversiaLink(pArmy);
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_agressor : begin
      [*] = ;                  if (not gbool_peacemode) and (moveCount = 0) then
      [*] = ;                  _ai_ArmyAgressorLink(pArmy);
      [*] = ;               end;
      [*] = ;               gc_ai_armyorder_transport : begin
      [*] = ;                  if not gbool_peacemode then
      [*] = ;                  _ai_ArmyTransportLink(pArmy);
      [*] = ;               end;
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  if not gbool_peacemode then
      [*] = ;                  begin
      [*] = ;                     if _ai_IsArmyTransport(pArmy) then
      [*] = ;                     TArmy(pArmy).fOrder.iType := gc_ai_armyorder_transport
      [*] = ;                     else
      [*] = ;                     _ai_ArmyMakeBattle(pArmy);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (bai) then
      [*] = ;begin
      [*] = ;   _ai_TryCreateOfficerSquad(plInd);
      [*] = ;   _ai_ExecuteArmies(plInd);
      [*] = ;   _ai_SmartGamer(plInd);
      [*] = ;   _ai_SendAgressors(plInd);
      [*] = ;   _ai_ProgressDefenders(plInd);
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   {var armyCount : Integer = gPlayer[plInd].aiData.armyList.GetCount;
      [*] = ;   var i : Integer;
      [*] = ;   for i := 0 to armyCount-1 do
      [*] = ;   begin
      [*] = ;      var pArmy : Pointer = gPlayer[plInd].aiData.armyList.Get(i);
      [*] = ;      if pArmy <> nil then
      [*] = ;      begin
      [*] = ;         if TArmy(pArmy).fOrder.iType=gc_ai_armyorder_buildmine then
      [*] = ;         begin
      [*] = ;            var moveCount : Integer = _ai_GetArmyMoveCount(pArmy);
      [*] = ;            if moveCount = 0 then
      [*] = ;            _ai_ProgressMineBuilders(pArmy);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;}
      [*] = ;   if (gPlayer[plind].playeradviser.adviser.bavailable and gPlayer[plind].playeradviser.adviser.benable and gPlayer[plind].playeradviser.adviser.bquartermaster and gPlayer[plind].playeradviser.adviser.bsetupguards) then
      [*] = ;   _ai_ProgressDefenders(plInd);
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

