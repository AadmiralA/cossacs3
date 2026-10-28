section.begin
   Name = SetRallyPoint
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;var x, y, z : Float;
      [*] = ;var pObjProp : Pointer = _unit_GetObjProp(myHnd);
      [*] = ;var transport : Boolean = (pObjProp <> nil) and (TObjProp(pObjProp).transport > 0);
      [*] = ;if _unit_GetAbsLastExitPoint(myHnd, true, x, y, z) then
      [*] = ;begin
      [*] = ;   if (not transport) or _misc_TestTransportExitPoint(x, z) then
      [*] = ;   begin
      [*] = ;      _unit_SetRallyPoint(myHnd, x, z, true, false);
      [*] = ;      if GetGameObjectPlayerHandleByHandle(myHnd) = GetPlayerHandleInterfaceIO then
      [*] = ;      gbool_gui_update_rallypoints := true;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   Log('Wrong place for rally point: (' + FloatToStr(x) + ', ' + FloatToStr(z) + ')');
      [*] = ;end;
      [*] = ;
      [*] = ;if transport then
      [*] = ;begin
      [*] = ;   var pArmy : Pointer = _unit_GetArmy(myHnd);
      [*] = ;   if pArmy <> nil then
      [*] = ;   begin
      [*] = ;      if TArmy(pArmy).fOrder.tag = gc_ai_armyorder_transport_waitloadunits then
      [*] = ;      TArmy(pArmy).fOrder.tag := gc_ai_armyorder_transport_loadunits;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

