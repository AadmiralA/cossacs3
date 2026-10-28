section.begin
   Name = OnEndFrameReached
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;FreeToPoolGameObjectHandle(myHnd);
      [*] = ;SetGameObjectVisibleByHandle(myHnd, False);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

