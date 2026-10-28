section.begin
   Name = DoDeath
   Code : struct.begin
      [*] = ;args arg_proj : TProj;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var dechnd : Integer = GetGameObjectDecalHandleByHandle(myHnd);
      [*] = ;if (dechnd<>0) then
      [*] = ;GameObjectDestroyDecalByHandle(myHnd); //DestroyDecalByHandle(dechnd); // sometimes crashes
      [*] = ;
      [*] = ;if (GetGameObjectStringPropertyTag(myHnd)=gc_properties_stringtag_fogreveal) then
      [*] = ;DelFOWObjects(myHnd);
      [*] = ;
      [*] = ;SwitchTo('NothingEmpty');
      [*] = ;
      [*] = ;ExecuteState('FreeToPool'); // should be after SwitchTo
   struct.end
section.end

