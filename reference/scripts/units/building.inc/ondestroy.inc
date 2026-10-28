section.begin
   Name = OnDestroy
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;if (not arg_obj.bdead) then
      [*] = ;ExecuteState('OnDeath');
      [*] = ;
      [*] = ;_misc_PrepareDestroyGameObject(myHnd);
      [*] = ;
      [*] = ;if (gScenario.bexists) and (GetMapCollisionTag(GetGameObjectMyPositionX, GetGameObjectMyPositionZ, False)=gc_collisiontag_terrain) then
      [*] = ;begin
      [*] = ;   _unit_SetTerrainCollision(myHnd, gc_collisiontag_none);
      [*] = ;   _misc_UnitTopologyUpdate(myHnd);
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   var phnd: Integer = GetGameObjectPlayerHandleByHandle(myHnd);
      [*] = ;   var parg: Pointer = StateMachineGetArgDataByInd(GetPlayerStateMachineHandle(phnd), gc_argplayer_plr);
      [*] = ;   if parg<>nil then begin
      [*] = ;      TPlayerArgs(parg).fhandle:=myHnd;
      [*] = ;      PlayerExecuteStateByHandle(phnd, 'WriteFree');
      [*] = ;   end else _net_ErrorLog('invalid pointer, parg=nil');
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

