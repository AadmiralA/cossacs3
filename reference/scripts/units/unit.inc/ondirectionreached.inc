section.begin
   Name = OnDirectionReached
   Code : struct.begin
      [*] = ;args arg_obj: TObj;
      [*] = ;
      [*] = ;SetGameObjectMyTargetRotatingMode('trmNone');
      [*] = ;
      [*] = ;if not arg_obj.bpathrequested and GetGameObjectMyPlayableObject then begin
      [*] = ;   var hnd: Integer = GetGameObjectMyHandle;
      [*] = ;   var tag: Integer;
      [*] = ;   if(GetGameObjectMyStatesTag and gc_statetag_move=gc_statetag_move_turn) then
      [*] = ;   tag:=gc_statetag_move_idle;
      [*] = ;   if _net_IsServer or _net_IsRecord then
      [*] = ;   tag:=tag or gc_statetag_sync_endpoint;
      [*] = ;   if _net_IsClient or _net_IsReplay then begin
      [*] = ;      var prevtag: Integer = GetGameObjectMyStatesTag;
      [*] = ;      if (tag<>0) then
      [*] = ;      prevtag:=prevtag and (not gc_statetag_move);
      [*] = ;      // prevtag:=prevtag and (not gc_statetag_sync_endpoint); // from gec
      [*] = ;      SetGameObjectMyStatesTag(prevtag or tag);
      [*] = ;      _unit_ForceStateTag(hnd, 0);
      [*] = ;      {SetGameObjectMyStatesTag(tag);
      [*] = ;      _unit_ForceStateTag(hnd, 0);}
      [*] = ;   end else begin
      [*] = ;      if (((GetGameObjectMyStatesTag and gc_statetag_execute_move)<>0) and ((tag and gc_statetag_move)<>0)) then
      [*] = ;      tag := tag and (not gc_statetag_move);
      [*] = ;      _unit_SetTagStates(hnd, tag);
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (arg_obj.orders[0].itype = gc_obj_order_type_move) and (arg_obj.orders[0].info.progress = gc_obj_order_move_mode_landing) then
      [*] = ;   begin
      [*] = ;      _unit_CheckTransportOpenTrap(hnd);
      [*] = ;
      [*] = ;      if arg_obj.orders[0].bRemove then
      [*] = ;      _unit_RemoveOrder(hnd, 0);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

