section.begin
   Name = DelayDestroy
   Code : struct.begin
      [*] = ;_misc_PrepareDestroyGameObject(GetGameObjectMyHandle);
      [*] = ;GameObjectRequestToDestroyByHandle(GetGameObjectMyHandle);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

