section.begin
   Name = InitMapHistoricalBattle
   Code : struct.begin
      [*] = ;Log('InitMapHistoricalBattle: Start');
      [*] = ;
      [*] = ;procedure FixServicePlayers();
      [*] = ;begin
      [*] = ;   var hnd : Integer = GetPlayerHandleByName(gc_racename_misc);
      [*] = ;   if (hnd<>0) then
      [*] = ;   begin
      [*] = ;      SetPlayerControlModeByHandle(hnd, 'cmPC');
      [*] = ;      _player_ResetSaveLoadStates(hnd);
      [*] = ;      var i : Integer;
      [*] = ;      for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;      AddPlayerEnemyPlayerByHandle(hnd, GetPlayerHandleByIndex(i));
      [*] = ;   end;
      [*] = ;
      [*] = ;   hnd := GetPlayerHandleByName(gc_racename_progress);
      [*] = ;   if (hnd<>0) then
      [*] = ;   begin
      [*] = ;      SetPlayerControlModeByHandle(hnd, 'cmPC');
      [*] = ;      SetPlayerProgressOnLanByHandle(hnd, True);
      [*] = ;      _player_ResetSaveLoadStates(hnd);
      [*] = ;      SetPlayerProgressOnEditByHandle(hnd, true);
      [*] = ;   end;
      [*] = ;
      [*] = ;   hnd := GetPlayerHandleByName(gc_plname_pool);
      [*] = ;   if (hnd<>0) then
      [*] = ;   begin
      [*] = ;      SetPlayerControlModeByHandle(hnd, 'cmPC');
      [*] = ;      _player_ResetSaveLoadStates(hnd);
      [*] = ;      var i : Integer;
      [*] = ;      for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;      AddPlayerEnemyPlayerByHandle(hnd, GetPlayerHandleByIndex(i));
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;procedure CreatePlayers();
      [*] = ;begin
      [*] = ;   var i: Integer;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do begin
      [*] = ;      if GetPlayerHandleByIndex(i)=0 then
      [*] = ;      _player_AddGamePlayer
      [*] = ;      else begin
      [*] = ;         var hnd: Integer = _player_GetOrCreateGamePlayerByIndex(i);
      [*] = ;         ClearEnemyPlayersByHandle(hnd);  //TODO: GEC?
      [*] = ;         ClearFriendPlayersByHandle(hnd); //TODO: GEC?
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   // Create env nature player, with wood, food, stone, mine objects. Player should be neutral.
      [*] = ;   _player_AddServicePlayers;
      [*] = ;   FixServicePlayers;
      [*] = ;end;
      [*] = ;
      [*] = ;_misc_ResetProgressPlayer;
      [*] = ;
      [*] = ;CreatePlayers;
      [*] = ;
      [*] = ;var i, j : Integer;
      [*] = ;for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;begin
      [*] = ;   gPlayer[i].enemyplmask := 0;
      [*] = ;   var myteam: Integer = gMap.players[i].team;
      [*] = ;   for j:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   begin
      [*] = ;      if (i<>j) then
      [*] = ;      begin
      [*] = ;         var histeam: Integer = gMap.players[j].team;
      [*] = ;         if (myteam<>histeam) or (histeam=0) or (myteam=0) then
      [*] = ;         begin
      [*] = ;            gPlayer[i].enemyplmask:=gPlayer[i].enemyplmask or (1 shl j);
      [*] = ;            AddPlayerEnemyPlayerByHandle(GetPlayerHandleByIndex(i), GetPlayerHandleByIndex(j));
      [*] = ;         end
      [*] = ;         else
      [*] = ;         AddPlayerFriendPlayerByHandle(GetPlayerHandleByIndex(i), GetPlayerHandleByIndex(j));
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;_player_FixPlayersObjBaseBadIndexes;
      [*] = ;var bPlayerSet : Boolean;
      [*] = ;for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;if (gMap.players[i].bexists) then
      [*] = ;begin
      [*] = ;   if IsLanGame then
      [*] = ;   begin
      [*] = ;      if (gMap.players[i].lanid=LanMyInfoID) then
      [*] = ;      begin
      [*] = ;         _control_SetPlayer(i);
      [*] = ;         bPlayerSet := True;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      if (gMap.players[i].bexists) and (gMap.players[i].bhuman) then
      [*] = ;      begin
      [*] = ;         _control_SetPlayer(i);
      [*] = ;         bPlayerSet := True;
      [*] = ;         break;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (not bPlayerSet) then
      [*] = ;begin
      [*] = ;   _control_SetPlayer(0);
      [*] = ;   ErrorLog('InitMapHistoricalBattle, _control_SetPlayer was not set, so we force set player to plind=0');
      [*] = ;end;
      [*] = ;
      [*] = ;gInt_UpdateAreasCount:=0;
      [*] = ;
      [*] = ;gScenario.bactive := True;
      [*] = ;gbool_editor_peacemode := False;
      [*] = ;gInterface.gamemode := gc_gamemode_game;
      [*] = ;
      [*] = ;if (gScenario.bactive) then
      [*] = ;begin
      [*] = ;   var i : Integer;
      [*] = ;   _misc_CleanRecordManagerExt;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   gPlayer[i].difficulty := gc_player_difficulty_impossible;
      [*] = ;   gScenario.difficulty := gc_player_difficulty_impossible;
      [*] = ;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   begin
      [*] = ;      gPlayer[i].bexists := True;
      [*] = ;      gPlayer[i].victorystate := gc_player_victorystate_none;
      [*] = ;      gPlayer[i].victorystategametime := 0;
      [*] = ;   end;
      [*] = ;   //gPlayer[i].bexists := (gPlayer[i].counter.farmused>0);
      [*] = ;
      [*] = ;   //if (gScenario.settings.filename<>'') then
      [*] = ;   //gMap.name := ExtractFileName(gScenario.settings.filename);
      [*] = ;   for i:=0 to gc_scenario_maxqueries-1 do
      [*] = ;   gScenario.queries[i].langtable := gScenario.settings.langtable;
      [*] = ;   for i:=0 to gc_scenario_maxresults-1 do
      [*] = ;   gScenario.results[i].langtable := gScenario.settings.langtable;
      [*] = ;   for i:=0 to gc_scenario_maxobjectives-1 do
      [*] = ;   gScenario.objectives[i].langtable := gScenario.settings.langtable;
      [*] = ;   _scenario_UpdateQueries(gScenario);
      [*] = ;   _scenario_UpdateAIRegions(gScenario);
      [*] = ;   _scenario_UpdateAIRegionsActiveStateToTriggers(gScenario);
      [*] = ;   if (gbool_editor_peacemode) then
      [*] = ;   GUIExecuteState('WritePeacemode');
      [*] = ;   //EmulateGUIShortcut('Ctrl+W');
      [*] = ;   _res_ResGridClear;
      [*] = ;   _res_ResGridResetEnvironment;
      [*] = ;   var j : Integer;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   for j:=0 to gc_ResCount-1 do
      [*] = ;   _res_SetResToPlayerByIndex(i, j, gScenario.players[i].resources[j]);
      [*] = ;   gScenario.triggers[0].bactive := true;
      [*] = ;
      [*] = ;   var smHnd : Integer = GetTriggerManagerStateMachineHandle('trigger0');
      [*] = ;   if (smHnd<>0) then
      [*] = ;   StateMachineExecuteState(smHnd, 'Initial', 0);
      [*] = ;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   gPlayer[i].team := gMap.players[i].team;
      [*] = ;end;
      [*] = ;
      [*] = ;if (not IsLanGame) then
      [*] = ;gMap.gamestage := gc_map_gamestage_started;
   struct.end
section.end

