section.begin
   Name = OnDestroy
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;if (GetGameObjectStringPropertyTag(myHnd)=gc_properties_stringtag_fogreveal) then
      [*] = ;DelFOWObjects(myHnd);
      [*] = ;
      [*] = ;BehaviourClear(myHnd);
      [*] = ;
      [*] = ;SwitchTo('NothingEmpty');
   struct.end
section.end

