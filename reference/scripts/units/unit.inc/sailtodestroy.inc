section.begin
   Name = SailToDestroy
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var pArmy : Pointer = _unit_GetArmy(myHnd);
      [*] = ;
      [*] = ;if _ai_IsArmyTransport(pArmy) then
      [*] = ;begin
      [*] = ;   var tZone : Integer = TArmy(pArmy).fCurTopZone;
      [*] = ;   var i : Integer;
      [*] = ;   var myX : Float = TArmy(pArmy).fCurX;
      [*] = ;   var myZ : Float = TArmy(pArmy).fCurZ;
      [*] = ;   var maxDist : Float;
      [*] = ;   var tarX, tarZ : Float;
      [*] = ;
      [*] = ;   if tZone >= 0 then
      [*] = ;   begin
      [*] = ;      for i := TopologyGetZoneNeighboursCountByIndex(tZone)-1 downto 0 do
      [*] = ;      begin
      [*] = ;         var nZone : Integer = TopologyGetZoneNeighbourByIndex(tZone, i);
      [*] = ;         var x, z : Float;
      [*] = ;         TopologyGetZoneCenterByIndex(nZone, x, z);
      [*] = ;         var dist : Float = VectorDistance(myX, 0, myZ, x, 0, z);
      [*] = ;         if dist > maxDist then
      [*] = ;         begin
      [*] = ;            maxDist := dist;
      [*] = ;            tarX := x;
      [*] = ;            tarZ := z;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if maxDist > 0 then
      [*] = ;      begin
      [*] = ;         _ai_SetArmyPosition(pArmy, tarX, tarZ, 0, 0, gc_obj_order_move_mode_default);
      [*] = ;         TArmy(pArmy).fOrder.tag := gc_ai_armyorder_transport_destroy;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

