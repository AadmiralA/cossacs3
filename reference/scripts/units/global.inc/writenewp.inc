section.begin
   Name = WriteNewP
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var race: String = arg_plr.fracename;
      [*] = ;var base: String = arg_plr.fbasename;
      [*] = ;var posx: Float = arg_plr.fposx;
      [*] = ;var posz: Float = arg_plr.fposz;
      [*] = ;var roll: Float = arg_plr.fdirx;
      [*] = ;var plind : Integer = arg_plr.fplayer;
      [*] = ;var id: Integer = arg_plr.fid;
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerHandleByIndex(plind);
      [*] = ;var gohnd: Integer;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then
      [*] = ;begin
      [*] = ;   gbool_unit_isshipdummy := True;
      [*] = ;   gohnd:=CreatePlayerGameObjectHandleByHandle(plhnd, race, base, posx, 0, posz);
      [*] = ;   gbool_unit_isshipdummy := False;
      [*] = ;   GameObjectRollByHandle(gohnd, roll);
      [*] = ;   if (base=gc_basename_field) then
      [*] = ;   begin
      [*] = ;      if (random>0.5) then
      [*] = ;      _misc_PlaceFieldDecal(gohnd, 60, 45)
      [*] = ;      else
      [*] = ;      _misc_PlaceFieldDecal(gohnd, 60, 45+180);
      [*] = ;   end
      [*] = ;   else
      [*] = ;   if (base=gc_basename_balloon) then
      [*] = ;   _misc_ApplyKeyColorRecursiveChild(gohnd, gMap.players[id].color);
      [*] = ;
      [*] = ;   if (gohnd<>0) then begin
      [*] = ;      if (_misc_GetBaseObjID(goHnd)=gc_baseid_obj) then
      [*] = ;      begin
      [*] = ;         var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;         if pobj<>nil then begin
      [*] = ;            if (gObjProp[TObj(pobj).cid][TObj(pobj).id].explmedia=gc_obj_explmedia_ship) then
      [*] = ;            begin
      [*] = ;               TObj(pobj).bbuilt := False;
      [*] = ;               TObj(pobj).buildprogress := 0;
      [*] = ;               TObj(pobj).hp := 10;
      [*] = ;               SetGameObjectCollisionInertiaByHandle(goHnd, false);
      [*] = ;               var tag : Integer = gc_statetag_essential_birth or gc_statetag_visual_stage_0;
      [*] = ;               _unit_SetTagStates(goHnd, tag);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadNewP') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadNewP'')');
      [*] = ;
      [*] = ;   var gouid: Integer;
      [*] = ;   if _net_IsServer or _net_IsRecord then //gohnd=0
      [*] = ;      gouid:=GetGameObjectUniqueIdByHandle(gohnd);
      [*] = ;   //else gouid:=GetMapNextUniqId;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;   RecordCustomWriteString(race);
      [*] = ;   RecordCustomWriteString(base);
      [*] = ;   RecordCustomWriteFloat(posx);
      [*] = ;   RecordCustomWriteFloat(posz);
      [*] = ;   RecordCustomWriteFloat(roll);
      [*] = ;   RecordCustomWriteInteger(plind);
      [*] = ;   RecordCustomWriteInteger(id);
      [*] = ;   RecordCustomWriteInteger(gouid);
      [*] = ;   RecordCustomWriteInteger(GetPlayerGameObjectsCountByHandle(plhnd));
      [*] = ;
      [*] = ;   if _net_debug_log_newp then
      [*] = ;      _net_Log('writenewp(uid='+IntToStr(gouid)+';num='+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+';name=['+race+';'+IntToStr(plind)+';'+base+'];coords=['+FloatToStr(posx)+';'+FloatToStr(posz)+'])');
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;      _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;arg_plr.fresult:=gohnd;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

