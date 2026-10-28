section.begin
   Name = OnAfterLoad
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;if arg_obj.bbuilt then
      [*] = ;_unit_TopologyAdd(myHnd);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

