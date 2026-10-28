section.begin
   Name = WriteProj
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var projhnd: Integer;
      [*] = ;var hnd : Integer = arg_plr.fhandle;
      [*] = ;var trg : Integer = arg_plr.ftarget;
      [*] = ;var busetrgpos : Boolean = arg_plr.fbstate;
      [*] = ;var sx : Float = arg_plr.fposx;
      [*] = ;var sy : Float = arg_plr.fposy;
      [*] = ;var sz : Float = arg_plr.fposz;
      [*] = ;var weaponid : Integer = arg_plr.fweaponid;
      [*] = ;var tx : Float = arg_plr.ftrgx;
      [*] = ;var ty : Float = arg_plr.ftrgy;
      [*] = ;var tz : Float = arg_plr.ftrgz;
      [*] = ;var cid : Integer = arg_plr.fcid;
      [*] = ;var id : Integer = arg_plr.fid;
      [*] = ;var weapind : Integer = arg_plr.find;
      [*] = ;var randkey : Float = arg_plr.frnd;
      [*] = ;var plhnd: Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then
      [*] = ;projhnd := _weapon_CreateProjectileByWeaponID(hnd, trg, busetrgpos, sx, sy, sz, weaponid, tx, ty, tz, cid, id, weapind, randkey);
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadProj') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadProj'')');
      [*] = ;
      [*] = ;   var projuid: Integer; //projuid:=GetGameObjectUniqueIdByHandle(projhnd);
      [*] = ;   if _net_IsServer or _net_IsRecord then
      [*] = ;   begin
      [*] = ;      //projuid:=GameObjectMakeUniqId(projhnd);
      [*] = ;      projuid:=GetMapNextUniqId;
      [*] = ;      GameObjectChangeUniqIdByHandle(projhnd, projuid);
      [*] = ;   end;
      [*] = ;   //else projuid:=GetMapNextUniqId;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteInteger(GetGameObjectUniqueIdByHandle(hnd));
      [*] = ;   RecordCustomWriteInteger(GetGameObjectUniqueIdByHandle(trg));
      [*] = ;   RecordCustomWriteBoolean(busetrgpos);
      [*] = ;   RecordCustomWriteFloat(sx);
      [*] = ;   RecordCustomWriteFloat(sy);
      [*] = ;   RecordCustomWriteFloat(sz);
      [*] = ;   RecordCustomWriteInteger(weaponid);
      [*] = ;   RecordCustomWriteFloat(tx);
      [*] = ;   RecordCustomWriteFloat(ty);
      [*] = ;   RecordCustomWriteFloat(tz);
      [*] = ;   RecordCustomWriteInteger(cid);
      [*] = ;   RecordCustomWriteInteger(id);
      [*] = ;   RecordCustomWriteInteger(weapind);
      [*] = ;   RecordCustomWriteFloat(randkey);
      [*] = ;   RecordCustomWriteInteger(projuid);
      [*] = ;   RecordCustomWriteInteger(GetPlayerGameObjectsCountByHandle(plhnd));
      [*] = ;
      [*] = ;   if _net_debug_log_projnew then
      [*] = ;      _net_Log('writeproj(uid='+IntToStr(projuid)+';num='+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+';weaponsid=['+gWeapons[weaponid].sid+'];startcoords=['+FloatToStr(sx)+';'+FloatToStr(sy)+';'+FloatToStr(sz)+'])');
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;      _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;arg_plr.fresult:=projhnd;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

