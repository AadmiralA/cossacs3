section.begin
   Name = ReadProj
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var uid : Integer = RecordCustomReadInteger;
      [*] = ;var trguid : Integer = RecordCustomReadInteger;
      [*] = ;var busetrgpos : Boolean = RecordCustomReadBoolean;
      [*] = ;var sx : Float = RecordCustomReadFloat;
      [*] = ;var sy : Float = RecordCustomReadFloat;
      [*] = ;var sz : Float = RecordCustomReadFloat;
      [*] = ;var weaponid : Integer = RecordCustomReadInteger;
      [*] = ;var tx : Float = RecordCustomReadFloat;
      [*] = ;var ty : Float = RecordCustomReadFloat;
      [*] = ;var tz : Float = RecordCustomReadFloat;
      [*] = ;var cid : Integer = RecordCustomReadInteger;
      [*] = ;var id : Integer = RecordCustomReadInteger;
      [*] = ;var weapind : Integer = RecordCustomReadInteger;
      [*] = ;var randkey : Float = RecordCustomReadFloat;
      [*] = ;var projuid : Integer = RecordCustomReadInteger;
      [*] = ;var num : Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;var hnd : Integer =  GetGameObjectHandleByUniqueId(uid);
      [*] = ;var trg : Integer =  GetGameObjectHandleByUniqueId(trguid);
      [*] = ;var projhnd : Integer = _weapon_CreateProjectileByWeaponID(hnd, trg, busetrgpos, sx, sy, sz, weaponid, tx, ty, tz, cid, id, weapind, randkey);
      [*] = ;var newprojuid : Integer = GetMapNextUniqId;
      [*] = ;GameObjectChangeUniqIdByHandle(projhnd, newprojuid);
      [*] = ;//var newprojuid : Integer = GameObjectMakeUniqId(projhnd); // general projs dont have uids. this used to control sync
      [*] = ;
      [*] = ;if _net_debug_log_projnew then
      [*] = ;   _net_Log('readproj(uid='+IntToStr(newprojuid)+';num='+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+';weaponsid=['+gWeapons[weaponid].sid+'];coords=['+FloatToStr(sx)+';'+FloatToStr(sy)+';'+FloatToStr(sz)+'])');
      [*] = ;if projuid<>newprojuid then
      [*] = ;   _net_ErrorLog('created invalid proj uid: coming('+IntToStr(projuid)+') != current('+IntToStr(newprojuid)+')');
      [*] = ;//if num<>GetPlayerGameObjectsCountByHandle(plhnd) then
      [*] = ;//   _net_ErrorLog('number of units in player misc is different: coming('+IntToStr(num)+') != current('+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

