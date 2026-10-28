section.begin
   Name = WriteWall
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var list: Pointer = arg_plr.fintlst;
      [*] = ;var cid: Integer = arg_plr.fcid;
      [*] = ;var id : Integer = arg_plr.fid;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;var usage : Integer = gObjProp[cid][id].usage;
      [*] = ;
      [*] = ;var pCluster : Pointer;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then begin
      [*] = ;   var wallList : TIntegerList;
      [*] = ;   pCluster := gWallSystem.Add;
      [*] = ;   TWallCluster(pCluster).plIndex := plind;
      [*] = ;   TWallCluster(pCluster).wallType := usage;
      [*] = ;   TWallCluster(pCluster).cid := cid;
      [*] = ;   var p : Pointer = gCanPlaceBuildingWalls;
      [*] = ;   if (p<>nil) then
      [*] = ;   FillMem(p, SizeOf(gCanPlaceBuildingWalls), 0);
      [*] = ;   _misc_PrepareWallClusterForLan(gWallClusterCreate);
      [*] = ;   var count : Integer = gWallClusterCreate.Cells.GetCount;
      [*] = ;   var i : Integer;
      [*] = ;   for i:=count-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var p : Pointer = gWallClusterCreate.Cells.GetPointer(i);
      [*] = ;      if (p<>nil) then
      [*] = ;      begin
      [*] = ;         var goHnd : Integer = TWallCell(p).goHnd;
      [*] = ;         if (goHnd<>0) then
      [*] = ;         begin
      [*] = ;            var px : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;            var pz : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;            if (_misc_CanPlaceBuilding(goHnd, plind, usage, px, pz, gc_path_mincoldist, 0.26, 0.5, gc_path_minemincoldist, True)) then
      [*] = ;            begin
      [*] = ;               //var trgHnd : Integer = _player_ConstructBuildingList(myPlHnd, cid, sid, px, pz, gSelectedObjects, (not GetGUIShiftStateShift), True);
      [*] = ;               //settuping cells:
      [*] = ;               var sprite  : Integer = TWallCell(p).GetSprite;
      [*] = ;               var pWallCell : Pointer = TWallCluster(pCluster).AddCell;
      [*] = ;               TWallCell(pWallCell).SetSprite(sprite, True);
      [*] = ;               TWallCell(pWallCell).wallType := TWallCell(p).wallType;
      [*] = ;               TWallCell(pWallCell).x := TWallCell(p).x;
      [*] = ;               TWallCell(pWallCell).y := TWallCell(p).y;
      [*] = ;               TWallCell(pWallCell).cid := TWallCell(p).cid;
      [*] = ;               TWallCell(pWallCell).plInd := TWallCell(p).plInd;
      [*] = ;               //TWallCell(pWallCell).goHnd := 0;
      [*] = ;
      [*] = ;               var sid : String;
      [*] = ;               _misc_GetSIDBySprite(gObjProp[cid, id].usage, sprite, cid, sid);
      [*] = ;               var px : Float = TWallCell(pWallCell).x+0.5;
      [*] = ;               var pz : Float = TWallCell(pWallCell).y+0.5;
      [*] = ;               gint_unit_parentcid := cid;
      [*] = ;               var trgHnd : Integer = CreatePlayerGameObjectHandleByHandle(plhnd, gc_racename_buildings, sid, px, 0, pz);
      [*] = ;               if trghnd<>0 then begin
      [*] = ;                  TWallCell(pWallCell).goHnd := trghnd;
      [*] = ;                  wallList.Add(trghnd);
      [*] = ;                  var pobj: Pointer = _unit_GetTObj(trghnd);
      [*] = ;                  if pobj<>nil then begin
      [*] = ;                     TObj(pobj).bbuilt := False;
      [*] = ;                     TObj(pobj).buildprogress := 0;
      [*] = ;                     TObj(pobj).hp := 10;
      [*] = ;                     _unit_ControlBuildProgress(trghnd);
      [*] = ;                  end else _misc_ErrorLog('_player_ConstructBuildingList: pobj = nil');
      [*] = ;                  // remove objects placed on object's collision mask
      [*] = ;                  if (GetCountOfPlayers>=gc_playerind_env) and (gObjProp[cid][id].usage<>gc_obj_usage_mine) then
      [*] = ;                  begin
      [*] = ;                     var mapW : Integer = GetMapWidth;
      [*] = ;                     var mapH : Integer = GetMapHeight;
      [*] = ;                     var w : Integer = GetGameObjectCollisionMaskWidthByHandle(trghnd);
      [*] = ;                     var h : Integer = GetGameObjectCollisionMaskHeightByHandle(trghnd);
      [*] = ;                     var i, j, k : Integer;
      [*] = ;                     var envPlHnd : Integer = GetPlayerHandleByIndex(gc_playerind_env);
      [*] = ;
      [*] = ;                     for i := 0 to h-1 do
      [*] = ;                     for j := 0 to w-1 do
      [*] = ;                     if GetGameObjectCollisionMaskValueByHandle(trghnd, i, j) then
      [*] = ;                     begin
      [*] = ;                        var testX : Float;
      [*] = ;                        if w mod 2 = 1 then
      [*] = ;                        testX := px + (j-(w-1)/2)/2
      [*] = ;                        else
      [*] = ;                        testX := px + (j+1-w/2)/2;
      [*] = ;
      [*] = ;                        var testZ : Float;
      [*] = ;                        if h mod 2 = 1 then
      [*] = ;                        testZ := pz + (i-(h-1)/2)/2
      [*] = ;                        else
      [*] = ;                        testZ := pz + (i-h/2)/2;
      [*] = ;
      [*] = ;                        if (testX >= -mapW/2) and (testX <= mapW/2) and (testZ >= -mapH/2) and (testZ <= mapH/2) then
      [*] = ;                        begin
      [*] = ;                           testX := Floor(2*testX)/2 + 0.25;
      [*] = ;                           testZ := Floor(2*testZ)/2 + 0.25;
      [*] = ;                           GetGameObjectsInRadius(testX, testZ, 1, false, true, envPlHnd, -1, 0, false, false, false, false, false, false);
      [*] = ;                           // _misc_MakeListEnvironmentInRadius not ok, cause we need to clear decor/bushes that dont have TRes struct and not in grid list
      [*] = ;
      [*] = ;                           for k:=GetGameObjectListCount-1 downto 0 do
      [*] = ;                           begin
      [*] = ;                              var hnd : Integer = GetGameObjectListByIndex(k);
      [*] = ;                              var baseid : Integer = _misc_GetBaseObjID(hnd);
      [*] = ;                              if (baseid=gc_baseid_nil) or (baseid=gc_baseid_res) then
      [*] = ;                              begin
      [*] = ;                                 if (baseid=gc_baseid_res) then
      [*] = ;                                 begin
      [*] = ;                                    var pres : Pointer = _misc_GetObjectArgData(hnd, gc_argenv_res);
      [*] = ;                                    if (not ((TRes(pres).itype=gc_resource_type_wood) or (TRes(pres).itype=gc_resource_type_none))) then
      [*] = ;                                    continue;
      [*] = ;                                 end;
      [*] = ;                                 var posX : Float = GetGameObjectPositionXByHandle(hnd);
      [*] = ;                                 var posZ : Float = GetGameObjectPositionZByHandle(hnd);
      [*] = ;
      [*] = ;                                 if (Abs(posX-testX) < 0.25*3) and (Abs(posZ-testZ) < 0.25*3) then
      [*] = ;                                 GameObjectDestroyByHandle(hnd);
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   _misc_UpdateWallTopology(pCluster);
      [*] = ;
      [*] = ;   var tmplist : TIntegerList;
      [*] = ;   for i:=0 to TIntegerList(list).GetCount-1 do
      [*] = ;   tmplist.Add(TIntegerList(list).Get(i));
      [*] = ;   for i:=0 to wallList.GetCount-1 do
      [*] = ;   _player_OrderUnitsToBuild(tmplist, wallList.Get(i), true, false, False);
      [*] = ;end
      [*] = ;else
      [*] = ;pCluster := gWallClusterCreate;
      [*] = ;
      [*] = ;var count : Integer = TWallCluster(pCluster).Cells.GetCount;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadWall') then
      [*] = ;   _net_ErrorLog('RecordCustomBegin(''ReadWall'')');
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;   RecordCustomWriteByte(usage);
      [*] = ;   RecordCustomWriteByte(cid);
      [*] = ;   RecordCustomWriteByte(id);
      [*] = ;   RecordCustomWriteInteger(count);
      [*] = ;   var i : Integer;
      [*] = ;   for i:=count-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var p : Pointer = TWallCluster(pCluster).Cells.GetPointer(i);
      [*] = ;      if (p<>nil) then
      [*] = ;      begin
      [*] = ;         //var goHnd : Integer = TWallCell(p).goHnd;
      [*] = ;         //if (goHnd<>0) then
      [*] = ;         //begin
      [*] = ;            var sprite : Integer = TWallCell(p).GetSprite;
      [*] = ;            var pWallCell : Pointer = TWallCluster(pCluster).GetCell(i);
      [*] = ;            var px : Float = TWallCell(pWallCell).x;
      [*] = ;            var pz : Float = TWallCell(pWallCell).y;
      [*] = ;            var gouid: Integer;
      [*] = ;            if _net_IsServer or _net_IsRecord then //gohnd=0
      [*] = ;            gouid:=GetGameObjectUniqueIdByHandle(TWallCell(pWallCell).goHnd);
      [*] = ;
      [*] = ;            RecordCustomWriteByte(sprite);
      [*] = ;            RecordCustomWriteFloat(px);
      [*] = ;            RecordCustomWriteFloat(pz);
      [*] = ;            RecordCustomWriteInteger(gouid);
      [*] = ;         //end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   RecordCustomWriteInteger(GetPlayerGameObjectsCountByHandle(plhnd));
      [*] = ;
      [*] = ;   var peasantcount: Integer;
      [*] = ;   if list<>nil then
      [*] = ;   peasantcount:=TIntegerList(list).GetCount
      [*] = ;   else peasantcount:=0;
      [*] = ;   RecordCustomWriteInteger(peasantcount);
      [*] = ;   for i:=0 to peasantcount-1 do begin
      [*] = ;      var handle: Integer = TIntegerList(list).Get(i);
      [*] = ;      var hnduid: Integer = GetGameObjectUniqueIdByHandle(handle);
      [*] = ;      RecordCustomWriteInteger(hnduid);
      [*] = ;   end;
      [*] = ;
      [*] = ;   if _net_debug_log_new then
      [*] = ;   _net_Log('writewall(num='+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+';count=['+IntToStr(count)+'])');
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;   _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;arg_plr.fresult:=count;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

