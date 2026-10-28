section.begin
   Name = ReadSync
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;Log('ReadSync');
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;var bFromServer : Boolean = RecordCustomReadBoolean;
      [*] = ;if _net_IsServer then
      [*] = ;begin
      [*] = ;   // read data from package
      [*] = ;   var uid : Integer = RecordCustomReadInteger;
      [*] = ;   arg_plr.fid := uid;
      [*] = ;   ExecuteState('WriteSync');
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;begin // _net_IsClient or _net_IsReplay
      [*] = ;   var count : Integer = RecordCustomReadInteger;
      [*] = ;   Log('_net_ReadSync : start process : count = '+IntToStr(count));
      [*] = ;   var i : Integer;
      [*] = ;   for i:=0 to count-1 do
      [*] = ;   begin
      [*] = ;      var syncuid : Integer = RecordCustomReadInteger;
      [*] = ;      var bexists : Boolean = RecordCustomReadBoolean;
      [*] = ;      if (bexists) then
      [*] = ;      begin
      [*] = ;         var clientHnd : Integer = GetGameObjectHandleByUniqueId(syncuid);
      [*] = ;         if (clientHnd=0) then
      [*] = ;         begin
      [*] = ;            var racename : String = RecordCustomReadString;
      [*] = ;            var basename : String = RecordCustomReadString;
      [*] = ;            var posx : Float = RecordCustomReadFloat;
      [*] = ;            var posz : Float = RecordCustomReadFloat;
      [*] = ;            var scale : Float = RecordCustomReadFloat;
      [*] = ;            //var angle : Float = RecordCustomReadFloat;
      [*] = ;            var upx : Float = RecordCustomReadFloat;
      [*] = ;            var upy : Float = RecordCustomReadFloat;
      [*] = ;            var upz : Float = RecordCustomReadFloat;
      [*] = ;            var dirx : Float = RecordCustomReadFloat;
      [*] = ;            var diry : Float = RecordCustomReadFloat;
      [*] = ;            var dirz : Float = RecordCustomReadFloat;
      [*] = ;            var statestag : Integer = RecordCustomReadInteger;
      [*] = ;            var sto : Integer = RecordCustomReadInteger;
      [*] = ;            var stpx : Float = RecordCustomReadFloat;
      [*] = ;            var stpz : Float = RecordCustomReadFloat;
      [*] = ;            var sta : Float = RecordCustomReadFloat;
      [*] = ;            // TObj
      [*] = ;            var cid : Integer = RecordCustomReadInteger;
      [*] = ;            var id : Integer = RecordCustomReadInteger;
      [*] = ;            var pl : Integer = RecordCustomReadInteger;
      [*] = ;            var hp : Integer = RecordCustomReadInteger;
      [*] = ;            var bbuilt : Boolean = RecordCustomReadBoolean;
      [*] = ;            var bdead : Boolean = RecordCustomReadBoolean;
      [*] = ;            var buildprogress : Float = RecordCustomReadFloat;
      [*] = ;            var uniqrnd : Float = RecordCustomReadFloat;
      [*] = ;
      [*] = ;            var plHnd : Integer = GetPlayerHandleByIndex(pl);
      [*] = ;            gint_unit_parentcid := cid;
      [*] = ;            var hnd : Integer = CreatePlayerGameObjectHandleByHandle(plHnd, racename, basename, posx, 0, posz);
      [*] = ;            var newuid : Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;            if (newuid<>syncuid) then
      [*] = ;            ErrorLog('ReadSync, created gameobject have other UID then needed. My = '+IntToStr(newuid)+' syncuid = '+IntToStr(newuid));
      [*] = ;            var pobj: Pointer = _unit_GetTObj(hnd);
      [*] = ;            if (pobj<>nil) then
      [*] = ;            begin
      [*] = ;               // check valid start
      [*] = ;               if (cid<>TObj(pobj).cid) or (id<>TObj(pobj).id) or (pl<>TObj(pobj).pl) then
      [*] = ;               ErrorLog('ReadSync : invalid unit');
      [*] = ;               // check valid finish
      [*] = ;               TObj(pobj).hp := hp;
      [*] = ;               TObj(pobj).bbuilt := bbuilt;
      [*] = ;               TObj(pobj).bdead := bdead;
      [*] = ;               TObj(pobj).buildprogress := buildprogress;
      [*] = ;               TObj(pobj).uniqrnd := uniqrnd;
      [*] = ;               if (gObjProp[cid][id].bbuilding) and (buildprogress<1) then
      [*] = ;               _unit_ControlBuildProgress(hnd);
      [*] = ;            end
      [*] = ;            else
      [*] = ;            _net_ErrorLog('invalid pointer: pobj=nil');
      [*] = ;            SetGameObjectOrientationByHandle(hnd, upx, upy, upz, dirx, diry, dirz);
      [*] = ;            SetGameObjectScaleByHandle(hnd, scale, scale, scale);
      [*] = ;            GameObjectDoRayCastTerrainByHandle(hnd);
      [*] = ;            SetGameObjectStatesTagByHandle(hnd, statestag);
      [*] = ;            SetGameObjectStatesTagForceUpdateByHandle(hnd);
      [*] = ;            SetGameObjectSTOHandleByHandle(hnd, sto);
      [*] = ;            SetGameObjectStateTargetPositionByHandle(hnd, stpx, 0, stpz);
      [*] = ;            SetGameObjectSTArrowAngleByHandle(hnd, sta);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         ErrorLog('ReadSync, try to recrate already existed unit');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   Log('_net_ReadSync : finish process');
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

