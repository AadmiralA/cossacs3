section.begin
   Name = ReadFree
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var uid: Integer = RecordCustomReadInteger;
      [*] = ;var num: Integer = RecordCustomReadInteger;
      [*] = ;var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;
      [*] = ;if hnd<>0 then begin
      [*] = ;   var pobj: Pointer = _unit_GetTObj(hnd);
      [*] = ;   SetGameObjectOnStateDestroyByHandle(hnd, '');
      [*] = ;   if not TObj(pobj).bdead then
      [*] = ;      GameObjectExecuteStateByHandle(hnd, 'OnDeath');
      [*] = ;   EffectHighlightRemove(hnd, ''); // execute before EffectClear
      [*] = ;   EffectClear(hnd); // execute after EffectHighlightRemove
      [*] = ;   _misc_PrepareDestroyGameObject(hnd);
      [*] = ;   //GameObjectDestroyByHandle(hnd);
      [*] = ;   GameObjectRequestToDestroyByHandle(hnd);
      [*] = ;end else _net_ErrorLog('free invalid uid('+IntToStr(uid)+')');
      [*] = ;
      [*] = ;//if num<>GetPlayerGameObjectsCountByHandle(GetPlayerCurrentPlHandle) then
      [*] = ;//   _net_ErrorLog('free invalid count: coming('+IntToStr(num)+') != current('+IntToStr(GetPlayerGameObjectsCountByHandle(GetPlayerCurrentPlHandle))+')');
      [*] = ;
      [*] = ;if _net_debug_log_free then
      [*] = ;   _net_Log('readfree(uid='+IntToStr(uid)+';num='+IntToStr(GetPlayerGameObjectsCountByHandle(GetPlayerCurrentPlHandle))+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

