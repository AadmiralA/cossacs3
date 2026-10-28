section.begin
   Name = OnAfterLoad
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;if (not GetGameObjectMyVisible) and (GetGameObjectGrHandleByHandle(myHnd)=0) then
      [*] = ;FreeToPoolGameObjectHandle(myHnd);
   struct.end
section.end

