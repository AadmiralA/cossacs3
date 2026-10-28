section.begin
   Name = OnAclAnimationReachedAttackEnd
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;if (not arg_obj.bdead) then
      [*] = ;begin
      [*] = ;   var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;   if (arg_obj.orders[0].itype<>gc_obj_order_type_guard) and (arg_obj.orders[0].itype<>gc_obj_order_type_gainres) and (arg_obj.orders[0].itype<>gc_obj_order_type_build) then
      [*] = ;   _unit_TryAttack(myHnd, arg_obj.orders[0].info.trg, True)
      [*] = ;   else
      [*] = ;   _unit_TryAttack(myHnd, 0, True);
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

