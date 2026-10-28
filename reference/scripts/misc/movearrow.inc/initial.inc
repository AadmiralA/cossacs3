section.begin
   Name = Initial
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;//SetGameObjectOnStateDestroyByHandle(myHnd, 'OnDestroy');
      [*] = ;
      [*] = ;if (gSerialMode=0){and (GetGameObjectUniqueIdByHandle(myHnd)=0)} then
      [*] = ;begin
      [*] = ;   //GameObjectMakeUniqId(myHnd);
      [*] = ;   SetGameObjectMyPlayableObject(False);
      [*] = ;   SetGameObjectMyCollisionDetection(False);
      [*] = ;   SetGameObjectDeferredFrameBlendByHandle(myHnd, False);
      [*] = ;   SetGameObjectMyAnimationMode('aamPlayOnce');
      [*] = ;   GameObjectMySwitchToFrameAnimation('idle', false);
      [*] = ;   SetGameObjectMyCurrentFrame(0);
      [*] = ;   SetGameObjectMyOnStateEndFrameReached('OnEndFrameReached');
      [*] = ;   SetGameObjectVisibleByHandle(myHnd, True);
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

