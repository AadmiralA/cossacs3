section.begin
   Name = Initial
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;SetGameObjectOnStateDestroyByHandle(myHnd, 'OnDestroy');
      [*] = ;
      [*] = ;if (gSerialMode=0) and (GetGameObjectUniqueIdByHandle(myHnd)=0) then
      [*] = ;begin
      [*] = ;   GameObjectMakeUniqId(myHnd);
      [*] = ;   SetGameObjectMyPlayableObject(False);
      [*] = ;   SetGameObjectMyCollisionDetection(False);
      [*] = ;   SetGameObjectDeferredFrameBlendByHandle(myHnd, False);
      [*] = ;   SetGameObjectMyAnimationCyclesMode('acmPlayOnce');
      [*] = ;   SetGameObjectMyAnimationControlerEnabled(true);
      [*] = ;   SetGameObjectVisibleByHandle(myHnd, True);
      [*] = ;   SetGameObjectMyStatesTag(gc_statetag_essential_birth or gc_statetag_visual_stage_0);
      [*] = ;   SetGameObjectMyStatesTagForceUpdate;
      [*] = ;   ResetGameObjectPrevStatesTagByHandle(myHnd);
      [*] = ;end;
      [*] = ;if (gSerialMode>0) then // OnLoad
      [*] = ;begin
      [*] = ;   if (GetGameObjectCountChildByHandle(myHnd)>0) then
      [*] = ;   begin
      [*] = ;      var child : Integer = GetGameObjectGOHandleChildByHandle(myHnd, 0);
      [*] = ;      if child<>0 then
      [*] = ;      SetGameObjectVisibleByHandle(child, False);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

