section.begin
   Name = ReadNewP
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bFromServer: Boolean = RecordCustomReadBoolean;
      [*] = ;var race: String = RecordCustomReadString;
      [*] = ;var base: String = RecordCustomReadString;
      [*] = ;var posx: Float = RecordCustomReadFloat;
      [*] = ;var posz: Float = RecordCustomReadFloat;
      [*] = ;var roll: Float = RecordCustomReadFloat;
      [*] = ;var plind : Integer = RecordCustomReadInteger;
      [*] = ;var id : Integer = RecordCustomReadInteger;
      [*] = ;var uid : Integer = RecordCustomReadInteger;
      [*] = ;var num : Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;if _net_IsServer then begin
      [*] = ;   arg_plr.fracename:=race;
      [*] = ;   arg_plr.fbasename:=base;
      [*] = ;   arg_plr.fposx:=posx;
      [*] = ;   arg_plr.fposz:=posz;
      [*] = ;   arg_plr.fdirx:=roll;
      [*] = ;   arg_plr.fplayer:=plind;
      [*] = ;   arg_plr.fplayer:=id;
      [*] = ;   ExecuteState('WriteNewP');
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;begin // _net_IsClient or _net_IsReplay
      [*] = ;   var gohnd: Integer;
      [*] = ;   var plhnd: Integer = GetPlayerHandleByIndex(plind);
      [*] = ;   // fixing fishing ships
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
      [*] = ;   if (_misc_GetBaseObjID(goHnd)=gc_baseid_obj) then
      [*] = ;   begin
      [*] = ;      var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;      if (pobj<>nil) then begin
      [*] = ;         if (gObjProp[TObj(pobj).cid][TObj(pobj).id].explmedia=gc_obj_explmedia_ship) then
      [*] = ;         begin
      [*] = ;            TObj(pobj).bbuilt := False;
      [*] = ;            TObj(pobj).buildprogress := 0;
      [*] = ;            TObj(pobj).hp := 10;
      [*] = ;            SetGameObjectCollisionInertiaByHandle(goHnd, false);
      [*] = ;            var tag : Integer = gc_statetag_essential_birth or gc_statetag_visual_stage_0;
      [*] = ;            _unit_SetTagStates(goHnd, tag);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   var gouid: Integer = GetGameObjectUniqueIdByHandle(gohnd);
      [*] = ;   if _net_debug_log_newp then
      [*] = ;   _net_Log('readnewp(uid='+IntToStr(gouid)+';num='+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+';name=['+race+';'+base+'];coords=['+FloatToStr(posx)+';'+FloatToStr(posz)+'])');
      [*] = ;   if uid<>gouid then
      [*] = ;   _net_ErrorLog('created invalid uid: coming('+IntToStr(uid)+') != current('+IntToStr(gouid)+')');
      [*] = ;   //if (plind<gc_MaxPlayerCount) and (num<>GetPlayerGameObjectsCountByHandle(plhnd)) then
      [*] = ;   //  _net_ErrorLog('number of units is different: coming('+IntToStr(num)+') != current('+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+')');
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

