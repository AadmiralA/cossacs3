section.begin
   Name = WriteNew
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var race: String = arg_plr.fracename;
      [*] = ;var base: String = arg_plr.fbasename;
      [*] = ;var posx: Float = arg_plr.fposx;
      [*] = ;var posz: Float = arg_plr.fposz;
      [*] = ;var cid : Integer = arg_plr.fcid; // if cid<=0 then this is countryid, else this is uid for building that produce unit, and by building uid we know it's cid
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var gohnd: Integer;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then begin
      [*] = ;   if (cid<=0) then
      [*] = ;   gint_unit_parentcid:=-cid // required in gameobject initial state to setup country id
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      var buildhnd : Integer = GetGameObjectHandleByUniqueId(cid);
      [*] = ;      if (buildhnd<>0) then
      [*] = ;      begin
      [*] = ;         var pobj: Pointer = _unit_GetTObj(buildhnd);
      [*] = ;         if (pobj<>nil) then
      [*] = ;         gint_unit_parentcid := TObj(pobj).cid;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   gohnd:=CreatePlayerGameObjectHandleByHandle(plhnd, race, base, posx, 0, posz);
      [*] = ;   var pobj: Pointer = _unit_GetTObj(gohnd);
      [*] = ;   if pobj<>nil then begin
      [*] = ;      if gObjProp[TObj(pobj).cid][TObj(pobj).id].bbuilding then begin
      [*] = ;         //function _player_ConstructBuilding(plHnd, cid: Integer; sid: String; px, pz: Float): Integer;
      [*] = ;         TObj(pobj).bbuilt:=false;
      [*] = ;         TObj(pobj).buildprogress:=0;
      [*] = ;         TObj(pobj).hp:=gPlayer[TObj(pobj).pl].objbase[TObj(pobj).cid][TObj(pobj).id].maxhp;
      [*] = ;         _unit_ControlBuildProgress(gohnd);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (gObjProp[TObj(pobj).cid][TObj(pobj).id].explmedia=gc_obj_explmedia_ship) then
      [*] = ;      GameObjectRollByHandle(gohnd, gc_obj_shipcontructrollangle);
      [*] = ;   end else _net_ErrorLog('invalid pointer: pobj=nil');
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadNew') then
      [*] = ;   _net_ErrorLog('RecordCustomBegin(''ReadNew'')');
      [*] = ;
      [*] = ;   var gouid: Integer;
      [*] = ;   if _net_IsServer or _net_IsRecord then //gohnd=0
      [*] = ;   gouid:=GetGameObjectUniqueIdByHandle(gohnd);
      [*] = ;   //else gouid:=GetMapNextUniqId;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;   RecordCustomWriteString(race);
      [*] = ;   RecordCustomWriteString(base);
      [*] = ;   RecordCustomWriteFloat(posx);
      [*] = ;   RecordCustomWriteFloat(posz);
      [*] = ;   RecordCustomWriteInteger(cid);
      [*] = ;   RecordCustomWriteInteger(gouid);
      [*] = ;   RecordCustomWriteInteger(GetPlayerGameObjectsCountByHandle(plhnd));
      [*] = ;
      [*] = ;   if _net_debug_log_new then
      [*] = ;   _net_Log('writenew(uid='+IntToStr(gouid)+';num='+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+';name=['+race+';'+IntToStr(cid)+';'+base+'];coords=['+FloatToStr(posx)+';'+FloatToStr(posz)+'])');
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;   _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;if (goHnd<>0) and (_net_IsServer or _net_IsOffline) then begin
      [*] = ;   if _unit_IsWaterUnit(goHnd) then
      [*] = ;   begin
      [*] = ;      var pobj: Pointer = _unit_GetTObj(gohnd);
      [*] = ;      if (pobj<>nil) and (TObj(pobj).bbuilt) then
      [*] = ;      begin
      [*] = ;         arg_plr.fid := GetPlayerIndexByHandle(plhnd);
      [*] = ;         arg_plr.fsid := base;
      [*] = ;         arg_plr.find := -1;
      [*] = ;         arg_plr.fofficer := gohnd;
      [*] = ;         arg_plr.fposition := True;
      [*] = ;         arg_plr.fbstate := False;
      [*] = ;         arg_plr.fintlst := nil;
      [*] = ;         PlayerExecuteStateByHandle(plHnd, 'WriteSquadNew');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;arg_plr.fresult:=gohnd;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

