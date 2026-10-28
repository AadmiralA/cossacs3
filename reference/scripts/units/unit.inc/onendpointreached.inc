section.begin
   Name = OnEndPointReached
   Code : struct.begin
      [*] = ;args arg_obj: TObj;
      [*] = ;
      [*] = ;var hnd: Integer = GetGameObjectMyHandle;
      [*] = ;var pObjProp : Pointer = _unit_GetObjProp(hnd);
      [*] = ;var ordRemove : Boolean = true;
      [*] = ;
      [*] = ;if not GetGameObjectMyCollisionDetection and (TObjProp(pobjprop).media=gc_obj_media_water) and (GetMapCollisionTag(GetGameObjectMyPositionX, GetGameObjectMyPositionZ, false)=gc_collisiontag_water) then begin
      [*] = ;   SetGameObjectMyCollisionDetection(true);
      [*] = ;   SetGameObjectMyTrackPointSkipPoints(true);
      [*] = ;end;
      [*] = ;
      [*] = ;if not arg_obj.bpathrequested then begin
      [*] = ;   var tag: Integer;
      [*] = ;   var prevtag: Integer = GetGameObjectMyStatesTag;
      [*] = ;   var transport : Boolean = (pObjProp <> 0) and (TObjProp(pObjProp).transport > 0);
      [*] = ;   if _unit_NeedTurn(hnd) and (not _unit_IsRunAway(hnd)) and ((prevTag and gc_statetag_essential_birth)=0) then
      [*] = ;   begin
      [*] = ;      if transport then
      [*] = ;      ordRemove := false;
      [*] = ;      tag:=gc_statetag_move_turn;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      if transport then
      [*] = ;      _unit_CheckTransportOpenTrap(hnd);
      [*] = ;      tag:=gc_statetag_move_idle;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if _net_IsServer or _net_IsRecord then
      [*] = ;   tag:=tag or gc_statetag_sync_endpoint;
      [*] = ;
      [*] = ;   if _net_IsClient or _net_IsReplay then begin
      [*] = ;      prevtag:=prevtag and (not gc_statetag_move);
      [*] = ;      prevtag:=prevtag and (not gc_statetag_sync_endpoint); // from gec
      [*] = ;      SetGameObjectMyStatesTag(prevtag or tag);
      [*] = ;      _unit_ForceStateTag(hnd, 0);
      [*] = ;   end else begin
      [*] = ;      if (((prevtag and gc_statetag_execute_move)<>0) and ((tag and gc_statetag_move)<>0)) then
      [*] = ;      tag := tag and (not gc_statetag_move);
      [*] = ;      _unit_SetTagStates(hnd, tag);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if ordRemove then
      [*] = ;begin
      [*] = ;   var tx, ty, tz : Float;
      [*] = ;   var tpcount : Integer = GetGameObjectMyTrackPointCount;
      [*] = ;   if tpcount>0 then
      [*] = ;   GetGameObjectMyTrackPointCoordsByIndex(tpcount-1, tx, ty, tz);
      [*] = ;
      [*] = ;   if (tpcount>0) and (VectorDistance(tx, 0, tz, GetGameObjectMyPositionX, 0, GetGameObjectMyPositionZ)>GetGameObjectMyEpsilonDistance) then
      [*] = ;   begin
      [*] = ;      _unit_RemoveOrder(hnd, 0);
      [*] = ;      //Log('OnEndPointReached : should be logged only if unit stop cause of new unpathable collision on his way (propable wall). other way it is ERROR');
      [*] = ;   end
      [*] = ;   else
      [*] = ;   case arg_obj.orders[0].itype of
      [*] = ;      gc_obj_order_type_move: begin
      [*] = ;         //if (arg_obj.orders[0].bremove) and (VectorDistance(arg_obj.orders[0].info.x, 0, arg_obj.orders[0].info.y, GetGameObjectMyPositionX, 0, GetGameObjectMyPositionZ)<GetGameObjectMyEpsilonDistance) then
      [*] = ;         if (arg_obj.orders[0].bremove) and (VectorDistance(tx, 0, tz, GetGameObjectMyPositionX, 0, GetGameObjectMyPositionZ)<GetGameObjectMyEpsilonDistance) then
      [*] = ;         _unit_RemoveOrder(hnd, 0);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end