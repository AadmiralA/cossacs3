section.begin
   Name = Initial
   Code : struct.begin
      [*] = ;args arg_proj : TProj;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;if (gSerialMode=0) then
      [*] = ;begin
      [*] = ;   arg_proj.baseid := gc_baseid_prj;
      [*] = ;
      [*] = ;   const cBound = 0.25;
      [*] = ;   SetGameObjectCustomBoundingAABBByHandle(myHnd, -cBound, -cBound, -cBound, cBound, cBound, cBound);
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

