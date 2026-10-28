section.begin
   Name = Progress
   UsedFileName = 
   Code : struct.begin
      [*] = ;procedure CheckSquadsDisband(plInd : Integer);
      [*] = ;begin
      [*] = ;   var i, j : Integer;
      [*] = ;   for i := gPlayer[plInd].squads.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var pSquad : Pointer = gPlayer[plInd].squads.Get(i);
      [*] = ;      var plHnd : Integer = GetPlayerHandleByIndex(plInd);
      [*] = ;      var basecount : Integer = TSquad(psquad).fBaseCount;
      [*] = ;      var count : Integer = _squad_GetBaseUnitCount(pSquad);
      [*] = ;
      [*] = ;      if (count<basecount*gc_player_SquadDismissPercent) then
      [*] = ;      begin
      [*] = ;         var select : Boolean;
      [*] = ;         if (plHnd=GetPlayerHandleInterfaceIO) and (TSquad(psquad).fSelected) then
      [*] = ;         begin
      [*] = ;            select := true;
      [*] = ;            gIntegerList.Clear;
      [*] = ;            var j : Integer;
      [*] = ;            for j := 0 to count-1 do
      [*] = ;            begin
      [*] = ;               var goHnd : Integer = TSquad(psquad).Get(j);
      [*] = ;               gIntegerList.Add(goHnd);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         _misc_DisbandSquad(plHnd, i, true);
      [*] = ;         if select then
      [*] = ;         _misc_AddUnitsToSelection(gIntegerList);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;var lastprogresstime : Float = gPlayer[plind].lastprogresstime;
      [*] = ;var deltatime : Float = GetGameTime-lastprogresstime;
      [*] = ;
      [*] = ;if (lastprogresstime>0) and (deltatime>0) then
      [*] = ;begin
      [*] = ;   var gametime : Float = GetGameTime;
      [*] = ;   if (_net_IsClient or _net_IsReplay) then
      [*] = ;   begin
      [*] = ;      // global do not progress when client or replay
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      if (gPlayer[plInd].lastattacktime<>0) and ((gPlayer[plInd].lastattacktime<GetCurrentTime) or (GetCurrentTime-gPlayer[plInd].lastattacktime)>(gc_gui_battlemusicinterval-gc_gui_underattackalarminterval)) then
      [*] = ;      gPlayer[plInd].lastattacktime := 0;
      [*] = ;      if (not gScenario.bactive) and (gMap.gamestage>gc_map_gamestage_waitingloading) and ((gInterface.gamemode=gc_gamemode_game) or (gInterface.gamemode=gc_gamemode_spectator)) then
      [*] = ;      begin
      [*] = ;         var addtime : Float;
      [*] = ;         if (gPlayer[plind].bexists) then
      [*] = ;         addtime := (0.125*plInd)+gc_global_TimeCheckExists*4
      [*] = ;         else
      [*] = ;         addtime := (0.125*plInd)+gc_global_TimeCheckExists*12;
      [*] = ;         if (_player_CheckProgressLastTimeAndIncrease(gPlayer[plInd].lastcheckexists, 0, addtime)) then
      [*] = ;         begin
      [*] = ;            var bnetsurrender : Boolean = (_net_IsServer) {and (gMap.players[plind].bhuman)} and (gMap.players[plind].bleave);
      [*] = ;            var bsurrender : Boolean = (_net_IsOffline) and (gPlayer[plind].victorystate=gc_player_victorystate_lose);
      [*] = ;            if (gMap.bbattle) then
      [*] = ;            begin
      [*] = ;               gPlayer[plind].bexists := True;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               if (not bnetsurrender) and (not bsurrender) then
      [*] = ;               begin
      [*] = ;                  var bPrevExists : Boolean = gPlayer[plind].bexists;
      [*] = ;                  var farmused : Integer = gPlayer[plInd].counter.farmused;
      [*] = ;                  //var bLeaver : Boolean = ((gMap.players[plInd].bexists) and (gMap.players[plInd].bhuman) and (gMap.players[plInd].bleave));
      [*] = ;
      [*] = ;                  var peasecind : Integer = GetPlayerProgressSectionIndexByInterval(plHnd, gc_statemachine_interval_peasants);
      [*] = ;                  var peaseccount : Integer;
      [*] = ;                  if (peasecind>=0) then
      [*] = ;                  peaseccount := GetPlayerProgressSectionCountGOByIndex(plHnd, peasecind);
      [*] = ;                  var bnopea : Boolean = (peaseccount=0);
      [*] = ;
      [*] = ;                  var bldsecind : Integer = GetPlayerProgressSectionIndexByInterval(plHnd, gc_statemachine_interval_buildings);
      [*] = ;                  var bldseccount : Integer;
      [*] = ;                  if (bldsecind>=0) then
      [*] = ;                  bldseccount := GetPlayerProgressSectionCountGOByIndex(plHnd, bldsecind);
      [*] = ;                  var bnobld : Boolean = (bldseccount=0);
      [*] = ;
      [*] = ;                  if {(not gMap.players[plInd].bhuman) and }(farmused>0) and (farmused<100) then
      [*] = ;                  begin
      [*] = ;                     gPlayer[plind].bexists := False;
      [*] = ;                     var i : Integer;
      [*] = ;                     for i:=peaseccount-1 downto 0 do
      [*] = ;                     begin
      [*] = ;                        var goHnd : Integer = GetPlayerProgressSectionGOHandleByIndex(plHnd, peasecind, i);
      [*] = ;                        var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                        if (pobj<>nil) then
      [*] = ;                        begin
      [*] = ;                           var statetag : Integer = GetGameObjectStatesTagByHandle(goHnd);
      [*] = ;                           if (((statetag and gc_statetag_visual_none)<>0) and ((statetag and gc_statetag_essential_none)<>0)) or ((gMap.players[plInd].bhuman) and (TObj(pobj).insideofuid<>0)) then
      [*] = ;                           begin
      [*] = ;                              gPlayer[plind].bexists := True;
      [*] = ;                              break;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                     if not gPlayer[plind].bexists then
      [*] = ;                     begin
      [*] = ;                        for i:=bldseccount-1 downto 0 do
      [*] = ;                        begin
      [*] = ;                           var goHnd : Integer = GetPlayerProgressSectionGOHandleByIndex(plHnd, bldsecind, i);
      [*] = ;                           var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;                           var pobjprop : Pointer = _unit_GetObjProp(goHnd);
      [*] = ;                           if (pobj<>nil) and (TObjProp(pobjprop).bbuilding) and (TObjProp(pobjprop).usage=gc_obj_usage_center) and (TObj(pobj).bbuilt) then
      [*] = ;                           begin
      [*] = ;                              gPlayer[plind].bexists := True;
      [*] = ;                              break;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  gPlayer[plind].bexists := (farmused>0);
      [*] = ;                  if (not gPlayer[plind].bexists) and ((gPlayer[plind].bai) or (gMap.players[plind].bhuman)) then
      [*] = ;                  begin
      [*] = ;                     if (GetPlayerGameObjectsCountByHandle(plHnd)>0) then
      [*] = ;                     _misc_KillPlayerUnits(plHnd);
      [*] = ;                     if (bPrevExists) then
      [*] = ;                     begin
      [*] = ;                        gPlayer[plind].victorystate := gc_player_victorystate_lose;
      [*] = ;                        if (gPlayer[plind].victorystategametime=0) then
      [*] = ;                        gPlayer[plind].victorystategametime := GetGameTime;
      [*] = ;                        _misc_LanServerSendResults;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               if (GetPlayerGameObjectsCountByHandle(plHnd)>0) then
      [*] = ;               _misc_KillPlayerUnits(plHnd);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if (plind<gc_MaxPlayerCount-1) then
      [*] = ;      begin
      [*] = ;         _player_ProcessResourceIncome(plind, deltatime);
      [*] = ;         _player_ProcessResourceConsume(plind, deltatime);
      [*] = ;      end;
      [*] = ;
      [*] = ;      if (not gPlayer[plInd].bAI) then
      [*] = ;      begin
      [*] = ;         if (_player_CheckProgressLastTimeAndIncrease(gPlayer[plInd].lastcalcsquadsmovecounttime, 1, (0.001*plInd)+gc_global_TimeCalcSquadsMoveCount)) then
      [*] = ;         _player_CalcSquadsMoveCount(plInd);
      [*] = ;      end;
      [*] = ;
      [*] = ;      var i : Integer;
      [*] = ;      for i:=gPlayer[plInd].squads.GetCount-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var pSquad : Pointer = gPlayer[plInd].squads.Get(i);
      [*] = ;         if (TSquad(pSquad).fStandGround) and (not TSquad(pSquad).fHoldMode) and (not TSquad(pSquad).fAgressive) and (TSquad(pSquad).fMoveCount=0) then
      [*] = ;         begin
      [*] = ;            if ((TSquad(pSquad).fAddDamageHold>0) or (TSquad(pSquad).fAddShieldHold>0)) and ((TSquad(pSquad).fAddDamageHold<>TSquad(pSquad).fAddDamage) or (TSquad(pSquad).fAddShieldHold<>TSquad(pSquad).fAddShield)) then
      [*] = ;            begin
      [*] = ;               TSquad(pSquad).fHoldModeProgress := TSquad(pSquad).fHoldModeProgress+(deltatime/gc_squad_holdmode_time);
      [*] = ;               if (TSquad(pSquad).fHoldModeProgress>=1) then
      [*] = ;               begin
      [*] = ;                  TSquad(pSquad).fHoldModeProgress := 0;
      [*] = ;                  TSquad(pSquad).fHoldMode := True;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if (not gPlayer[plind].bai) then
      [*] = ;      CheckSquadsDisband(plind);
      [*] = ;
      [*] = ;      if (gPlayer[plind].bexists) and ((gPlayer[plind].bai) or ((gPlayer[plind].playeradviser.adviser.bavailable) and (gPlayer[plind].playeradviser.adviser.benable))) then
      [*] = ;      ExecuteState('ProgressAI');
      [*] = ;   end;
      [*] = ;
      [*] = ;   gPlayer[plind].progresstick := gPlayer[plind].progresstick+1;
      [*] = ;end;
      [*] = ;
      [*] = ;gPlayer[plind].lastprogresstime := GetGameTime;
   struct.end
section.end

