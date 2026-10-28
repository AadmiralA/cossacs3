section.begin
   Name = DeathStage4
   Code : struct.begin
      [*] = ;BehaviourClear(GetGameObjectMyHandle);
      [*] = ;SetGameObjectOnStateDestroyByHandle(GetGameObjectMyHandle, '');
      [*] = ;//GameObjectDestroyByHandle(GetGameObjectMyHandle);
      [*] = ;GameObjectRequestToDestroyByHandle(GetGameObjectMyHandle);
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

