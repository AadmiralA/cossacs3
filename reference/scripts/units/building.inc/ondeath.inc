section.begin
   Name = OnDeath
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var i : Integer;
      [*] = ;for i:=gc_obj_MaxOrderCount-1 downto 0 do
      [*] = ;begin
      [*] = ;   var porderinfo : Pointer = arg_obj.orders[i].info;
      [*] = ;   case arg_obj.orders[i].itype of
      [*] = ;      gc_obj_order_type_produce : begin
      [*] = ;         if (TOrderInfo(porderinfo).amount<>0) then
      [*] = ;         begin
      [*] = ;            gIntegerList.Clear;
      [*] = ;            gIntegerList.Add(myHnd);
      [*] = ;            _unit_ProduceUnit(GetGameObjectPlayerHandleByHandle(myHnd), gIntegerList, arg_obj.cid, TOrderInfo(porderinfo).produceid, TOrderInfo(porderinfo).amount, False, False, True);
      [*] = ;            TOrderInfo(porderinfo).amount := 0;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      gc_obj_order_type_performupgrade : begin
      [*] = ;         _unit_CancelUpgradePerform(myHnd, gCountry[arg_obj.cid].upgrade[TOrderInfo(porderinfo).upgradeid].id);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (arg_obj.scenariogroupid>-1) then
      [*] = ;begin
      [*] = ;   gScenario.groups[arg_obj.scenariogroupid].unitslist.Remove(myHnd);
      [*] = ;   if (gScenario.ui.bshowui) then
      [*] = ;   gScenarioTmp.groups[arg_obj.scenariogroupid].unitslist.Remove(myHnd);
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
      [*] = ;
      [*] = ;_unit_DestroyObj(myHnd);
   struct.end
section.end

