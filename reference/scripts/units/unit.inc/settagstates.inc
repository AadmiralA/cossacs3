section.begin
   Name = SetTagStates
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;var switchState : Integer = gint_unit_tagstate;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var oldState : Integer = GetGameObjectMyStatesTag;
      [*] = ;var newState : Integer = oldState;
      [*] = ;
      [*] = ;var iOldResource : Integer = oldState and gc_statetag_resource;
      [*] = ;var iOldExecute : Integer = oldState and gc_statetag_execute;
      [*] = ;var iOldMove : Integer = oldState and gc_statetag_move;
      [*] = ;var iOldWeapon : Integer = oldState and gc_statetag_weapon;
      [*] = ;var iOldAction : Integer = oldState and gc_statetag_action;
      [*] = ;var iOldEssential : Integer = oldState and gc_statetag_essential;
      [*] = ;var iOldVisual : Integer = oldState and gc_statetag_visual;
      [*] = ;
      [*] = ;var switchResource : Integer = (switchState and gc_statetag_resource);
      [*] = ;if (switchResource<>0) then
      [*] = ;newState := (newState and not gc_statetag_resource) or switchResource;
      [*] = ;
      [*] = ;var switchExecute : Integer = (switchState and gc_statetag_execute);
      [*] = ;if (switchExecute<>0) then
      [*] = ;newState := (newState and not gc_statetag_execute) or switchExecute;
      [*] = ;
      [*] = ;var switchMove : Integer = (switchState and gc_statetag_move);
      [*] = ;if (switchMove<>0) then
      [*] = ;newState := (newState and not gc_statetag_move) or switchMove;
      [*] = ;
      [*] = ;var switchWeapon : Integer = (switchState and gc_statetag_weapon);
      [*] = ;if (switchWeapon<>0) then
      [*] = ;newState := (newState and not gc_statetag_weapon) or switchWeapon;
      [*] = ;
      [*] = ;var switchAction : Integer = (switchState and gc_statetag_action);
      [*] = ;if (switchAction<>0) then
      [*] = ;newState := (newState and not gc_statetag_action) or switchAction;
      [*] = ;
      [*] = ;var switchEssential : Integer = (switchState and gc_statetag_essential);
      [*] = ;if (switchEssential<>0) then
      [*] = ;newState := (newState and not gc_statetag_essential) or switchEssential;
      [*] = ;
      [*] = ;var switchVisual : Integer = (switchState and gc_statetag_visual);
      [*] = ;if (switchVisual<>0) then
      [*] = ;newState := (newState and not gc_statetag_visual) or switchVisual;
      [*] = ;
      [*] = ;var iResource : Integer = newState and gc_statetag_resource;
      [*] = ;//var iExecute : Integer = newState and gc_statetag_execute;
      [*] = ;var iMove : Integer = newState and gc_statetag_move;
      [*] = ;//var iWeapon : Integer = newState and gc_statetag_weapon;
      [*] = ;//var iAction : Integer = newState and gc_statetag_action;
      [*] = ;var iEssential : Integer = newState and gc_statetag_essential;
      [*] = ;var iVisual : Integer = newState and gc_statetag_visual;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsRecord then begin
      [*] = ;   var switchendpoint: Integer = (switchState and gc_statetag_sync_endpoint);
      [*] = ;   if (switchendpoint<>0) then
      [*] = ;   newState := (newState and not gc_statetag_sync_endpoint) or switchendpoint;
      [*] = ;   //var syncstp: Integer = (switchState and gc_statetag_sync_stp);
      [*] = ;   //if (syncstp<>0) then
      [*] = ;   //newState := (newState and not gc_statetag_sync_stp) or syncstp;
      [*] = ;end;
      [*] = ;
      [*] = ;if (switchEssential=gc_statetag_essential_death) then
      [*] = ;begin
      [*] = ;   if (_unit_IsWaterUnit(myHnd)) then
      [*] = ;   newState := (newState and not gc_statetag_visual) or gc_statetag_visual_stage_0;
      [*] = ;
      [*] = ;   //log('this one died='+IntToStr(GetGameObjectUniqueIdByHandle(myHnd))+' his sto='+IntToStr(GetGameObjectUniqueIdByHandle(GetGameObjectSTOHandleByHandle(myHnd))));
      [*] = ;   newState := (newState xor gc_statetag_execute_move) or gc_statetag_execute_none;
      [*] = ;   newState := (newState xor gc_statetag_move_walk) or gc_statetag_move_idle;
      [*] = ;   newState := (newState xor gc_statetag_move_turn) or gc_statetag_move_idle;
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   if (switchExecute=gc_statetag_execute_move) or ((switchAction<>0){to prevent archer to shake, when his maxrange changes due to standtime>0.25 bonus} and (switchAction<>gc_statetag_action_none) and (switchAction<>gc_statetag_action_attack)) then
      [*] = ;   arg_obj.standtime := 0;
      [*] = ;
      [*] = ;   if (switchMove=gc_statetag_move_turn) then
      [*] = ;   begin
      [*] = ;      if (not _unit_NeedTurn(myHnd)) then
      [*] = ;      newState := (newState and not gc_statetag_move) or gc_statetag_move_idle;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if ((switchMove<>0) and (switchMove<>gc_statetag_move_walk)) then
      [*] = ;   begin
      [*] = ;      if (iMove=gc_statetag_move_idle) then
      [*] = ;      begin
      [*] = ;         var dirX : Float = GetGameObjectTransformedVirtualDirectionXByHandle(myHnd);
      [*] = ;         var dirZ : Float = GetGameObjectTransformedVirtualDirectionZByHandle(myHnd);
      [*] = ;         var angle : Float = _misc_GetDirAngleToXVector(dirX, dirZ);
      [*] = ;         if Abs(angle-GetGameObjectSTArrowAngleByHandle(myHnd))>GetGameObjectMyEpsilonAngle then
      [*] = ;         SetGameObjectSTArrowAngleByHandle(myHnd, angle);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (iMove=gc_statetag_move_turn) then
      [*] = ;      begin
      [*] = ;         var stoHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;         if (stoHnd<>0) then
      [*] = ;         begin
      [*] = ;            if (not gObjProp[arg_obj.cid][arg_obj.id].bshotdirection) then
      [*] = ;            begin
      [*] = ;               var angle : Float = _misc_GetAngleToXVector(GetGameObjectMyPositionX, GetGameObjectMyPositionZ, GetGameObjectPositionXByHandle(stoHnd), GetGameObjectPositionZByHandle(stoHnd));
      [*] = ;               if Abs(angle-GetGameObjectSTArrowAngleByHandle(myHnd))>GetGameObjectMyEpsilonAngle then
      [*] = ;               SetGameObjectSTArrowAngleByHandle(myHnd, angle);
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            {if (VectorDistance(GetGameObjectMyPositionX, 0, GetGameObjectMyPositionZ, GetGameObjectMyStateTargetPositionX, 0, GetGameObjectMyStateTargetPositionZ)>GetGameObjectMyEpsilonDistance) then
      [*] = ;            begin
      [*] = ;               var angle : Float = _misc_GetAngleToXVector(GetGameObjectMyPositionX, GetGameObjectMyPositionZ, GetGameObjectMyStateTargetPositionX, GetGameObjectMyStateTargetPositionZ);
      [*] = ;               SetGameObjectSTArrowAngleByHandle(myHnd, angle);
      [*] = ;            end;}
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      _unit_SetSTP(myHnd, GetGameObjectMyPositionX, GetGameObjectMyPositionZ);
      [*] = ;      newState := newState or gc_statetag_sync_stp;
      [*] = ;      newState := newState or gc_statetag_sync_endpoint;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (arg_obj.resamount>0) then
      [*] = ;   begin
      [*] = ;      case arg_obj.restype of
      [*] = ;         gc_resource_type_food : switchResource := gc_statetag_resource_food;
      [*] = ;         gc_resource_type_wood : switchResource := gc_statetag_resource_wood;
      [*] = ;         gc_resource_type_stone : switchResource := gc_statetag_resource_stone;
      [*] = ;      end;
      [*] = ;      newState := (newState and not gc_statetag_resource) or switchResource;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   if (iResource<>gc_statetag_resource_none) then
      [*] = ;   newState := (newState and not gc_statetag_resource) or gc_statetag_resource_none;
      [*] = ;end;
      [*] = ;
      [*] = ;if (switchAction<>gc_statetag_action_attack) and (iVisual=gc_statetag_visual_stage_0) and (not _unit_IsWaterUnit(myHnd)) then
      [*] = ;newState := (newState and not gc_statetag_visual) or gc_statetag_visual_none;
      [*] = ;
      [*] = ;if (switchAction=gc_statetag_action_attack) and (iMove<>gc_statetag_move_idle) then
      [*] = ;newState := (newState and not gc_statetag_action) or gc_statetag_action_none; // fix units melee that comes to target, have action=attack, move=idle, and animation idle0 and stack in this situation
      [*] = ;
      [*] = ;if (switchVisual=gc_statetag_visual_hide) and (iEssential=gc_statetag_essential_none) then
      [*] = ;begin
      [*] = ;   ErrorLog('SetTagStates : something gone wrong and unit got visual_hide and essential_none. that should be impossible. and may lead to crash. I fix some things here');
      [*] = ;   newState := (newState and not gc_statetag_essential) or gc_statetag_essential_birth;
      [*] = ;end;
      [*] = ;
      [*] = ;if (newState<>oldState) then
      [*] = ;SetGameObjectMyStatesTag(newState);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

