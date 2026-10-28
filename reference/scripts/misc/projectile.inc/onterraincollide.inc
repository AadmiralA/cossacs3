section.begin
   Name = OnTerrainCollide
   Code : struct.begin
      [*] = ;args arg_proj : TProj;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;ExecuteState('DoExplode');
      [*] = ;
      [*] = ;if (GetGameObjectStringPropertyTag(myHnd)=gc_properties_stringtag_fogreveal) then
      [*] = ;DelFOWObjects(myHnd);
      [*] = ;
      [*] = ;SwitchTo('NothingEmpty');
      [*] = ;
      [*] = ;ExecuteState('FreeToPool'); // should be after SwitchTo
   struct.end
section.end

