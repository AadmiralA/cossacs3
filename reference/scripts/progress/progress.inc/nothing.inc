section.begin
   Name = Nothing
   UsedFileName = 
   Code : struct.begin
      [*] = ;procedure DoSetupMoveAnimation(goHnd : Integer);
      [*] = ;begin
      [*] = ;   var stateTag : Integer = GetGameObjectStatesTagByHandle(goHnd);
      [*] = ;   var minFrameBlend : Integer = 6;
      [*] = ;   var maxFrameBlend : Integer = 8;
      [*] = ;   var anim, animturn : String;
      [*] = ;   var bchecked : Boolean;
      [*] = ;   var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;   var iResource : Integer = stateTag and gc_statetag_resource;
      [*] = ;   var iWeapon : Integer = stateTag and gc_statetag_weapon;
      [*] = ;   if (iResource<>gc_statetag_resource_none) then
      [*] = ;   begin
      [*] = ;      case iResource of
      [*] = ;         gc_statetag_resource_food : anim := gc_anim_walkfood;
      [*] = ;         gc_statetag_resource_wood : anim := gc_anim_walkwood;
      [*] = ;         gc_statetag_resource_stone : anim := gc_anim_walkstone;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   case iWeapon of
      [*] = ;      gc_statetag_weapon_none : anim := gc_anim_walk;
      [*] = ;      gc_statetag_weapon_0 : begin
      [*] = ;         anim := gc_anim_walk0;
      [*] = ;         if (not _unit_IsAnimationExists(goHnd, anim)) then
      [*] = ;         anim := gc_anim_walk;
      [*] = ;         bchecked := True;
      [*] = ;      end;
      [*] = ;      gc_statetag_weapon_1 : begin
      [*] = ;         anim := gc_anim_walk1;
      [*] = ;         if (not _unit_IsAnimationExists(goHnd, anim)) then begin
      [*] = ;            anim := gc_anim_walk0;
      [*] = ;            if (not _unit_IsAnimationExists(goHnd, anim)) then
      [*] = ;            anim := gc_anim_walk;
      [*] = ;         end;
      [*] = ;         bchecked := True;
      [*] = ;      end;
      [*] = ;      gc_statetag_weapon_2 : begin
      [*] = ;         anim := gc_anim_walk2;
      [*] = ;         if (not _unit_IsAnimationExists(goHnd, anim)) then begin
      [*] = ;            anim := gc_anim_walk1;
      [*] = ;            if (not _unit_IsAnimationExists(goHnd, anim)) then
      [*] = ;            begin
      [*] = ;               anim := gc_anim_walk0;
      [*] = ;               if (not _unit_IsAnimationExists(goHnd, anim)) then
      [*] = ;               anim := gc_anim_walk;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         bchecked := True;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   if (anim<>'') then
      [*] = ;   begin
      [*] = ;      if (not bchecked) and (not _unit_IsAnimationExists(goHnd, anim)) then // in cases if this unit have attack0 and dont have walk0, like musketeeraus
      [*] = ;      anim := gc_anim_walk;
      [*] = ;      //if (anim<>GetGameObjectAnimationCycleNameByHandle(goHnd)) then
      [*] = ;      begin
      [*] = ;         GameObjectSwitchToTreeAnimationCyclesBlendByHandle(goHnd, anim, False, True, False, False, minFrameBlend, maxFrameBlend);
      [*] = ;         if (gObjProp[TObj(pobj).cid][TObj(pobj).id].usage=gc_obj_usage_peasant) then
      [*] = ;         _unit_ManagePeasantAttach(goHnd, anim);
      [*] = ;         var animinterval : Float = gObjProp[TObj(pobj).cid][TObj(pobj).id].walkintervalfactor;
      [*] = ;         if (animinterval<>GetGameObjectIntervalFactorByHandle(goHnd)) then
      [*] = ;         SetGameObjectIntervalFactorByHandle(goHnd, animinterval);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;var lastprogresstime : Float = gProgress.lastprogresstime;
      [*] = ;var gametime : Float = GetGameTime;
      [*] = ;var deltatime : Float = gametime-lastprogresstime;
      [*] = ;
      [*] = ;if (lastprogresstime>0) and (deltatime>0) then // not on pause
      [*] = ;begin
      [*] = ;   var perffps : Float = GetProgressPlayersPerformance;
      [*] = ;   perffps := MinFloat(perffps, 1/FramesPerSecond);
      [*] = ;   var realfps : Float = 1/perffps;
      [*] = ;   var cycletime : Float = perffps*(GetTimeSpeedFactor/10);
      [*] = ;   gint_perf_progressind := ((gint_perf_progressind+1) mod gc_perf_progresshistory);
      [*] = ;   garrfloat_perf_progress[gint_perf_progressind] := perffps;
      [*] = ;
      [*] = ;   _res_ProcessEconomy(deltatime);
      [*] = ;   if (gScenario.bexists) then
      [*] = ;   begin
      [*] = ;      var i : Integer;
      [*] = ;      for i:=0 to gc_scenario_maxzones-1 do
      [*] = ;      begin
      [*] = ;         if (gScenario.zones[i].bactive) and (gScenario.zones[i].blinkgroup) and (gScenario.zones[i].linkgroup>-1) and (gScenario.groups[gScenario.zones[i].linkgroup].unitslist.GetCount>0) then
      [*] = ;         begin
      [*] = ;            //_misc_ProfilerBegin('SAIRRTUL');
      [*] = ;            _scenario_SetAIRegionRectToUnitsList(gScenario.groups[gScenario.zones[i].linkgroup].unitslist, i, gScenario.zones[i].addradius, True, gScenario.zones[i].buseaverage);
      [*] = ;            //_misc_ProfilerEnd('SAIRRTUL');
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   // progress units production in buildings
      [*] = ;   if (gWriteNewRequestList.GetCount>0) and (_net_IsOffline or _net_IsServer) then
      [*] = ;   _player_ProgressWriteNewRequestList;
      [*] = ;
      [*] = ;   if (gScenario.bactive) then
      [*] = ;   begin
      [*] = ;      if _net_IsOffline or ((IsLanReady) and (gInterface.gamemode=gc_gamemode_game) and (gMap.gamestage>gc_map_gamestage_waitingloading)) then
      [*] = ;      ExecuteState('Scenario');
      [*] = ;   end;
      [*] = ;   if (gMovie.bstarted) then
      [*] = ;   _movie_ProcessMovie;
      [*] = ;
      [*] = ;   // progress projectile simulations
      [*] = ;   if (gProjSimulatePoints.maxdamagetime>0) then
      [*] = ;   _misc_ProjSimulatePointsProgress;
      [*] = ;
      [*] = ;   var pList : Pointer;
      [*] = ;   var landPath : Boolean;
      [*] = ;   if (gWaterPathList.GetCount>0) and ((gGOPathList.GetCount=0) or (gProgress.progresstick mod 2=0)) then
      [*] = ;   begin
      [*] = ;      pList := gWaterPathList;
      [*] = ;      landPath := false;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      pList := gGOPathList;
      [*] = ;      landPath := true;
      [*] = ;   end;
      [*] = ;
      [*] = ;   var count : Integer = TIntegerList(pList).GetCount;
      [*] = ;   if (count>0) then
      [*] = ;   begin
      [*] = ;      TopologyClearPathGameObjects;
      [*] = ;      var i, j : Integer;
      [*] = ;      for i := 0 to count-1 do
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = TIntegerList(pList).Get(i);
      [*] = ;
      [*] = ;         if _net_debug_log_progress_path then
      [*] = ;         _net_Log('uid('+IntToStr(GetGameObjectUniqueIdByHandle(TIntegerList(pList).Get(i)))+') '+
      [*] = ;         'tag('+IntToStr(GetGameObjectPrevStatesTagByHandle(TIntegerList(pList).Get(i)))+'->'+IntToStr(GetGameObjectStatesTagByHandle(TIntegerList(pList).Get(i)))+') '+
      [*] = ;         'pos('+FloatToStr(GetGameObjectPositionXByHandle(TIntegerList(pList).Get(i)))+';'+FloatToStr(GetGameObjectPositionZByHandle(TIntegerList(pList).Get(i)))+') '+
      [*] = ;         'tar('+FloatToStr(GetGameObjectStateTargetPositionXByHandle(TIntegerList(pList).Get(i)))+';'+FloatToStr(GetGameObjectStateTargetPositionZByHandle(TIntegerList(pList).Get(i)))+') '+
      [*] = ;         'ang('+FloatToStr(GetGameObjectSTArrowAngleByHandle(TIntegerList(pList).Get(i)))+')');
      [*] = ;
      [*] = ;         TopologyAddPathGameObjectByHandle(goHnd);
      [*] = ;
      [*] = ;         var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;         var pObjProp : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;
      [*] = ;         if TObjProp(pObjProp).media = gc_obj_media_land then
      [*] = ;         SetGameObjectTagFloatByHandle(goHnd, TObj(pObj).squad)
      [*] = ;         else
      [*] = ;         SetGameObjectTagFloatByHandle(goHnd, -1);
      [*] = ;      end;
      [*] = ;
      [*] = ;      _misc_ProfilerBegin('progress.GetPath');
      [*] = ;
      [*] = ;      if landPath then
      [*] = ;      TopologyGetPath
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         var quadTree : Integer = TopologyGetTopologyQuadTree;
      [*] = ;         TopologyGetPathExt(quadTree, gc_collisiontag_water, gc_TestPriorityOption_Water);
      [*] = ;      end;
      [*] = ;
      [*] = ;      _misc_ProfilerEnd('progress.GetPath');
      [*] = ;
      [*] = ;      for i:=0 to count-1 do
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = TIntegerList(pList).Get(i);
      [*] = ;         if (goHnd<>0) then // rare ca.se when list count decreased after DoSetupMoveAnimation, so this check is ok
      [*] = ;         begin
      [*] = ;            var noPath : Boolean = (GetGameObjectTrackPointCountByHandle(goHnd) = 0);
      [*] = ;
      [*] = ;            var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;            if (pobj<>nil) then
      [*] = ;            begin
      [*] = ;               TObj(pobj).bpathrequested := False;
      [*] = ;               var curOrd : Integer = _unit_GetCurrentOrder(goHnd);
      [*] = ;               TObj(pobj).orders[curOrd].bremove := true;
      [*] = ;               //var trgHnd : Integer = GetGameObjectSTOHandleByHandle(goHnd);
      [*] = ;               var trgHnd : Integer = TObj(pobj).trgHnd;
      [*] = ;               if (trgHnd<>0) and (_misc_GetBaseObjID(trgHnd)=gc_baseid_obj) then
      [*] = ;               begin
      [*] = ;                  var pTrgObj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;                  if (pTrgObj<>nil) and ((TObj(pTrgObj).bbuilt) and (TObj(pobj).orders[curOrd].itype<>gc_obj_order_type_repair)) and (not ((gPlayer[TObj(pObj).pl].enemyPlMask) and (gPlayer[TObj(pTrgObj).pl].myplmask)<>0)) then
      [*] = ;                  begin
      [*] = ;                     var cid : Integer = TObj(pTrgObj).cid;
      [*] = ;                     var id : Integer = TObj(pTrgObj).id;
      [*] = ;                     var pTrgObjProp : Pointer = gObjProp[cid][id];
      [*] = ;                     if TObjProp(pTrgObjProp).bbuilding or (TObjProp(pTrgObjProp).transport > 0) then
      [*] = ;                     begin
      [*] = ;                        var eCount : Integer = gCustomObjPoints[cid, id].exitCount;
      [*] = ;
      [*] = ;                        if GetGameObjectVisibleByHandle(goHnd) then
      [*] = ;                        begin
      [*] = ;                           //if ((TObj(pTrgObj).hp>=gPlayer[TObj(pTrgObj).pl].objbase[cid][id].maxhp) and (TObjProp(pTrgObjProp).peasantabsorber>0)) or (TObjProp(pTrgObjProp).transport>0) then
      [*] = ;                           if (eCount>0) and (not nopath) and ((TObjProp(pTrgObjProp).peasantabsorber>0) or (TObjProp(pTrgObjProp).transport>0)) then
      [*] = ;                           begin
      [*] = ;                              var stX : Float = GetGameObjectStateTargetPositionXByHandle(goHnd);
      [*] = ;                              var stZ : Float = GetGameObjectStateTargetPositionZByHandle(goHnd);
      [*] = ;                              for j := eCount-1 downto 0 do
      [*] = ;                              begin
      [*] = ;                                 var x, y, z : Float;
      [*] = ;                                 _unit_GetAbsExitPoint(trgHnd, j, true, x, y, z);
      [*] = ;
      [*] = ;                                 if j < eCount-1 then
      [*] = ;                                 begin
      [*] = ;                                    var tpCount : Integer = GetGameObjectTrackPointCountByHandle(goHnd);
      [*] = ;                                    if VectorDistance(stX, 0, stZ, x, 0, z) > gc_epsilon then
      [*] = ;                                    begin
      [*] = ;                                       GameObjectTrackPointAddByHandle(goHnd, x, y, z);
      [*] = ;                                       tpCount := tpCount+1;
      [*] = ;                                    end;
      [*] = ;
      [*] = ;                                    SetGameObjectTrackPointTagByIndexByHandle(goHnd, tpCount-1, 1);
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;
      [*] = ;                           TObj(pObj).bHideRequested := false;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           if (eCount>0) then
      [*] = ;                           begin
      [*] = ;                              var posX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;                              var posZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;                              var trgX : Float = GetGameObjectPositionXByHandle(trgHnd);
      [*] = ;                              var trgY : Float = GetGameObjectPositionYByHandle(trgHnd);
      [*] = ;                              var trgZ : Float = GetGameObjectPositionZByHandle(trgHnd);
      [*] = ;                              for j := 0 to eCount-1 do
      [*] = ;                              begin
      [*] = ;                                 var x, y, z : Float;
      [*] = ;                                 _unit_GetAbsExitPoint(trgHnd, j, true, x, y, z);
      [*] = ;
      [*] = ;                                 if (j < eCount-1) or noPath or (VectorDistance(posX, 0, posZ, x, 0, z) > gc_epsilon) then
      [*] = ;                                 GameObjectTrackPointInsertByHandle(goHnd, j, x, y, z);
      [*] = ;
      [*] = ;                                 SetGameObjectTrackPointTagByIndexByHandle(goHnd, j, 1);
      [*] = ;
      [*] = ;                                 if (j=0) then
      [*] = ;                                 begin
      [*] = ;                                    var ordtype : Integer = TObj(pObj).orders[0].itype;
      [*] = ;                                    if (ordtype=gc_obj_order_type_gototransport) or (ordtype=gc_obj_order_type_leavetransport) or (ordtype=gc_obj_order_type_leavebuilding) then
      [*] = ;                                    begin
      [*] = ;                                       const cFerryOffset = 2;
      [*] = ;                                       SetGameObjectCollisionDetectionByHandle(goHnd, True);
      [*] = ;                                       SetGameObjectPositionByHandle(goHnd, x, y+cFerryOffset, z);
      [*] = ;                                       GameObjectDoRayCastTerrainByHandle(goHnd);
      [*] = ;                                    end
      [*] = ;                                    else
      [*] = ;                                    SetGameObjectPositionByHandle(goHnd, x, y, z);
      [*] = ;                                    if eCount > 1 then
      [*] = ;                                    begin
      [*] = ;                                       var epx, epy, epz : Float;
      [*] = ;                                       _unit_GetAbsExitPoint(trgHnd, 1, true, epx, epy, epz);
      [*] = ;                                       _unit_SetDirection(goHnd, x-epx, z-epz);
      [*] = ;                                    end;
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;
      [*] = ;                           var tag : Integer = GetGameObjectStatesTagByHandle(goHnd) and (not gc_statetag_visual);
      [*] = ;                           SetGameObjectStatesTagByHandle(goHnd, tag or gc_statetag_visual_none);
      [*] = ;                           SetGameObjectVisibleByHandle(goHnd, true);
      [*] = ;                        end;
      [*] = ;
      [*] = ;                        if ((GetGameObjectStatesTagByHandle(goHnd) and gc_statetag_essential_birth)<>0) then
      [*] = ;                        begin
      [*] = ;                           var tpCount : Integer = GetGameObjectTrackPointCountByHandle(goHnd);
      [*] = ;                           if tpCount > 1 then
      [*] = ;                           begin
      [*] = ;                              var x1, y1, z1, x2, y2, z2 : Float;
      [*] = ;                              GetGameObjectTrackPointCoordsByIndexByHandle(goHnd, tpCount-2, x1, y1, z1);
      [*] = ;                              GetGameObjectTrackPointCoordsByIndexByHandle(goHnd, tpCount-1, x2, y2, z2);
      [*] = ;
      [*] = ;                              var angle : Float = _misc_GetAngleToXVector(x1, z1, x2, z2);
      [*] = ;                              SetGameObjectSTArrowAngleByHandle(goHnd, angle);
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;                  TObj(pObj).trghnd := 0;
      [*] = ;               end;
      [*] = ;
      [*] = ;               //ship move to water
      [*] = ;               if not landPath and _unit_IsWaterUnit(goHnd) and GetMapCollisionTag(GetGameObjectPositionXByHandle(goHnd), GetGameObjectPositionZByHandle(goHnd), false)<>gc_collisiontag_water then begin
      [*] = ;                  //log('Progress: ship move to water');
      [*] = ;                  if GetGameObjectCollisionDetectionByHandle(goHnd) then begin
      [*] = ;                     SetGameObjectCollisionDetectionByHandle(goHnd, false);
      [*] = ;                     SetGameObjectTrackPointSkipPointsByHandle(goHnd, false);
      [*] = ;                  end;
      [*] = ;                  if noPath then begin
      [*] = ;                     GameObjectTrackPointAddByHandle(goHnd, GetGameObjectPositionXByHandle(goHnd), GetGameObjectPositionYByHandle(goHnd), GetGameObjectPositionZByHandle(goHnd));
      [*] = ;                     SetGameObjectTrackPointTagByIndexByHandle(goHnd, 0, 1);
      [*] = ;                     GameObjectTrackPointAddByHandle(goHnd, GetGameObjectStateTargetPositionXByHandle(goHnd), GetGameObjectStateTargetPositionYByHandle(goHnd), GetGameObjectStateTargetPositionZByHandle(goHnd));
      [*] = ;                     SetGameObjectTrackPointTagByIndexByHandle(goHnd, 1, 1);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;
      [*] = ;               var tpCount : Integer = GetGameObjectTrackPointCountByHandle(goHnd);
      [*] = ;               if tpCount > 0 then
      [*] = ;               begin
      [*] = ;                  //if (_unit_GetTagStateByType(goHnd, gc_statetag_move)=gc_statetag_move_walk) then
      [*] = ;                  begin
      [*] = ;                     SetGameObjectTargetRotatingModeByHandle(goHnd, 'trmNone');
      [*] = ;                     DoSetupMoveAnimation(goHnd); // sometime TIntegerList(pList).GetCount decreased after this
      [*] = ;                     SetGameObjectTrackPointCurrentPointIndexByHandle(goHnd, 0);
      [*] = ;                     SetGameObjectTrackPointCurrentPointIndexByHandle(goHnd, 1);
      [*] = ;                     //SetGameObjectTrackPointMovementModeByHandle(goHnd, 'mmDoTrackOnce');
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  if _unit_GetTagStateByType(goHnd, gc_statetag_move) = gc_statetag_move_walk then
      [*] = ;                  _unit_Stop(goHnd);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      TIntegerList(pList).Clear;
      [*] = ;   end;
      [*] = ;
      [*] = ;   var miscplhnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;   var miscplcount : Integer = GetPlayerGameObjectsCountByHandle(miscplhnd);
      [*] = ;   if gametime>gProgress.lastmiscplsecmaxtime then
      [*] = ;   begin
      [*] = ;      gProgress.lastmiscplsecmaxtime := gametime+gc_progress_TimeMiscPlSecMax;
      [*] = ;      // section max for misc player. actually, it is now almost empty
      [*] = ;      var count : Integer = miscplcount;
      [*] = ;      var secmax : Integer = 100;
      [*] = ;      if (count>700) then
      [*] = ;      begin
      [*] = ;         if (count<2000) then
      [*] = ;         secmax := secmax+(count div 20)
      [*] = ;         else
      [*] = ;         if (count<4000) then
      [*] = ;         secmax := secmax+(count div 15)
      [*] = ;         else
      [*] = ;         secmax := secmax+(count div 10);
      [*] = ;      end;
      [*] = ;      var curmax : Integer = GetPlayerProgressSectionMaxByHandle(miscplhnd);
      [*] = ;      if (Abs(secmax-curmax)>20) then
      [*] = ;      SetPlayerProgressSectionMaxByHandle(miscplhnd, secmax);
      [*] = ;   end;
      [*] = ;
      [*] = ;   var poolplhnd : Integer = GetPlayerHandleByName('@pool');
      [*] = ;   if (poolplhnd<>0) then
      [*] = ;   begin
      [*] = ;      var poolplcount : Integer = GetPlayerGameObjectsCountByHandle(poolplhnd);
      [*] = ;      var passivecount : Integer;
      [*] = ;      var i : Integer;
      [*] = ;      for i:=GetPlayerCountOfGroupsByHandle(poolplhnd)-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var grhnd : Integer = GetGRHandleByPlayerHandleByIndex(poolplhnd, i);
      [*] = ;         if (grhnd<>0) then
      [*] = ;         passivecount := passivecount+GetGroupCountGameObjectsByHandle(grhnd);
      [*] = ;      end;
      [*] = ;      var count : Integer = poolplcount-passivecount;
      [*] = ;      // section max for @pool player
      [*] = ;      if gametime>gProgress.lastpoolplsecmaxtime then
      [*] = ;      begin
      [*] = ;         gProgress.lastpoolplsecmaxtime := gametime+gc_progress_TimePoolPlSecMax;
      [*] = ;         var secmax : Integer = 250;
      [*] = ;         if (count>700) then
      [*] = ;         begin
      [*] = ;            if (count<2000) then
      [*] = ;            secmax := secmax+(count div 20)
      [*] = ;            else
      [*] = ;            if (count<4000) then
      [*] = ;            secmax := secmax+(count div 15)
      [*] = ;            else
      [*] = ;            secmax := secmax+(count div 10);
      [*] = ;         end;
      [*] = ;         var curmax : Integer = GetPlayerProgressSectionMaxByHandle(poolplhnd);
      [*] = ;         if (Abs(secmax-curmax)>20) then
      [*] = ;         SetPlayerProgressSectionMaxByHandle(poolplhnd, secmax);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if gametime>gProgress.lastsearchenemycountertime then
      [*] = ;   begin
      [*] = ;      gProgress.lastsearchenemycountertime := gametime+gc_progress_TimeSearchEnemyCounter;
      [*] = ;      //gfloat_searchenemygridcachetime := 0.25;
      [*] = ;
      [*] = ;      var progressmax : Integer = GetPlayerProgressSectionMax;;
      [*] = ;      if gametime>gProgress.lastsearchenemycountmidtime then
      [*] = ;      begin
      [*] = ;         gProgress.lastsearchenemycountmidtime := gametime+gc_progress_TimeSearchEnemyCountMid;
      [*] = ;
      [*] = ;         gfloat_searchenemy_mid := (gfloat_searchenemy_mid+(gint_searchenemy_count/(gint_searchenemy_progress+1)))/2;
      [*] = ;         if (gfloat_searchenemy_mid>40) then
      [*] = ;         gint_searchenemy_skiptickcount := floor(gfloat_searchenemy_mid/30)
      [*] = ;         else
      [*] = ;         gint_searchenemy_skiptickcount := 0;
      [*] = ;         gint_searchenemy_count := 0;
      [*] = ;         gint_searchenemy_progress := 0;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      gfloat_searchenemy_mid := (gint_searchenemy_count/(gint_searchenemy_progress+1));
      [*] = ;      gint_searchenemy_progress := gint_searchenemy_progress+1;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if ((gProgress.progresstick mod 53)=0) and (gLanSyncUnitsParamsUIDList.GetCount>0) and ((_net_IsOnline and _net_IsServer) or (_net_IsRecord)) then
      [*] = ;   _misc_SyncUnitsParams;
      [*] = ;
      [*] = ;   //if ((gProgress.progresstick mod 10)=0) then
      [*] = ;   //gProgress.lastsearchenemycountsumtime
      [*] = ;   begin
      [*] = ;      var weight : array [0..gc_MaxPlayerCount-1] of Float;
      [*] = ;      var weightsum : Float;
      [*] = ;      var searchenemysum : Integer;
      [*] = ;
      [*] = ;      var prevcount : Integer = GetPlayerProgressSectionMax;
      [*] = ;      var newcount : Integer = 500;
      [*] = ;      if (prevcount<>newcount) then
      [*] = ;      SetPlayerProgressSectionMax(newcount);
      [*] = ;
      [*] = ;      const cProgressCheckTime = 1;
      [*] = ;
      [*] = ;      var existplcount : Integer;
      [*] = ;      var i : Integer;
      [*] = ;      for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;      begin
      [*] = ;         var searchenemydelta : Integer;
      [*] = ;         if (gPlayer[i].searchenemyprev<0) or (gPlayer[i].searchenemylast<0) or (gPlayer[i].searchenemycur<0) then
      [*] = ;         begin
      [*] = ;            gPlayer[i].searchenemyprev := 0;
      [*] = ;            gPlayer[i].searchenemylast := 0;
      [*] = ;            gPlayer[i].searchenemycur := 0;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (gametime-gPlayer[i].searchenemylasttime)>cProgressCheckTime then
      [*] = ;         begin
      [*] = ;            gPlayer[i].searchenemyprev := gPlayer[i].searchenemylast;
      [*] = ;            gPlayer[i].searchenemylast := gPlayer[i].searchenemycur;
      [*] = ;            searchenemydelta := gPlayer[i].searchenemylast-gPlayer[i].searchenemyprev;
      [*] = ;
      [*] = ;            if searchenemydelta>0 then
      [*] = ;            searchenemysum := searchenemysum+searchenemydelta;
      [*] = ;
      [*] = ;            weight[i] := searchenemydelta/gc_progress_searchenemyweight;
      [*] = ;            if (gPlayer[i].bai) then
      [*] = ;            weight[i] := floor(weight[i]*0.75);
      [*] = ;            weightsum := weightsum+weight[i];
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if (cycletime>0) then
      [*] = ;      begin
      [*] = ;         var arrProgress : array [0..gc_MaxPlayerCount-1] of Integer;
      [*] = ;         const cIntervalOutOfScreenWar = 0.2;
      [*] = ;         const cIntervalOutOfScreenPeace = 0.6;
      [*] = ;         var interval : float = gc_statemachine_interval_units/1000;
      [*] = ;         var bprocess : Boolean;
      [*] = ;         var maxperc : Float;
      [*] = ;         for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;         begin
      [*] = ;            var delta : Float = gametime-gPlayer[i].searchenemylasttime;
      [*] = ;            if (delta>cProgressCheckTime) or (delta<0) then
      [*] = ;            begin
      [*] = ;               bprocess := True;
      [*] = ;               var fWarFactor : Float = Clamp(weight[i]/20, 0, 1);
      [*] = ;               var maxtime : Float = Lerp(cIntervalOutOfScreenPeace, cIntervalOutOfScreenWar, fWarFactor);
      [*] = ;               var cycles : Float = (maxtime-interval)/cycletime;
      [*] = ;               if (cycles<=0) then
      [*] = ;               cycles := 1;
      [*] = ;
      [*] = ;               var plhnd : Integer = GetPlayerHandleByIndex(i);
      [*] = ;               var curmax : Integer = GetPlayerProgressSectionMaxByHandle(plhnd);
      [*] = ;               var secmax : Integer;
      [*] = ;
      [*] = ;               var psgocount, peacount, warcount : Integer;
      [*] = ;               var psind : Integer = GetPlayerProgressSectionIndexByInterval(plHnd, gc_statemachine_interval_peasants);
      [*] = ;               if (psind>=0) then
      [*] = ;               peacount := GetPlayerProgressSectionCountGOByIndex(plHnd, psind);
      [*] = ;
      [*] = ;               psind := GetPlayerProgressSectionIndexByInterval(plHnd, gc_statemachine_interval_units);
      [*] = ;               if (psind>=0) then
      [*] = ;               warcount := GetPlayerProgressSectionCountGOByIndex(plHnd, psind);
      [*] = ;
      [*] = ;               psgocount := Max(peacount, warcount);
      [*] = ;
      [*] = ;               secmax := _misc_RoundUp(psgocount/cycles){+15};
      [*] = ;               secmax := Max(50, Min(psgocount, secmax));
      [*] = ;
      [*] = ;               var change : Integer = secmax-curmax;
      [*] = ;               if (change>0) then
      [*] = ;               secmax := curmax+Max(change, 20)
      [*] = ;               else
      [*] = ;               if (change<-30) then
      [*] = ;               secmax := curmax-Max(40, round(-change/1.5));
      [*] = ;
      [*] = ;               if (change>0) or (change<-30) then
      [*] = ;               arrProgress[i] := secmax
      [*] = ;               else
      [*] = ;               arrProgress[i] := curmax;
      [*] = ;
      [*] = ;               if {(fWarFactor>=1) and} (warcount>0) and (secmax>200) then
      [*] = ;               maxperc := MaxFloat(maxperc, arrProgress[i]/warcount);
      [*] = ;
      [*] = ;               gPlayer[i].searchenemylasttime := gametime;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         if (bprocess) then
      [*] = ;         begin
      [*] = ;            if ((_net_IsServer) or ((_net_IsOffline) and (not _net_IsReplay))) then
      [*] = ;            begin
      [*] = ;               var speed : Float = 10;
      [*] = ;               case gProfile.igamespeed of
      [*] = ;                  0 : speed := gc_settings_gamespeed_0;
      [*] = ;                  1 : speed := gc_settings_gamespeed_1;
      [*] = ;                  2 : speed := gc_settings_gamespeed_2;
      [*] = ;               end;
      [*] = ;               var curspeed : Float = GetTimeSpeedFactor;
      [*] = ;
      [*] = ;               if (((gametime-gfloat_progressspeedfactor_lasttimechange)>0.25) or (gfloat_progressspeedfactor_lasttimechange>gametime)) then
      [*] = ;               begin
      [*] = ;                  gfloat_progressspeedfactor_lasttimechange := gametime;
      [*] = ;                  const cDeltaFactor = 0.33;
      [*] = ;                  var realfactor : Float = (maxperc-cDeltaFactor)*(1/(1-cDeltaFactor));
      [*] = ;                  var factor : Float = Clamp(realfactor, 0, 1);
      [*] = ;
      [*] = ;                  var curfactor : Float = gfloat_progressspeedfactor_lastvalue;
      [*] = ;                  var newfactor : Float;
      [*] = ;                  //if (curfactor<>factor) then
      [*] = ;                  begin
      [*] = ;                     const cStep = 0.2;
      [*] = ;                     var deltafactor : Float = (factor-curfactor);
      [*] = ;                     if (deltafactor>0) then
      [*] = ;                     begin
      [*] = ;                        if (deltafactor>cStep) then
      [*] = ;                        deltafactor := cStep;
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     if (deltafactor<0) then
      [*] = ;                     begin
      [*] = ;                        if (deltafactor<-cStep) then
      [*] = ;                        deltafactor := -cStep;
      [*] = ;                     end;
      [*] = ;                     newfactor := Clamp(curfactor+deltafactor, 0, 1);
      [*] = ;                     //if (Abs(curfactor-newfactor)>0.09) or (newfactor=1) then
      [*] = ;                     begin
      [*] = ;                        gfloat_progressspeedfactor_lastvalue := newfactor;
      [*] = ;
      [*] = ;                        var newspeed : Float = speed*(1-newfactor);
      [*] = ;                        newspeed := Clamp(newspeed, 5, gc_settings_gamespeed_2);
      [*] = ;
      [*] = ;                        const cSpeedStepUp = 0.333*2;
      [*] = ;                        const cSpeedStepDown = cSpeedStepUp;
      [*] = ;                        if newspeed>curspeed and (newspeed-curspeed)>cSpeedStepUp then
      [*] = ;                        newspeed := curspeed+(newspeed-curspeed)*cSpeedStepUp
      [*] = ;                        else
      [*] = ;                        if newspeed<curspeed and (curspeed-newspeed)>cSpeedStepDown then
      [*] = ;                        newspeed := curspeed-(curspeed-newspeed)*cSpeedStepDown;
      [*] = ;
      [*] = ;                        newspeed := MaxFloat(newspeed, 5);
      [*] = ;                        newspeed := MinFloat(newspeed, gc_settings_gamespeed_2);
      [*] = ;                        //if (newspeed<>curspeed) then
      [*] = ;                        //Log('newspeed = '+FloatToStr(newspeed)+' ; curspeed = '+FloatToStr(curspeed));
      [*] = ;
      [*] = ;                        var pr, pp : Float;
      [*] = ;                        const cCheckFrames = 16;
      [*] = ;                        for i:=0 to cCheckFrames-1 do
      [*] = ;                        pr := pr+GetPerfRender(i);
      [*] = ;                        pr := (pr/cCheckFrames);
      [*] = ;
      [*] = ;                        var progfps : Float;
      [*] = ;                        for i:=0 to gc_perf_progresshistory-1 do
      [*] = ;                        progfps := progfps+garrfloat_perf_progress[i];
      [*] = ;                        progfps := progfps/gc_perf_progresshistory;
      [*] = ;
      [*] = ;                        var pt : Float = progfps;
      [*] = ;                        var ppadaptive : Float = 1/pt*(curspeed/(MinFloat(curspeed+1, speed)));
      [*] = ;                        pt := 1/pt;
      [*] = ;                        pr := 1/pr;
      [*] = ;                        pp := 1/progfps;
      [*] = ;
      [*] = ;                        //Log('newspeed = '+FloatToStr(newspeed)+' realfps='+FloatToStr(realfps)+' pt='+FloatToStr(pt)+' pr='+FloatToStr(pr)+' ppadaptive='+FloatToStr(ppadaptive));
      [*] = ;
      [*] = ;                        if {(newfactor>0.4) and} (realfps<20) then
      [*] = ;                        begin
      [*] = ;                           if (ppadaptive>(pr*2)) then
      [*] = ;                           newspeed := MinFloat(curspeed+1, speed);
      [*] = ;                           for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;                           if (arrProgress[i]>0) then
      [*] = ;                           begin
      [*] = ;                              var plhnd : Integer = GetPlayerHandleByIndex(i);
      [*] = ;                              var curmax : Integer = GetPlayerProgressSectionMaxByHandle(plhnd);
      [*] = ;                              var newprog : Integer = _misc_RoundUp(arrProgress[i]*(1-(1-realfps/20)));
      [*] = ;                              var change : Integer = newprog-curmax;
      [*] = ;                              if (change>0) then
      [*] = ;                              newprog := curmax+Max(change, 20)
      [*] = ;                              else
      [*] = ;                              if (change<-30) then
      [*] = ;                              newprog := Max(50, curmax-Max(40, round(-change/1.35)));
      [*] = ;                              if (Abs(change)>30) or (curmax<50) then
      [*] = ;                              SetPlayerProgressSectionMaxByHandle(plhnd, newprog);
      [*] = ;                           end;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;                           if arrProgress[i]>0 then
      [*] = ;                           SetPlayerProgressSectionMaxByHandle(GetPlayerHandleByIndex(i), arrProgress[i]);
      [*] = ;                        end;
      [*] = ;                        if (factor<0.2) then
      [*] = ;                        newspeed := MinFloat(curspeed+1, speed);
      [*] = ;                        if (newspeed<>GetTimeSpeedFactor) then
      [*] = ;                        begin
      [*] = ;                           if (_net_IsServer) then
      [*] = ;                           begin
      [*] = ;                              {FloatRegister0 := newspeed;
      [*] = ;                              IntRegister0 := gProfile.igamespeed;
      [*] = ;                              GUIExecuteState('WriteTimeSpeed');}
      [*] = ;                              SetTimeSpeedFactor(newspeed);
      [*] = ;                              var pLan : Integer = _parser_ParserTemporary(True);
      [*] = ;                              ParserSetFloatValueByKeyByHandle(pLan, 't', GetGameTime);
      [*] = ;                              ParserSetFloatValueByKeyByHandle(pLan, 's', newspeed);
      [*] = ;                              LanSendParser(gc_LAN_GAME_SYNC_GAMETIME, pLan);
      [*] = ;                              gfloat_gui_lanlastgametimesync := _misc_GetRealTime;
      [*] = ;                           end
      [*] = ;                           else
      [*] = ;                           if not gProfile.bdevmode then
      [*] = ;                           SetTimeSpeedFactor(newspeed);
      [*] = ;                           //Log('SetTimeSpeedFactor('+FloatToStr(newspeed)+')');
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            if (_net_IsClient) or (_net_IsReplay) then
      [*] = ;            begin
      [*] = ;               for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;               begin
      [*] = ;                  var plhnd : Integer = GetPlayerHandleByIndex(i);
      [*] = ;                  if (arrProgress[i]>0) and (arrProgress[i]<>GetPlayerProgressSectionMaxByHandle(plhnd)) then
      [*] = ;                  SetPlayerProgressSectionMaxByHandle(plhnd, arrProgress[i]);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   _misc_CheckEndGame;
      [*] = ;
      [*] = ;   if gInt_UpdateAreasCount > 0 then
      [*] = ;   begin
      [*] = ;      var i : Integer;
      [*] = ;      for i := gInt_UpdateAreasCount-1 downto 0 do
      [*] = ;      _misc_UpdateTopology(gUpdateAreas[i].minX, gUpdateAreas[i].minY, gUpdateAreas[i].maxX, gUpdateAreas[i].maxY);
      [*] = ;
      [*] = ;      gInt_UpdateAreasCount := 0;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (gbool_peacemode) and (gametime > gfloat_peacetime) and (not _net_IsReplay) and (not _net_IsClient) then
      [*] = ;   begin
      [*] = ;      gbool_peacemode := false;
      [*] = ;      if (GetCountOfPlayers>gc_playerind_misc) then
      [*] = ;      begin
      [*] = ;         var plHnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;         if plHnd <> 0 then
      [*] = ;         begin
      [*] = ;            gIntegerList.Clear;
      [*] = ;            var i : Integer;
      [*] = ;            for i := GetPlayerGameObjectsCountByHandle(plHnd)-1 downto 0 do
      [*] = ;            begin
      [*] = ;               var goHnd : Integer = GetGameObjectHandleByIndex(i, plHnd);
      [*] = ;               if GetGameObjectStringPropertyTag(goHnd)=gc_properties_stringtag_ptborder then
      [*] = ;               gIntegerList.Add(goHnd);
      [*] = ;            end;
      [*] = ;            if (gIntegerList.GetCount>0) then
      [*] = ;            begin
      [*] = ;               var plHnd : Integer = GetPlayerHandleByIndex(0); // ask 0 player to do all job
      [*] = ;               var pArg: Pointer = StateMachineGetArgDataByInd(GetPlayerStateMachineHandle(plHnd), gc_argplayer_plr);
      [*] = ;               if pArg<>nil then begin
      [*] = ;                  // Destroy ptborder objects
      [*] = ;                  TPlayerArgs(pArg).fintlst := gIntegerList;
      [*] = ;                  PlayerExecuteStateByHandle(plHnd, 'WriteFreeList');
      [*] = ;
      [*] = ;                  // Switch peace mode
      [*] = ;                  TPlayerArgs(pArg).fbstate := gbool_peacemode;
      [*] = ;                  PlayerExecuteStateByHandle(plHnd, 'WritePeaceTime');
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            for i:=0 to gIntegerList.GetCount-1 do
      [*] = ;            GameObjectRequestToDestroyByHandle(gIntegerList.Get(i));
      [*] = ;
      [*] = ;            GUIMiniMapPrimitivesClear;
      [*] = ;            _misc_UpdateMiniMapPrimitives(True);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   var curtime : Float = GetCurrentTime;
      [*] = ;   if (curtime-gfloat_lan_lastsyncrestime)>0.1 then
      [*] = ;   begin
      [*] = ;      gfloat_lan_lastsyncrestime := curtime;
      [*] = ;      ExecuteState('WriteRes');
      [*] = ;   end;
      [*] = ;   if (curtime-gfloat_lan_lastsyncstatstime)>20 then
      [*] = ;   begin
      [*] = ;      gfloat_lan_lastsyncstatstime := curtime;
      [*] = ;      ExecuteState('WriteStats');
      [*] = ;   end;
      [*] = ;   if (curtime-gfloat_lan_lastsyncdatatime)>0.1 then
      [*] = ;   begin
      [*] = ;      gfloat_lan_lastsyncdatatime := curtime;
      [*] = ;      gbool_net_forcesyncdata := true;
      [*] = ;      ExecuteState('WriteLanSyncData');
      [*] = ;   end;
      [*] = ;
      [*] = ;   // progress statistics
      [*] = ;   if gametime>gProgress.lastprogressstatisticstime then
      [*] = ;   begin
      [*] = ;      gProgress.lastprogressstatisticstime := gametime+gc_progress_TimeProgressStatistics;
      [*] = ;      const cInterval = 5; // 5 seconds
      [*] = ;      var i, j : Integer;
      [*] = ;      for i:=0 to gc_MaxPlayerCount-2 do // -2, no need to count mercenary player
      [*] = ;      begin
      [*] = ;         var count : Integer = gPlayer[i].stat.population.GetCount;
      [*] = ;         var nexttime : Integer = count*cInterval;
      [*] = ;         if (gametime>=nexttime) then
      [*] = ;         begin
      [*] = ;            var delta : Integer = floor(gametime-nexttime);
      [*] = ;            var enum : Integer = delta div cInterval;
      [*] = ;            if (enum>31) then
      [*] = ;            begin
      [*] = ;               gPlayer[i].stat.population.AdjustCapacityToAtLeast(count+enum);
      [*] = ;               gPlayer[i].stat.scores.AdjustCapacityToAtLeast(count+enum);
      [*] = ;            end;
      [*] = ;            for j:=0 to enum do
      [*] = ;            begin
      [*] = ;               var popul : Integer = gPlayer[i].counter.farmused;
      [*] = ;               var score : Integer = gPlayer[i].counter.scores;
      [*] = ;               gPlayer[i].stat.population.Add(popul);
      [*] = ;               gPlayer[i].stat.scores.Add(score);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if _misc_IsProcessAI{_net_IsOffline or _net_IsServer} then
      [*] = ;begin
      [*] = ;   if gametime>gProgress.lastprogresstopzonestime then
      [*] = ;   begin
      [*] = ;      gProgress.lastprogresstopzonestime := gametime+gc_progress_TimeProgressTopZones;
      [*] = ;      _misc_UpdateTopZonesInfo;
      [*] = ;   end;
      [*] = ;
      [*] = ;   _ai_CheckProcessEnemyInfo(gProgress.progressTick);
      [*] = ;end;
      [*] = ;
      [*] = ;if (lastprogresstime>0) and (deltatime>0) then // not on pause
      [*] = ;gProgress.progresstick := gProgress.progresstick + 1;
      [*] = ;gProgress.lastprogresstime := gametime;
   struct.end
section.end

