section.begin
   Name = InitMap
   Code : struct.begin
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
      [*] = ;   gLanSyncData.Init;
      [*] = ;   gLanSyncDataLast.Init;
      [*] = ;end;
      [*] = ;
      [*] = ;TimeLogReset();
      [*] = ;
      [*] = ;var envplhnd: Integer = GetPlayerHandleByName(gc_racename_env);
      [*] = ;if envplhnd<>0 then
      [*] = ;ClearPlayerGameObjectsByHandle(envplhnd);
      [*] = ;
      [*] = ;_map_ResetMapSettings(gMap, true, false, false, false, false, false);
      [*] = ;gMap.settings.gen.relieftype := 3;
      [*] = ;gMap.settings.gen.resourcestart := 2;
      [*] = ;gMap.settings.gen.resourcemines := 1;
      [*] = ;var i : Integer;
      [*] = ;for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;gMap.players[i].color := i;
      [*] = ;gMap.players[gc_player_mercenaryind].color := gc_MaxColorCount-1;
      [*] = ;
      [*] = ;_misc_ResetProgressPlayer;
      [*] = ;
      [*] = ;CreatePlayers;
      [*] = ;
      [*] = ;_misc_ResetGameData;
      [*] = ;
      [*] = ;for i:=0 to gc_MaxPlayerCount-1 do begin
      [*] = ;   var ccsid: String = 'pol';
      [*] = ;   var ccid: Integer = _country_GetIDBySID(ccsid);
      [*] = ;   if gMap.players[i].bexists then
      [*] = ;   gPlayer[i].cid:=ccid;
      [*] = ;   if i<>gc_MaxPlayerCount-1 then
      [*] = ;   gPlayer[i].enemyplmask:=1 shl (gc_MaxPlayerCount-1);
      [*] = ;end;
      [*] = ;
      [*] = ;for i:=0 to gc_MaxPlayerCount-1 do begin
      [*] = ;   var j: Integer;
      [*] = ;   var myteam: Integer = gMap.players[i].team;
      [*] = ;   for j:=0 to gc_MaxPlayerCount-1 do begin
      [*] = ;      if i<>j then begin
      [*] = ;         var histeam: Integer = gMap.players[j].team;
      [*] = ;         if(myteam<>histeam) or (histeam=0) or (myteam=0) then begin
      [*] = ;            gPlayer[i].enemyplmask:=gPlayer[i].enemyplmask or (1 shl j);
      [*] = ;            AddPlayerEnemyPlayerByHandle(GetPlayerHandleByIndex(i), GetPlayerHandleByIndex(j));
      [*] = ;         end else AddPlayerFriendPlayerByHandle(GetPlayerHandleByIndex(i), GetPlayerHandleByIndex(j));
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;_player_SetupTeams(false);
      [*] = ;
      [*] = ;var plind: Integer;
      [*] = ;if _net_IsOnline and _net_IsClient then plind:=1;
      [*] = ;_control_SetPlayer(plind);
      [*] = ;
      [*] = ;var xoff: Float = 35;
      [*] = ;var yoff: Float = -14;
      [*] = ;
      [*] = ;//LoadPatterns(true, true); // make map gen much faster 800ms instead of 4100ms
      [*] = ;
      [*] = ;{if _net_IsOnline then begin
      [*] = ;   CreateStartPointDebug(0, 0, 0);
      [*] = ;   CreateStartPointDebug(1, Round(xoff), 0);
      [*] = ;   end else begin
      [*] = ;   _map_CreateStartPoint(0, -32-3, -14);
      [*] = ;   _map_CreateStartPoint(1, 32+3, -14);
      [*] = ;end;}
      [*] = ;
      [*] = ;MapWorldUpdateTerrainData(0, 0);
      [*] = ;TopologyBuild;
      [*] = ;_misc_BuildIslandsGrid;
      [*] = ;gInt_MinTopZoneChanged:=TopologyGetZonesCount;
      [*] = ;gInt_MaxTopZoneChanged:=-1;
      [*] = ;gInt_UpdateAreasCount:=0;
      [*] = ;CleanRecordManager;
      [*] = ;TimeLog('...InitMap finished...');
   struct.end
section.end

