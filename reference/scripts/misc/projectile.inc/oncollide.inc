section.begin
   Name = OnCollide
   Code : struct.begin
      [*] = ;args arg_proj : TProj;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;_weapon_OnCollide(GetGameObjectMyHandle);
      [*] = ;
      [*] = ;var goHnd : Integer = GetGameObjectMyStateCollisionObject;
      [*] = ;if (goHnd<>0) and (GetGameObjectHandleByUniqueId(arg_proj.owner)<>goHnd) then
      [*] = ;SwitchTo('Nothing')
      [*] = ;else
      [*] = ;SwitchTo('NothingEmpty');
   struct.end
section.end

