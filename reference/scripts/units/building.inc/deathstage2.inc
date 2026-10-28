section.begin
   Name = DeathStage2
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;//GameObjectDestroyByHandle(myHnd);
      [*] = ;if (gScenario.bexists) and (not (gObjProp[arg_obj.cid][arg_obj.id].bwall)) and (GetMapCollisionTag(GetGameObjectMyPositionX, GetGameObjectMyPositionZ, False)=gc_collisiontag_terrain)  then
      [*] = ;begin
      [*] = ;   // bwall is handled in OnTagStates
      [*] = ;   _unit_SetTerrainCollision(myHnd, gc_collisiontag_none);
      [*] = ;   _misc_UnitTopologyUpdate(myHnd);
      [*] = ;end;
      [*] = ;
      [*] = ;GameObjectRequestToDestroyByHandle(myHnd);
      [*] = ;{
      [*] = ;var phnd: Integer = GetGameObjectPlayerHandleByHandle(myHnd);
      [*] = ;var parg: Pointer = StateMachineGetArgDataByInd(GetPlayerStateMachineHandle(phnd), gc_argplayer_plr);
      [*] = ;TPlayerArgs(parg).fhandle:=myHnd;
      [*] = ;PlayerExecuteStateByHandle(phnd, 'WriteFree');
      [*] = ;}
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

