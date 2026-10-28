section.begin
   Name = ReadWall
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
      [*] = ;var usage : Integer = RecordCustomReadByte;
      [*] = ;var cid : Integer = RecordCustomReadByte;
      [*] = ;var id : Integer = RecordCustomReadByte;
      [*] = ;var count : Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;gWallClusterCreate.Clear(False);
      [*] = ;gWallClusterCreate.plIndex := plind;
      [*] = ;gWallClusterCreate.wallType := usage;
      [*] = ;gWallClusterCreate.cid := cid;
      [*] = ;
      [*] = ;if _net_IsServer then
      [*] = ;begin
      [*] = ;   var i : Integer;
      [*] = ;   for i:=count-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var sprite : Integer = RecordCustomReadByte;
      [*] = ;      var px : Float = RecordCustomReadFloat;
      [*] = ;      var pz : Float = RecordCustomReadFloat;
      [*] = ;      var gouid : Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;      var pWallCell : Pointer = gWallClusterCreate.AddCell;
      [*] = ;      TWallCell(pWallCell).fsprite := sprite;
      [*] = ;      TWallCell(pWallCell).x := floor(px);
      [*] = ;      TWallCell(pWallCell).y := floor(pz);
      [*] = ;   end;
      [*] = ;   var num : Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;   var peasantcount: Integer = RecordCustomReadInteger;
      [*] = ;   _net_intlist.Clear();
      [*] = ;   for i:=0 to peasantcount-1 do begin
      [*] = ;      var hnduid: Integer = RecordCustomReadInteger;
      [*] = ;      var handle: Integer = GetGameObjectHandleByUniqueId(hnduid);
      [*] = ;      if handle<>0 then
      [*] = ;      _net_intlist.Add(handle)
      [*] = ;      else _net_ErrorLog('readwall invalid uid('+IntToStr(hnduid)+')');
      [*] = ;   end;
      [*] = ;   arg_plr.fintlst := _net_intlist;
      [*] = ;   arg_plr.fcid := cid;
      [*] = ;   arg_plr.fid := id;
      [*] = ;   ExecuteState('WriteWall');
      [*] = ;
      [*] = ;   //gWallClusterCreate.Clear(False);
      [*] = ;   gWallClusterCreate.SetWallBuildMode(-1, -1, -1);
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;begin // _net_IsClient or _net_IsReplay
      [*] = ;   var pCluster : Pointer = gWallSystem.Add;
      [*] = ;   TWallCluster(pCluster).plIndex := plind;
      [*] = ;   TWallCluster(pCluster).wallType := usage;
      [*] = ;   TWallCluster(pCluster).cid := cid;
      [*] = ;   var i : Integer;
      [*] = ;   for i:=count-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var sprite : Integer = RecordCustomReadByte;
      [*] = ;      var px : Float = RecordCustomReadFloat;
      [*] = ;      var pz : Float = RecordCustomReadFloat;
      [*] = ;      var gouid : Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;      var pWallCell : Pointer = TWallCluster(pCluster).AddCell;
      [*] = ;      TWallCell(pWallCell).SetSprite(sprite, True);
      [*] = ;      TWallCell(pWallCell).wallType := usage;
      [*] = ;      TWallCell(pWallCell).x := floor(px);
      [*] = ;      TWallCell(pWallCell).y := floor(pz);
      [*] = ;      TWallCell(pWallCell).cid := cid;
      [*] = ;      TWallCell(pWallCell).plInd := plInd;
      [*] = ;   end;
      [*] = ;
      [*] = ;   for i:=count-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var pWallCell : Pointer = TWallCluster(pCluster).GetCell(i);
      [*] = ;      var sid : String;
      [*] = ;      _misc_GetSIDBySprite(gObjProp[cid, id].usage, TWallCell(pWallCell).GetSprite, cid, sid);
      [*] = ;      var px : Float = TWallCell(pWallCell).x+0.5;
      [*] = ;      var pz : Float = TWallCell(pWallCell).y+0.5;
      [*] = ;      gint_unit_parentcid := cid;
      [*] = ;      var trgHnd : Integer = CreatePlayerGameObjectHandleByHandle(plhnd, gc_racename_buildings, sid, px, 0, pz);
      [*] = ;      if trghnd<>0 then begin
      [*] = ;         TWallCell(pWallCell).goHnd := trghnd;
      [*] = ;         var pobj: Pointer = _unit_GetTObj(trghnd);
      [*] = ;         if pobj<>nil then begin
      [*] = ;            TObj(pobj).bbuilt := False;
      [*] = ;            TObj(pobj).buildprogress := 0;
      [*] = ;            TObj(pobj).hp := 10;
      [*] = ;            _unit_ControlBuildProgress(trghnd);
      [*] = ;         end else _misc_ErrorLog('_player_ConstructBuildingList: pobj = nil');
      [*] = ;         // remove objects placed on object's collision mask
      [*] = ;         if (GetCountOfPlayers>=gc_playerind_env) and (gObjProp[cid][id].usage<>gc_obj_usage_mine) then
      [*] = ;         begin
      [*] = ;            var mapW : Integer = GetMapWidth;
      [*] = ;            var mapH : Integer = GetMapHeight;
      [*] = ;            var w : Integer = GetGameObjectCollisionMaskWidthByHandle(trghnd);
      [*] = ;            var h : Integer = GetGameObjectCollisionMaskHeightByHandle(trghnd);
      [*] = ;            var i, j, k : Integer;
      [*] = ;            var envPlHnd : Integer = GetPlayerHandleByIndex(gc_playerind_env);
      [*] = ;
      [*] = ;            for i := 0 to h-1 do
      [*] = ;            for j := 0 to w-1 do
      [*] = ;            if GetGameObjectCollisionMaskValueByHandle(trghnd, i, j) then
      [*] = ;            begin
      [*] = ;               var testX : Float;
      [*] = ;               if w mod 2 = 1 then
      [*] = ;               testX := px + (j-(w-1)/2)/2
      [*] = ;               else
      [*] = ;               testX := px + (j+1-w/2)/2;
      [*] = ;
      [*] = ;               var testZ : Float;
      [*] = ;               if h mod 2 = 1 then
      [*] = ;               testZ := pz + (i-(h-1)/2)/2
      [*] = ;               else
      [*] = ;               testZ := pz + (i-h/2)/2;
      [*] = ;
      [*] = ;               if (testX >= -mapW/2) and (testX <= mapW/2) and (testZ >= -mapH/2) and (testZ <= mapH/2) then
      [*] = ;               begin
      [*] = ;                  testX := Floor(2*testX)/2 + 0.25;
      [*] = ;                  testZ := Floor(2*testZ)/2 + 0.25;
      [*] = ;                  GetGameObjectsInRadius(testX, testZ, 1, false, true, envPlHnd, -1, 0, false, false, false, false, false, false);
      [*] = ;                  // _misc_MakeListEnvironmentInRadius not ok, cause we need to clear decor/bushes that dont have TRes struct and not in grid list
      [*] = ;
      [*] = ;                  for k:=GetGameObjectListCount-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     var goHnd : Integer = GetGameObjectListByIndex(k);
      [*] = ;                     var baseid : Integer = _misc_GetBaseObjID(goHnd);
      [*] = ;                     if (baseid=gc_baseid_nil) or (baseid=gc_baseid_res) then
      [*] = ;                     begin
      [*] = ;                        if (baseid=gc_baseid_res) then
      [*] = ;                        begin
      [*] = ;                           var pres : Pointer = _misc_GetObjectArgData(goHnd, gc_argenv_res);
      [*] = ;                           if (not ((TRes(pres).itype=gc_resource_type_wood) or (TRes(pres).itype=gc_resource_type_none))) then
      [*] = ;                           continue;
      [*] = ;                        end;
      [*] = ;                        var posX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;                        var posZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;
      [*] = ;                        if (Abs(posX-testX) < 0.25*3) and (Abs(posZ-testZ) < 0.25*3) then
      [*] = ;                        GameObjectDestroyByHandle(goHnd);
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   var num : Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;   var peasantcount : Integer = RecordCustomReadInteger;
      [*] = ;   for i:=0 to peasantcount-1 do
      [*] = ;   RecordCustomReadInteger; // prevent from accert that rec.ord fo.rmat is invalid, cause I read less than rec.ord size
      [*] = ;
      [*] = ;   if (count>0) then
      [*] = ;   _misc_UpdateWallTopology(pCluster);
      [*] = ;
      [*] = ;   if _net_debug_log_new then
      [*] = ;   _net_Log('readwall(num='+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+';count=['+IntToStr(count)+'])');
      [*] = ;   //if num<>GetPlayerGameObjectsCountByHandle(plhnd) then
      [*] = ;   //  _net_ErrorLog('number of units is different: coming('+IntToStr(num)+') != current('+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+')');
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   var i : Integer;
      [*] = ;   for i:=count-1 downto 0 do
      [*] = ;   begin
      [*] = ;      RecordCustomReadByte;
      [*] = ;      RecordCustomReadFloat;
      [*] = ;      RecordCustomReadFloat;
      [*] = ;      RecordCustomReadInteger;
      [*] = ;   end;
      [*] = ;   RecordCustomReadInteger;
      [*] = ;   var peasantcount : Integer = RecordCustomReadInteger;
      [*] = ;   for i:=0 to peasantcount-1 do
      [*] = ;   RecordCustomReadInteger; // prevent from accert that rec.ord fo.rmat is invalid, cause I read less than rec.ord size
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

