section.begin
   Name = DoExtract
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;if (_misc_GetBaseObjID(myHnd)=gc_baseid_res) then
      [*] = ;begin
      [*] = ;   args arg_res : TRes;
      [*] = ;   
      [*] = ;   if (arg_res.itype=gc_resource_type_food) then
      [*] = ;   arg_res.hp := arg_res.hp-1;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

