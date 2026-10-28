section.begin
   Name = OnTagStates
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;procedure DebugLogStateTag(iEssential, iVisual : Integer);
      [*] = ;begin
      [*] = ;   var s, val : String = '';
      [*] = ;   if (iEssential<>0) then
      [*] = ;   begin
      [*] = ;      case iEssential of
      [*] = ;         gc_statetag_essential_none : val := 'none';
      [*] = ;         gc_statetag_essential_birth : val := 'birth';
      [*] = ;         gc_statetag_essential_death : val := 'death';
      [*] = ;      end;
      [*] = ;      s := s+'iEssential='; if val<>'' then s:=s+val else s:=s+IntToStr(iEssential); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (iVisual<>0) then
      [*] = ;   begin
      [*] = ;      case iVisual of
      [*] = ;         gc_statetag_visual_none : val := 'none';
      [*] = ;         gc_statetag_visual_stage_0 : val := 'stage0';
      [*] = ;         gc_statetag_visual_stage_1 : val := 'stage1';
      [*] = ;         gc_statetag_visual_stage_2 : val := 'stage2';
      [*] = ;         gc_statetag_visual_stage_3 : val := 'stage3';
      [*] = ;      end;
      [*] = ;      s := s+'iVisual='; if val<>'' then s:=s+val else s:=s+IntToStr(iVisual); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if s='' then s:='none';
      [*] = ;   Log(s);
      [*] = ;end;
      [*] = ;
      [*] = ;procedure DebugLogStateTagSwitches(switchEssential, switchVisual : Integer);
      [*] = ;begin
      [*] = ;   var s, val : String = '';
      [*] = ;   if (switchEssential<>0) then
      [*] = ;   begin
      [*] = ;      case switchEssential of
      [*] = ;         gc_statetag_essential_none : val := 'none';
      [*] = ;         gc_statetag_essential_birth : val := 'birth';
      [*] = ;         gc_statetag_essential_death : val := 'death';
      [*] = ;      end;
      [*] = ;      s := s+'switchEssential='; if val<>'' then s:=s+val else s:=s+IntToStr(switchEssential); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (switchVisual<>0) then
      [*] = ;   begin
      [*] = ;      case switchVisual of
      [*] = ;         gc_statetag_visual_none : val := 'none';
      [*] = ;         gc_statetag_visual_stage_0 : val := 'stage0';
      [*] = ;         gc_statetag_visual_stage_1 : val := 'stage1';
      [*] = ;         gc_statetag_visual_stage_2 : val := 'stage2';
      [*] = ;         gc_statetag_visual_stage_3 : val := 'stage3';
      [*] = ;      end;
      [*] = ;      s := s+'switchVisual='; if val<>'' then s:=s+val else s:=s+IntToStr(switchVisual); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if s='' then s:='none';
      [*] = ;   Log(s);
      [*] = ;end;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var oldState : Integer = GetGameObjectMyPrevStatesTag;
      [*] = ;var newState : Integer = GetGameObjectMyStatesTag;
      [*] = ;
      [*] = ;if _net_debug_log_statestag then _net_Log('');
      [*] = ;
      [*] = ;var iOldAction : Integer = oldState and gc_statetag_action;
      [*] = ;var iAction : Integer = newState and gc_statetag_action;
      [*] = ;var iOldEssential : Integer = oldState and gc_statetag_essential;
      [*] = ;var iEssential : Integer = newState and gc_statetag_essential;
      [*] = ;var iOldVisual : Integer = oldState and gc_statetag_visual;
      [*] = ;var iVisual : Integer = newState and gc_statetag_visual;
      [*] = ;
      [*] = ;var switchAction, switchEssential, switchVisual : Integer;
      [*] = ;if (iOldAction<>iAction) then switchAction := iAction;
      [*] = ;if (iOldEssential<>iEssential) then switchEssential := iEssential;
      [*] = ;if (iOldVisual<>iVisual) then switchVisual := iVisual;
      [*] = ;
      [*] = ;//Log('..');
      [*] = ;//DebugLogStateTag(iEssential, iVisual);
      [*] = ;//DebugLogStateTagSwitches(switchEssential, switchVisual);
      [*] = ;
      [*] = ;case switchEssential of
      [*] = ;   gc_statetag_essential_death : begin
      [*] = ;      if (gSoundManager.bprocess) and (not gbool_gui_gatefinished) and (gObjProp[arg_obj.cid][arg_obj.id].usage<>gc_obj_usage_weakwall) then
      [*] = ;      _unit_RequestPlaySound(myHnd, gc_snd_ind_deathbld);
      [*] = ;      SetGameObjectMyPlayableObject(False);
      [*] = ;      if (not gObjProp[arg_obj.cid][arg_obj.id].bwall) then
      [*] = ;      MyGameObjectPFXClear;
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].bgate) then
      [*] = ;      _misc_OpenCloseGates(myHnd, True, false);
      [*] = ;   end;
      [*] = ;   gc_statetag_essential_none : begin
      [*] = ;      var actor : String = GetGameObjectMyLODActorName+'.mesh';
      [*] = ;      SetGameObjectActorNameByHandle(myHnd, actor);
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) then
      [*] = ;      begin
      [*] = ;         if (GetGameObjectGOHandleChildByBaseName(myHnd, gc_racename_misc, gc_basename_bui)=0) then
      [*] = ;         begin
      [*] = ;            var tmpx, tmpz : Float;
      [*] = ;            if (_misc_GetPortResourcePoint(myHnd, tmpx, tmpz, True)) then
      [*] = ;            begin
      [*] = ;               var miscplhnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;               var buiHnd : Integer = CreatePlayerGameObjectHandleByHandle(miscplhnd, gc_racename_misc, gc_basename_bui, tmpx, 0, tmpz);
      [*] = ;               GameObjectSwitchToFrameAnimationByHandle(buiHnd, 'idle', True);
      [*] = ;               SetGameObjectAnimationModeByHandle(buiHnd, 'aamLoop');
      [*] = ;               GameObjectParentToChildAbsolute(buiHnd, myHnd);
      [*] = ;               _misc_ApplyKeyColorRecursiveChild(buiHnd, gMap.players[arg_obj.pl].color);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if (GetGameObjectCountChildByHandle(myHnd)>0) and (not gObjProp[arg_obj.cid][arg_obj.id].bgate) then
      [*] = ;      begin
      [*] = ;         //var child : Integer = GetGameObjectGOHandleChildByHandle(myHnd, 0);
      [*] = ;         //SetGameObjectVisibleByHandle(child, False);
      [*] = ;         //new do not hide child set new mesh and material
      [*] = ;         var child : Integer = GetGameObjectGOHandleChildByHandle(myHnd, 0);
      [*] = ;         SetGameObjectActorNameByHandle(child, actor+'a.mesh');
      [*] = ;         var material : String = 'detail';
      [*] = ;         if GetSeasonType=2 then
      [*] = ;         material := material+'w';
      [*] = ;         // easter material
      [*] = ;         if (actor='eurmar.mesh') or (actor='rusmar.mesh') then
      [*] = ;         material := 'eurmara';
      [*] = ;         SetGameObjectMaterialNameByHandle(child, material);
      [*] = ;         SetGameObjectVisibleByHandle(child, True);
      [*] = ;      end;
      [*] = ;
      [*] = ;      var aname : String = GetGameObjectMyLODActorName+'anim';
      [*] = ;      GameObjectAddNewChild(myHnd, gc_racename_buildings, aname);
      [*] = ;      aname := GetGameObjectMyLODActorName+'anim2';
      [*] = ;      GameObjectAddNewChild(myHnd, gc_racename_buildings, aname);
      [*] = ;
      [*] = ;      arg_obj.bbuilt := True;
      [*] = ;      arg_obj.hp := gPlayer[arg_obj.pl].objBase[arg_obj.cid][arg_obj.id].maxhp;
      [*] = ;
      [*] = ;      arg_obj.buildprogress := 1;
      [*] = ;      _unit_AddObjToPlayerCounters(myHnd, True, False, False);
      [*] = ;
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_mill) then
      [*] = ;      begin
      [*] = ;         var sid : String = gc_basename_field;
      [*] = ;         var fieldid : Integer = _unit_ConvertObjSIDToID(arg_obj.cid, sid);
      [*] = ;         _unit_OrderProduce(myHnd, arg_obj.cid, fieldid, 1);
      [*] = ;      end;
      [*] = ;      if (gSoundManager.bprocess) and (not gObjProp[arg_obj.cid][arg_obj.id].bwall) and (GetPlayerIndexByHandle(GetPlayerHandleInterfaceIO)=arg_obj.pl) then
      [*] = ;      begin
      [*] = ;         var sid : String = gObjProp[arg_obj.cid][arg_obj.id].sid;
      [*] = ;         var sndname : String = 'bld'+SubStr(sid, StrLength(sid)-2, 3);
      [*] = ;         if (sid='turcen') or (sid='algcen') or (sid='turtem') or (sid='algtem') then
      [*] = ;         sndname := sndname+'tur';
      [*] = ;         _misc_PlaySound(gc_snd_lib_interface, sndname);
      [*] = ;      end;
      [*] = ;      // Show log if player InterfaceIO
      [*] = ;      if (GetGameObjectPlayerHandleByHandle(myHnd)=GetPlayerHandleInterfaceIO) then
      [*] = ;      begin
      [*] = ;         gbool_gui_update_unitcontrol := True;
      [*] = ;         var s1, s2 : String;
      [*] = ;         var sid : String;
      [*] = ;         if (gObjProp[arg_obj.cid][arg_obj.id].bwall) then
      [*] = ;         _unit_ConvertObjIDToSID(arg_obj.cid, arg_obj.id, sid)
      [*] = ;         else
      [*] = ;         sid := GetGameObjectBaseNameByHandle(myHnd);
      [*] = ;         _misc_GetUnitNameBySID(sid, s1);
      [*] = ;         _misc_ShowLogMessage(gc_logmessagesid_buildingbuilt, s1, s2, False);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;case switchVisual of
      [*] = ;   gc_statetag_visual_hide : begin
      [*] = ;      SetGameObjectVisibleByHandle(myHnd, False);
      [*] = ;      SetGameObjectCollisionDetectionByHandle(myHnd, False);
      [*] = ;
      [*] = ;      SetGameObjectMyPlayableObject(false);
      [*] = ;      SetGameObjectPickedByHandle(myHnd, False);
      [*] = ;      gSelectedObjects.Remove(myHnd);
      [*] = ;      gGuiUpdateHighlights.Add(myHnd);
      [*] = ;      _unit_ScanGridRemoveUnit(myHnd);
      [*] = ;   end;
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      if (iOldVisual=gc_statetag_visual_hide) then
      [*] = ;      begin
      [*] = ;         SetGameObjectMyPlayableObject(true);
      [*] = ;         SetGameObjectCollisionDetectionByHandle(myHnd, True);
      [*] = ;         gGuiUpdateHighlights.Add(myHnd);
      [*] = ;         _unit_ScanGridProgress(myHnd);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;var actor : String;
      [*] = ;var stage : Integer;
      [*] = ;case iEssential of
      [*] = ;   gc_statetag_essential_none : begin
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].bgate) then
      [*] = ;      begin
      [*] = ;         var cHnd : Integer;
      [*] = ;         var i : Integer;
      [*] = ;         for i:=0 to GetGameObjectCountChildByHandle(myHnd)-1 do
      [*] = ;         begin
      [*] = ;            var objHnd : Integer = GetGameObjectGOHandleChildByHandle(myHnd, i);
      [*] = ;            if GetGameObjectStringPropertyTag(objHnd)=gc_properties_stringtag_gatedoor then
      [*] = ;            begin
      [*] = ;               cHnd := objHnd;
      [*] = ;               break;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         case iVisual of
      [*] = ;            gc_statetag_visual_none : begin
      [*] = ;               _misc_OpenCloseGates(myHnd, False, true);
      [*] = ;               if (cHnd<>0) then
      [*] = ;               begin
      [*] = ;                  GameObjectResetFrameAnimationBlend(cHnd);
      [*] = ;                  GameObjectSwitchToFrameAnimationBlendByHandle(cHnd, 'close', False, False, 0, 0);
      [*] = ;                  if (switchEssential=gc_statetag_essential_none) then
      [*] = ;                  SetGameObjectAnimationModeByHandle(cHnd, 'aamNone')
      [*] = ;                  else
      [*] = ;                  SetGameObjectAnimationModeByHandle(cHnd, 'aamPlayOnce');
      [*] = ;                  SetGameObjectIntervalFactorByHandle(cHnd, 4);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            gc_statetag_visual_stage_0 : begin
      [*] = ;               _misc_OpenCloseGates(myHnd, True, true);
      [*] = ;               if (cHnd<>0) then
      [*] = ;               begin
      [*] = ;                  GameObjectResetFrameAnimationBlend(cHnd);
      [*] = ;                  GameObjectSwitchToFrameAnimationBlendByHandle(cHnd, 'open', False, False, 0, 0);
      [*] = ;                  if (switchEssential=gc_statetag_essential_none) then
      [*] = ;                  SetGameObjectAnimationModeByHandle(cHnd, 'aamNone')
      [*] = ;                  else
      [*] = ;                  SetGameObjectAnimationModeByHandle(cHnd, 'aamPlayOnce');
      [*] = ;                  SetGameObjectIntervalFactorByHandle(cHnd, 4);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   gc_statetag_essential_birth : begin
      [*] = ;      case iVisual of
      [*] = ;         gc_statetag_visual_stage_0 : stage := 1;
      [*] = ;         gc_statetag_visual_stage_1 : stage := 2;
      [*] = ;         gc_statetag_visual_stage_2 : stage := 3;
      [*] = ;         gc_statetag_visual_stage_3 : stage := 4;
      [*] = ;      end;
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].bwall) then
      [*] = ;      stage := 1+stage div 3;
      [*] = ;
      [*] = ;      actor := GetGameObjectMyLODActorName+IntToStr(stage);
      [*] = ;      SetGameObjectActorNameByHandle(myHnd, actor+'.mesh');
      [*] = ;
      [*] = ;      if (GetGameObjectCountChildByHandle(myHnd)>0) and (not gObjProp[arg_obj.cid][arg_obj.id].bgate) then
      [*] = ;      begin
      [*] = ;         var child : Integer = GetGameObjectGOHandleChildByHandle(myHnd, 0);
      [*] = ;         SetGameObjectActorNameByHandle(child, actor+'a.mesh');
      [*] = ;         SetGameObjectMaterialNameByHandle(child, 'scaffold');
      [*] = ;         SetGameObjectVisibleByHandle(child, True);
      [*] = ;      end;
      [*] = ;
      [*] = ;      // set uniform color player
      [*] = ;      _misc_ApplyKeyColorRecursiveChild(myHnd, gMap.players[arg_obj.pl].color);
      [*] = ;   end;
      [*] = ;   gc_statetag_essential_death : begin
      [*] = ;      if (not arg_obj.bdead) then
      [*] = ;      GameObjectExecuteStateByHandle(myHnd, 'OnDeath');
      [*] = ;      if (not gObjProp[arg_obj.cid][arg_obj.id].bwall) then
      [*] = ;      begin
      [*] = ;         case iVisual of
      [*] = ;            gc_statetag_visual_stage_0 : stage := 1;
      [*] = ;            gc_statetag_visual_stage_1 : stage := 2;
      [*] = ;         end;
      [*] = ;         actor := GetGameObjectMyLODActorName+'_death'+IntToStr(stage);
      [*] = ;         SetGameObjectActorNameByHandle(myHnd, actor+'.mesh');
      [*] = ;      end;
      [*] = ;
      [*] = ;      var behHnd : Integer = GetBehaviourByClassName(myHnd, 'TXProgressChildrenBehaviour');
      [*] = ;      if (behHnd<>0) then
      [*] = ;      BehaviourDestroy(behHnd);
      [*] = ;
      [*] = ;      if (GetGameObjectCountChildByHandle(myHnd)>0) and (not gObjProp[arg_obj.cid][arg_obj.id].bgate) then
      [*] = ;      begin
      [*] = ;         var child : Integer = GetGameObjectGOHandleChildByHandle(myHnd, 0);
      [*] = ;         SetGameObjectActorNameByHandle(child, actor+'a.mesh');
      [*] = ;         SetGameObjectVisibleByHandle(child, True);
      [*] = ;         SetGameObjectMaterialNameByHandle(child, 'debris');
      [*] = ;
      [*] = ;         var i : Integer;
      [*] = ;         for i:=GetGameObjectCountChildByHandle(myHnd)-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var childHnd : Integer = GetGameObjectGOHandleChildByHandle(myHnd, i);
      [*] = ;            if (GetGameObjectRaceTag(childHnd)=gc_racetag_misc) then
      [*] = ;            begin
      [*] = ;               BehaviourClear(childHnd);
      [*] = ;               EffectClear(childHnd);
      [*] = ;               GameObjectRequestToDestroyByHandle(childHnd);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         for i:=1 to GetGameObjectCountChildByHandle(myHnd)-1 do
      [*] = ;         begin
      [*] = ;            var child : Integer = GetGameObjectGOHandleChildByHandle(myHnd, i);
      [*] = ;            SetGameObjectVisibleByHandle(child, False);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      // set uniform color player
      [*] = ;      _misc_ApplyKeyColorRecursiveChild(myHnd, gMap.players[arg_obj.pl].color);
      [*] = ;
      [*] = ;      if (switchEssential=gc_statetag_essential_death) and (iVisual<>gc_statetag_visual_hide) and
      [*] = ;      ((iOldEssential=gc_statetag_essential_none) or ((iOldEssential=gc_statetag_essential_birth) and ((iOldVisual=gc_statetag_visual_stage_2) or (iOldVisual=gc_statetag_visual_stage_3)))) then
      [*] = ;      _unit_DoExplosion(myHnd);
      [*] = ;
      [*] = ;      if (iOldEssential<>gc_statetag_essential_death) and (gObjProp[arg_obj.cid][arg_obj.id].bwall) then
      [*] = ;      begin
      [*] = ;         EffectHighlightRemove(myHnd, ''); // execute before EffectClear
      [*] = ;         EffectClear(myHnd); // execute after EffectHighlightRemove
      [*] = ;         SetGameObjectOnStateDestroyByHandle(myHnd, '');
      [*] = ;         _misc_PrepareDestroyGameObject(myHnd);
      [*] = ;         //SetGameObjectColIntMaskOptionsByHandle(myHnd, True, False); // done in _misc_PrepareDestroyGameObject
      [*] = ;         SetGameObjectMyCollisionDetection(False);
      [*] = ;         SetGameObjectCollisionInertiaByHandle(myHnd, False);
      [*] = ;         SetGameObjectPlayableObjectByHandle(myHnd, False);
      [*] = ;         SetGameObjectVisibleByHandle(myHnd, False);
      [*] = ;         if (arg_obj.pl=GetPlayerIndexByHandle(GetPlayerHandleInterfaceIO)) then
      [*] = ;         gSelectedObjects.Remove(myHnd);
      [*] = ;
      [*] = ;         if (gScenario.bexists) and (gObjProp[arg_obj.cid][arg_obj.id].bwall) and (GetMapCollisionTag(GetGameObjectMyPositionX, GetGameObjectMyPositionZ, False)=gc_collisiontag_terrain)  then
      [*] = ;         begin
      [*] = ;            _unit_SetTerrainCollision(myHnd, gc_collisiontag_none);
      [*] = ;            _misc_UnitTopologyUpdate(myHnd);
      [*] = ;         end;
      [*] = ;
      [*] = ;         GameObjectMyCancelDelayExecuteState;
      [*] = ;         GameObjectMyDelayExecuteState('DelayDestroy', gc_unit_deathtime_3); // destroy on delay, to fix lan unsync if destory on tag change immidiatly, some time dont send tagstate to client.
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;case switchAction of
      [*] = ;   gc_statetag_action_none : begin
      [*] = ;      if (iEssential=gc_statetag_essential_none) and (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) then
      [*] = ;      begin
      [*] = ;         var buiHnd : Integer = GetGameObjectGOHandleChildByBaseName(myHnd, gc_racename_misc, gc_basename_bui);
      [*] = ;         if (buiHnd<>0) then
      [*] = ;         SetGameObjectVisibleByHandle(buiHnd, True);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   gc_statetag_action_build : begin
      [*] = ;      if (iEssential=gc_statetag_essential_none) and (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) then
      [*] = ;      begin
      [*] = ;         var buiHnd : Integer = GetGameObjectGOHandleChildByBaseName(myHnd, gc_racename_misc, gc_basename_bui);
      [*] = ;         if (buiHnd<>0) then
      [*] = ;         SetGameObjectVisibleByHandle(buiHnd, False);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   gc_statetag_action_attack : begin
      [*] = ;      var bProcess : Boolean = not (_net_IsClient or _net_IsReplay);
      [*] = ;      if (bProcess) then
      [*] = ;      begin
      [*] = ;         if (arg_obj.orders[0].itype=gc_obj_order_type_attackobj) or (arg_obj.orders[0].itype=gc_obj_order_type_attackpoint) then
      [*] = ;         begin
      [*] = ;            var weapind : Integer;
      [*] = ;            case _unit_GetTagStateByType(myHnd, gc_statetag_weapon) of
      [*] = ;               gc_statetag_weapon_none : weapind := 0;
      [*] = ;               gc_statetag_weapon_0 : weapind := 0;
      [*] = ;               gc_statetag_weapon_1 : weapind := 1;
      [*] = ;               gc_statetag_weapon_2 : weapind := 2;
      [*] = ;            end;
      [*] = ;            if (arg_obj.attackdelay=0) then
      [*] = ;            begin
      [*] = ;               var weaponid : Integer = gObjProp[arg_obj.cid][arg_obj.id].weapon[weapind].weaponid;
      [*] = ;               var trgHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;               if (trgHnd<>0) then
      [*] = ;               begin
      [*] = ;                  if (weaponid=0) then
      [*] = ;                  begin
      [*] = ;                     _unit_ApplyAttackPause(myHnd, weapind);
      [*] = ;                     var damage : Integer = gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id].weapon[weapind].damage;
      [*] = ;                     _misc_DoDamage(myHnd, trgHnd, damage, weapind, gObjProp[arg_obj.cid][arg_obj.id].weapon[weapind].kind);
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     var sx, sy, sz : Float;
      [*] = ;                     var tx, ty, tz : Float;
      [*] = ;                     _weapon_GetUnitShotStartPosition(myHnd, sx, sy, sz);
      [*] = ;                     _weapon_GetUnitShotEndPosition(trgHnd, tx, ty, tz);
      [*] = ;                     var bAllowShot : Boolean = _weapon_PredictShot(myHnd, weaponid, sx, sy, sz, tx, ty, tz);
      [*] = ;                     var bAllowShotPoint : Boolean = (gCustomObjPoints[arg_obj.cid][arg_obj.id].shotCount>0) and (_misc_ShotPointsAllowShoot(myHnd, sx, sz, tx, tz));
      [*] = ;                     if (bAllowShot) and (bAllowShotPoint) then
      [*] = ;                     begin
      [*] = ;                        _unit_ApplyAttackPause(myHnd, weapind);
      [*] = ;                        var flyweaponid : Integer = _weapon_GetFlyChild(weaponid);
      [*] = ;                        if (gWeapons[flyweaponid].propagation=gc_weapon_propagation_immediate) then
      [*] = ;                        begin
      [*] = ;                           var damage : Integer = gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id].weapon[weapind].damage;
      [*] = ;                           _misc_DoDamage(myHnd, trgHnd, damage, weapind, gObjProp[arg_obj.cid][arg_obj.id].weapon[weapind].kind);
      [*] = ;                        end;
      [*] = ;                        _unit_DoProjectile(myHnd, 0, False, sx, sy, sz, tx, ty, tz, weapind)
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     begin
      [*] = ;                        if (bAllowShotPoint) then
      [*] = ;                        begin
      [*] = ;                           arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                           arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           _unit_RemoveOrder(myHnd, 0);
      [*] = ;                           _unit_SetSTO(myHnd, 0);
      [*] = ;                           arg_obj.attackdelay := 0;
      [*] = ;                           arg_obj.attackmaxdelay := 0;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               if (arg_obj.orders[0].itype=gc_obj_order_type_attackpoint) then
      [*] = ;               begin
      [*] = ;                  if (weaponid<>0) then
      [*] = ;                  begin
      [*] = ;                     var sx, sy, sz : Float;
      [*] = ;                     _weapon_GetUnitShotStartPosition(myHnd, sx, sy, sz);
      [*] = ;                     var tx : Float = arg_obj.orders[0].info.dx;
      [*] = ;                     var tz : Float = arg_obj.orders[0].info.dy;
      [*] = ;                     var ty : Float;
      [*] = ;                     if (not RayCastWater(tx, tz, ty)) then // is water, then ty is water height
      [*] = ;                     ty := RayCastHeight(tx, tz);
      [*] = ;                     var bAllowShot : Boolean = _weapon_PredictShot(myHnd, weaponid, sx, sy, sz, tx, ty, tz);
      [*] = ;                     var bAllowShotPoint : Boolean = (gCustomObjPoints[arg_obj.cid][arg_obj.id].shotCount>0) and (_misc_ShotPointsAllowShoot(myHnd, sx, sz, tx, tz));
      [*] = ;                     if (bAllowShot) and (bAllowShotPoint) then
      [*] = ;                     begin
      [*] = ;                        _unit_ApplyAttackPause(myHnd, weapind);
      [*] = ;                        _unit_DoProjectile(myHnd, 0, False, sx, sy, sz, tx, ty, tz, weapind)
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     begin
      [*] = ;                        if (bAllowShotPoint) then
      [*] = ;                        begin
      [*] = ;                           arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                           arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           _unit_RemoveOrder(myHnd, 0);
      [*] = ;                           _unit_SetSTO(myHnd, 0);
      [*] = ;                           arg_obj.attackdelay := 0;
      [*] = ;                           arg_obj.attackmaxdelay := 0;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  ErrorLog('OnTagStates : trgHnd=0 and weaponid=0');
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                  arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (newState<>GetGameObjectMyStatesTag) then
      [*] = ;SetGameObjectMyStatesTag(newState);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

