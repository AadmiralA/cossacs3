section.begin
   Name = WriteSync
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;Log('WriteSync');
      [*] = ;
      [*] = ;var uid: Integer = arg_plr.fid;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadSync') then
      [*] = ;   _net_ErrorLog('RecordCustomBegin(''ReadSync'')');
      [*] = ;   // write data to package
      [*] = ;
      [*] = ;   RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;
      [*] = ;   var nextuid : Integer = GetMapNextUniqId;
      [*] = ;   var count : Integer = nextuid-uid;
      [*] = ;   RecordCustomWriteInteger(count);
      [*] = ;
      [*] = ;   var i : Integer;
      [*] = ;   for i:=0 to count-1 do
      [*] = ;   begin
      [*] = ;      var syncuid : Integer = uid-(count-1)+i;
      [*] = ;      var hnd : Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;      var pobj : Pointer = nil;
      [*] = ;      if (hnd<>0) then
      [*] = ;      pobj := _unit_GetTObj(hnd);
      [*] = ;      var bexists : Boolean = (hnd<>0) and (pobj<>nil);
      [*] = ;      RecordCustomWriteInteger(syncuid);
      [*] = ;      RecordCustomWriteBoolean(bexists);
      [*] = ;      if (bexists) then
      [*] = ;      begin
      [*] = ;         var racename : String = GetGameObjectRaceNameByHandle(hnd);
      [*] = ;         var basename : String = GetGameObjectBaseNameByHandle(hnd);
      [*] = ;         var posx : Float = GetGameObjectPositionXByHandle(hnd);
      [*] = ;         var posz : Float = GetGameObjectPositionZByHandle(hnd);
      [*] = ;         var scale : Float = GetGameObjectScaleXByHandle(hnd);
      [*] = ;         var upx, upy, upz, dirx, diry, dirz : Float;
      [*] = ;         GetGameObjectOrientationByHandle(hnd, upx, upy, upz, dirx, diry, dirz);
      [*] = ;         //var angle : Float = _misc_GetDirAngleToXVector(upx, upz);
      [*] = ;         var statestag : Integer = GetGameObjectStatesTagByHandle(hnd);
      [*] = ;         var stohnd : Integer = GetGameObjectSTOHandleByHandle(hnd);
      [*] = ;         var sto : Integer = GetGameObjectUniqueIdByHandle(GetGameObjectSTOHandleByHandle(hnd));
      [*] = ;         var stpx : Float = GetGameObjectStateTargetPositionXByHandle(hnd);
      [*] = ;         var stpz : Float = GetGameObjectStateTargetPositionXByHandle(hnd);
      [*] = ;         var sta : Float = GetGameObjectSTArrowAngleByHandle(hnd);
      [*] = ;
      [*] = ;         var cid : Integer = TObj(pobj).cid;
      [*] = ;         var id : Integer = TObj(pobj).id;
      [*] = ;         var pl : Integer = TObj(pobj).pl;
      [*] = ;         var hp : Integer = TObj(pobj).hp;
      [*] = ;         var bbuilt : Boolean = TObj(pobj).bbuilt;
      [*] = ;         var bdead : Boolean = TObj(pobj).bdead;
      [*] = ;         var buildprogress : Float = TObj(pobj).buildprogress;
      [*] = ;         var uniqrnd : Float = TObj(pobj).uniqrnd;
      [*] = ;
      [*] = ;         RecordCustomWriteString(racename);
      [*] = ;         RecordCustomWriteString(basename);
      [*] = ;         RecordCustomWriteFloat(posx);
      [*] = ;         RecordCustomWriteFloat(posz);
      [*] = ;         RecordCustomWriteFloat(scale);
      [*] = ;         //RecordCustomWriteFloat(angle);
      [*] = ;         RecordCustomWriteFloat(upx);
      [*] = ;         RecordCustomWriteFloat(upy);
      [*] = ;         RecordCustomWriteFloat(upz);
      [*] = ;         RecordCustomWriteFloat(dirx);
      [*] = ;         RecordCustomWriteFloat(diry);
      [*] = ;         RecordCustomWriteFloat(dirz);
      [*] = ;         RecordCustomWriteInteger(statestag);
      [*] = ;         RecordCustomWriteInteger(sto);
      [*] = ;         RecordCustomWriteFloat(stpx);
      [*] = ;         RecordCustomWriteFloat(stpz);
      [*] = ;         RecordCustomWriteFloat(sta);
      [*] = ;         // TObj
      [*] = ;         RecordCustomWriteInteger(cid);
      [*] = ;         RecordCustomWriteInteger(id);
      [*] = ;         RecordCustomWriteInteger(pl);
      [*] = ;         RecordCustomWriteInteger(hp);
      [*] = ;         RecordCustomWriteBoolean(bbuilt);
      [*] = ;         RecordCustomWriteBoolean(bdead);
      [*] = ;         RecordCustomWriteFloat(buildprogress);
      [*] = ;         RecordCustomWriteFloat(uniqrnd);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end
      [*] = ;else
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadSync') then
      [*] = ;   _net_ErrorLog('RecordCustomBegin(''ReadSync'')');
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteInteger(uid);
      [*] = ;
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;   _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

