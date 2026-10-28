section.begin
   Name = FreeToPool
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;EffectClearRequested(myHnd);
      [*] = ;BehaviourClearRequested(myHnd);
      [*] = ;
      [*] = ;//FreeToPoolGameObjectHandle(myHnd);
      [*] = ;SetGameObjectMyVisible(False);
      [*] = ;SetGameObjectMyCollisionDetection(False);
      [*] = ;SwitchTo('FreeToPoolNext');
   struct.end
section.end

