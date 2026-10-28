section.begin
   Name = FreeToPoolNext
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;if (GetGameObjectUniqueIdByHandle(myHnd)<>0) then
      [*] = ;GameObjectChangeUniqIdByHandle(myHnd, 0);
      [*] = ;
      [*] = ;FreeToPoolGameObjectHandle(myHnd);
      [*] = ;
      [*] = ;SwitchTo('NothingEmpty');
   struct.end
section.end

