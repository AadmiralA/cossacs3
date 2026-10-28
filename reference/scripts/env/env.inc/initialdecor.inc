section.begin
   Name = InitialDecor
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;SetGameObjectMyOnStateEndFrameReached('');
      [*] = ;SetGameObjectOnStateDestroyByHandle(myHnd, 'OnDestroy');
      [*] = ;
      [*] = ;SetGameObjectDeferredFrameBlendByHandle(myHnd, false);
      [*] = ;
      [*] = ;if (gSerialMode=0) and (GetGameObjectUniqueIdByHandle(myHnd)=0) then
      [*] = ;begin
      [*] = ;   GameObjectMakeUniqId(myHnd);
      [*] = ;
      [*] = ;   SetGameObjectMyOnStateEndPointReached('');
      [*] = ;   SetGameObjectMyOnStateDirectionReached('');
      [*] = ;   SetGameObjectMyOnStateEndCyclesReached('');
      [*] = ;   SetGameObjectMyStateTargetPosition(GetGameObjectMyPositionX, GetGameObjectMyPositionY, GetGameObjectMyPositionZ);
      [*] = ;   GameObjectMyTrackPointClear;
      [*] = ;   SetGameObjectMyTrackPointMovementMode('mmNone');
      [*] = ;   SetGameObjectMyTargetRotatingMode('trmNone');
      [*] = ;   //SetGameObjectMyAnimationControlerEnabled(true);
      [*] = ;   SetGameObjectMyPlayableObject(False);
      [*] = ;   SetGameObjectMyCollisionDetection(False);
      [*] = ;   SetGameObjectCollisionInertiaByHandle(myHnd, False);
      [*] = ;   SetGameObjectCIMovableByHandle(myHnd, False);
      [*] = ;   SetGameObjectCIIntersectRadiusByHandle(myHnd, 0);
      [*] = ;   _unit_SetSTO(myHnd, 0);
      [*] = ;
      [*] = ;   if (GetGameObjectMyPositionX>(GetMapWidth div 2)) or (GetGameObjectMyPositionX<(-GetMapWidth div 2)) or (GetGameObjectMyPositionZ>(GetMapHeight div 2)) or (GetGameObjectMyPositionZ<(-GetMapHeight div 2)) then
      [*] = ;   begin
      [*] = ;      //GameObjectRequestToDestroyByHandle(myHnd);
      [*] = ;      Log('unit : InitialDecor : obj out of map. removed');
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SetGameObjectCollisionDetectionByHandle(myHnd, False);
      [*] = ;
      [*] = ;SwitchTo('NothingDecor');
   struct.end
section.end

