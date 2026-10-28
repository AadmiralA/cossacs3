section.begin
   Name = OnDestroy
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;if (_misc_GetBaseObjID(myHnd)=gc_baseid_res) then
      [*] = ;begin
      [*] = ;   args arg_res : TRes; //doesnt work without this line. cause of arg indexation. looks like this state compiled before Initial state. some kind of sh.t
      [*] = ;   args arg_stolist : TIntegerList;
      [*] = ;   args arg_attlist : TIntegerList;
      [*] = ;   args arg_ordlist : TIntegerList;
      [*] = ;   
      [*] = ;   GameObjectDestroyDecalByHandle(myHnd);
      [*] = ;   var i : Integer;
      [*] = ;   for i:=arg_stolist.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var trgHnd : Integer = arg_stolist.Get(i);
      [*] = ;      if (trgHnd<>0) then
      [*] = ;      _unit_SetSTO(trgHnd, 0);
      [*] = ;   end;
      [*] = ;   
      [*] = ;   for i:=arg_ordlist.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var trgHnd : Integer = arg_ordlist.Get(i);
      [*] = ;      if (trgHnd<>0) then
      [*] = ;      begin
      [*] = ;         var j : Integer;
      [*] = ;         var pobj2 : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;         if (pobj2<>nil) then
      [*] = ;         for j:=gc_obj_MaxOrderCount-1 downto 0 do
      [*] = ;         if (TObj(pobj2).orders[j].info.trg=myHnd) then
      [*] = ;         _unit_RemoveOrder(trgHnd, j);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   
      [*] = ;   arg_attlist.Clear;
      [*] = ;   
      [*] = ;   if (arg_res.itype=gc_resource_type_gold) or (arg_res.itype=gc_resource_type_iron) or (arg_res.itype=gc_resource_type_coal) then
      [*] = ;   begin
      [*] = ;      for i:=gc_MaxPlayerCount-1 downto 0 do
      [*] = ;      gPlayer[i].aidata.orelist.Remove(myHnd);
      [*] = ;
      [*] = ;      var top : Integer = _gui_GetTop;
      [*] = ;      if (top<>0) then
      [*] = ;      begin
      [*] = ;         var hudCollection : Integer = GetGUIElementIndexByNameParent('envhud', top);
      [*] = ;         if (hudCollection<>0) then
      [*] = ;         begin
      [*] = ;            var hudHnd : Integer = GetGUIGroupHUDCollectionItemByGameObject(hudCollection, myHnd);
      [*] = ;            if (hudHnd<>0) then
      [*] = ;            GUIGroupHUDCollectionDeleteGameObject(hudCollection, myHnd);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   
      [*] = ;   _res_ResGridRemove(myHnd);
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

