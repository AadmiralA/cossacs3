section.begin
   Name = OnTagStates
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;procedure DebugLogStateTag(iEssential, iResource, iExecute, iMove, iWeapon, iAction, iVisual, iNewState : Integer);
      [*] = ;begin
      [*] = ;   var s, val : String = '';
      [*] = ;   s := 'DebugLogStateTag : ';
      [*] = ;   if (iEssential<>0) then
      [*] = ;   begin
      [*] = ;      case iEssential of
      [*] = ;         gc_statetag_essential_none : val := 'none';
      [*] = ;         gc_statetag_essential_birth : val := 'birth';
      [*] = ;         gc_statetag_essential_death : val := 'death';
      [*] = ;      end;
      [*] = ;      s := s+'iEssential='; if val<>'' then s:=s+val else s:=s+IntToStr(iEssential); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (iResource<>0) then
      [*] = ;   begin
      [*] = ;      case iResource of
      [*] = ;         gc_statetag_resource_none : val := 'none';
      [*] = ;         gc_statetag_resource_food : val := 'food';
      [*] = ;         gc_statetag_resource_wood : val := 'wood';
      [*] = ;         gc_statetag_resource_stone : val := 'stone';
      [*] = ;      end;
      [*] = ;      s := s+'iResource='; if val<>'' then s:=s+val else s:=s+IntToStr(iResource); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (iExecute<>0) then
      [*] = ;   begin
      [*] = ;      case iExecute of
      [*] = ;         gc_statetag_execute_none : val := 'none';
      [*] = ;         gc_statetag_execute_move : val := 'move';
      [*] = ;      end;
      [*] = ;      s := s+'iExecute='; if val<>'' then s:=s+val else s:=s+IntToStr(iExecute); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (iMove<>0) then
      [*] = ;   begin
      [*] = ;      case iMove of
      [*] = ;         gc_statetag_move_idle : val := 'idle';
      [*] = ;         gc_statetag_move_walk : val := 'walk';
      [*] = ;         gc_statetag_move_turn : val := 'turn';
      [*] = ;      end;
      [*] = ;      s := s+'iMove='; if val<>'' then s:=s+val else s:=s+IntToStr(iMove); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (iWeapon<>0) then
      [*] = ;   begin
      [*] = ;      case iWeapon of
      [*] = ;         gc_statetag_weapon_none : val := 'none';
      [*] = ;         gc_statetag_weapon_0 : val := 'weap0';
      [*] = ;         gc_statetag_weapon_1 : val := 'weap1';
      [*] = ;         gc_statetag_weapon_2 : val := 'weap2';
      [*] = ;      end;
      [*] = ;      s := s+'iWeapon='; if val<>'' then s:=s+val else s:=s+IntToStr(iWeapon); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (iAction<>0) then
      [*] = ;   begin
      [*] = ;      case iAction of
      [*] = ;         gc_statetag_action_none : val := 'none';
      [*] = ;         gc_statetag_action_attack : val := 'attack';
      [*] = ;         gc_statetag_action_build : val := 'build';
      [*] = ;         gc_statetag_action_extract : val := 'extract';
      [*] = ;      end;
      [*] = ;      s := s+'iAction='; if val<>'' then s:=s+val else s:=s+IntToStr(iAction); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (iVisual<>0) then
      [*] = ;   begin
      [*] = ;      case iVisual of
      [*] = ;         gc_statetag_visual_none : val := 'none';
      [*] = ;         gc_statetag_visual_hide : val := 'hide';
      [*] = ;         gc_statetag_visual_stage_0 : val := 'stage0';
      [*] = ;         gc_statetag_visual_stage_1 : val := 'stage1';
      [*] = ;         gc_statetag_visual_stage_2 : val := 'stage2';
      [*] = ;         gc_statetag_visual_stage_3 : val := 'stage3';
      [*] = ;      end;
      [*] = ;      s := s+'iVisual='; if val<>'' then s:=s+val else s:=s+IntToStr(iVisual); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (iNewState and gc_statetag_sync_endpoint=gc_statetag_sync_endpoint) then
      [*] = ;   s:=s+' sync_endpoint';
      [*] = ;   if (iNewState and gc_statetag_sync_stp=gc_statetag_sync_stp) then
      [*] = ;   s:=s+' sync_stp';
      [*] = ;   if s='' then s:='none';
      [*] = ;   Log(s);
      [*] = ;end;
      [*] = ;
      [*] = ;procedure DebugLogStateTagSwitches(hnd : Integer; switchEssential, switchResource, switchExecute, switchMove, switchWeapon, switchAction, switchVisual, iNewState : Integer);
      [*] = ;begin
      [*] = ;   var s, val : String = '';
      [*] = ;   s := 'uid='+IntToStr(GetGameObjectUniqueIdByHandle(hnd))+' pl='+IntToStr(arg_obj.pl)+' ..';
      [*] = ;   s := s+' : ';
      [*] = ;   if (switchEssential<>0) then
      [*] = ;   begin
      [*] = ;      case switchEssential of
      [*] = ;         gc_statetag_essential_none : val := 'none';
      [*] = ;         gc_statetag_essential_birth : val := 'birth';
      [*] = ;         gc_statetag_essential_death : val := 'death';
      [*] = ;      end;
      [*] = ;      s := s+'switchEssential='; if val<>'' then s:=s+val else s:=s+IntToStr(switchEssential); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (switchResource<>0) then
      [*] = ;   begin
      [*] = ;      case switchResource of
      [*] = ;         gc_statetag_resource_none : val := 'none';
      [*] = ;         gc_statetag_resource_food : val := 'food';
      [*] = ;         gc_statetag_resource_wood : val := 'wood';
      [*] = ;         gc_statetag_resource_stone : val := 'stone';
      [*] = ;      end;
      [*] = ;      s := s+'switchResource='; if val<>'' then s:=s+val else s:=s+IntToStr(switchResource); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (switchExecute<>0) then
      [*] = ;   begin
      [*] = ;      case switchExecute of
      [*] = ;         gc_statetag_execute_none : val := 'none';
      [*] = ;         gc_statetag_execute_move : val := 'move';
      [*] = ;      end;
      [*] = ;      s := s+'switchExecute='; if val<>'' then s:=s+val else s:=s+IntToStr(switchExecute); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (switchMove<>0) then
      [*] = ;   begin
      [*] = ;      case switchMove of
      [*] = ;         gc_statetag_move_idle : val := 'idle';
      [*] = ;         gc_statetag_move_walk : val := 'walk';
      [*] = ;         gc_statetag_move_turn : val := 'turn';
      [*] = ;      end;
      [*] = ;      s := s+'switchMove='; if val<>'' then s:=s+val else s:=s+IntToStr(switchMove); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (switchWeapon<>0) then
      [*] = ;   begin
      [*] = ;      case switchWeapon of
      [*] = ;         gc_statetag_weapon_none : val := 'none';
      [*] = ;         gc_statetag_weapon_0 : val := 'weap0';
      [*] = ;         gc_statetag_weapon_1 : val := 'weap1';
      [*] = ;         gc_statetag_weapon_2 : val := 'weap2';
      [*] = ;      end;
      [*] = ;      s := s+'switchWeapon='; if val<>'' then s:=s+val else s:=s+IntToStr(switchWeapon); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (switchAction<>0) then
      [*] = ;   begin
      [*] = ;      case switchAction of
      [*] = ;         gc_statetag_action_none : val := 'none';
      [*] = ;         gc_statetag_action_attack : val := 'attack';
      [*] = ;         gc_statetag_action_build : val := 'build';
      [*] = ;         gc_statetag_action_extract : val := 'extract';
      [*] = ;      end;
      [*] = ;      s := s+'switchAction='; if val<>'' then s:=s+val else s:=s+IntToStr(switchAction); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   if (switchVisual<>0) then
      [*] = ;   begin
      [*] = ;      case switchVisual of
      [*] = ;         gc_statetag_visual_none : val := 'none';
      [*] = ;         gc_statetag_visual_hide : val := 'hide';
      [*] = ;         gc_statetag_visual_stage_0 : val := 'stage0';
      [*] = ;         gc_statetag_visual_stage_1 : val := 'stage1';
      [*] = ;         gc_statetag_visual_stage_2 : val := 'stage2';
      [*] = ;         gc_statetag_visual_stage_3 : val := 'stage3';
      [*] = ;      end;
      [*] = ;      s := s+'switchVisual='; if val<>'' then s:=s+val else s:=s+IntToStr(switchVisual); s := s+' '; val := '';
      [*] = ;   end;
      [*] = ;   s:=s+' endpoint = '+BoolToStr((iNewState and gc_statetag_sync_endpoint<>0));
      [*] = ;   s:=s+' sync_stp = '+BoolToStr((iNewState and gc_statetag_sync_stp<>0));
      [*] = ;   if s='' then s:='none';
      [*] = ;   Log(s);
      [*] = ;end;
      [*] = ;
      [*] = ;procedure DoDeath(goHnd : Integer);
      [*] = ;begin
      [*] = ;   SetGameObjectMyPlayableObject(False);
      [*] = ;   SetGameObjectMyTrackPointMovementMode('mmNone');
      [*] = ;   SetGameObjectMyTargetRotatingMode('trmNone');
      [*] = ;   MyGameObjectPFXClear;
      [*] = ;   SetGameObjectMyPicked(false);
      [*] = ;   if (not _unit_IsWaterUnit(goHnd)) then
      [*] = ;   begin
      [*] = ;      SetGameObjectMyAnimationCyclesMode('acmPlayOnce');
      [*] = ;      SetGameObjectMyAnimationMode('aamPlayOnce');
      [*] = ;   end;
      [*] = ;   SetGameObjectMyAnimationControlerEnabled(False);
      [*] = ;   SetGameObjectMyOnStateStartCyclesReached('');
      [*] = ;   SetGameObjectMyOnStateEndCyclesReached('');
      [*] = ;   SetGameObjectMyOnStateFBEndCyclesReached('');
      [*] = ;   SetGameObjectMyOnStateDirectionReached('');
      [*] = ;   SetGameObjectMyOnStateEndPointReached('');
      [*] = ;   SetGameObjectMyOnStateCollectingPointReached('');
      [*] = ;   //SetGameObjectMyCollidedStateName('');
      [*] = ;   //SetGameObjectMyUncollidedStateName('');
      [*] = ;   SetGameObjectMyCollisionDetection(False);
      [*] = ;
      [*] = ;   var unittype : Integer = _misc_GetUnitType(goHnd);
      [*] = ;   if (unittype=gc_result_unittype_inf) then
      [*] = ;   begin
      [*] = ;      const cDeathRollAngle = 55;
      [*] = ;      GameObjectMyRoll(-cDeathRollAngle+random*cDeathRollAngle*2);
      [*] = ;   end;
      [*] = ;
      [*] = ;   // fix bug when sometimes unit dont declude from ai region in missions
      [*] = ;   if (CountOfAIRegions>0) then
      [*] = ;   AIRegionDoUpdateObject(0, goHnd, True);
      [*] = ;end;
      [*] = ;
      [*] = ;//var profHnd : Integer = _misc_ProfilerBeginHnd('ots');
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var oldState : Integer = GetGameObjectMyPrevStatesTag;
      [*] = ;var newState : Integer = GetGameObjectMyStatesTag;
      [*] = ;
      [*] = ;if (not _misc_IsProcessAI) then
      [*] = ;begin
      [*] = ;   var stoupd : Boolean = GetGameObjectStatesTagUpdateSTOByHandle(myHnd);
      [*] = ;   if (stoupd) then
      [*] = ;   begin
      [*] = ;      if (arg_obj.prevstouid<>0) then
      [*] = ;      begin
      [*] = ;         var prevsto : Integer = GetGameObjectHandleByUniqueId(arg_obj.prevstouid);
      [*] = ;         if (prevsto=0) then
      [*] = ;         begin
      [*] = ;            Log('OnTagStates : prevsto become invalid');
      [*] = ;            arg_obj.prevstouid := 0;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      _unit_SetClientSTO(myHnd, GetGameObjectSTOHandleByHandle(myHnd));
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_statestag then _net_Log('');
      [*] = ;
      [*] = ;var iOldResource : Integer = oldState and gc_statetag_resource;
      [*] = ;var iResource : Integer = newState and gc_statetag_resource;
      [*] = ;var iOldExecute : Integer = oldState and gc_statetag_execute;
      [*] = ;var iExecute : Integer = newState and gc_statetag_execute;
      [*] = ;var iOldMove : Integer = oldState and gc_statetag_move;
      [*] = ;var iMove : Integer = newState and gc_statetag_move;
      [*] = ;var iOldWeapon : Integer = oldState and gc_statetag_weapon;
      [*] = ;var iWeapon : Integer = newState and gc_statetag_weapon;
      [*] = ;var iOldAction : Integer = oldState and gc_statetag_action;
      [*] = ;var iAction : Integer = newState and gc_statetag_action;
      [*] = ;var iOldEssential : Integer = oldState and gc_statetag_essential;
      [*] = ;var iEssential : Integer = newState and gc_statetag_essential;
      [*] = ;var iOldVisual : Integer = oldState and gc_statetag_visual;
      [*] = ;var iVisual : Integer = newState and gc_statetag_visual;
      [*] = ;
      [*] = ;var switchResource : Integer;
      [*] = ;var switchExecute : Integer;
      [*] = ;var switchMove : Integer;
      [*] = ;var switchWeapon : Integer;
      [*] = ;var switchAction : Integer;
      [*] = ;var switchEssential : Integer;
      [*] = ;var switchVisual : Integer;
      [*] = ;
      [*] = ;if (iOldResource<>iResource) then switchResource := iResource;
      [*] = ;if (iOldExecute<>iExecute) then switchExecute := iExecute;
      [*] = ;if (iOldMove<>iMove) then switchMove := iMove;
      [*] = ;if (iOldWeapon<>iWeapon) then switchWeapon := iWeapon;
      [*] = ;if (iOldAction<>iAction) then switchAction := iAction;
      [*] = ;if (iOldEssential<>iEssential) then switchEssential := iEssential;
      [*] = ;if (iOldVisual<>iVisual) then switchVisual := iVisual;
      [*] = ;
      [*] = ;//Log('uid='+IntToStr(GetGameObjectUniqueIdByHandle(myHnd))+' pl='+IntToStr(arg_obj.pl)+' ..');
      [*] = ;//DebugLogStateTag(iEssential, iResource, iExecute, iMove, iWeapon, iAction, iVisual, 0);
      [*] = ;//DebugLogStateTagSwitches(myHnd, switchEssential, switchResource, switchExecute, switchMove, switchWeapon, switchAction, switchVisual, newState);
      [*] = ;
      [*] = ;var animidle, animwalk, anim : String;
      [*] = ;var minFrameBlend : Integer = 6;
      [*] = ;var maxFrameBlend : Integer = 8;
      [*] = ;
      [*] = ;var animinterval : Float;
      [*] = ;if (iAction=gc_statetag_action_attack) and ((gRecordGeneratorVersion=0) or (gRecordGeneratorVersion>78)) then
      [*] = ;animinterval := 0.8
      [*] = ;else
      [*] = ;animinterval := 1;
      [*] = ;
      [*] = ;//animinterval := 1;
      [*] = ;
      [*] = ;var syncstp : Boolean;
      [*] = ;
      [*] = ;var bship : Boolean = _unit_IsWaterUnit(myHnd);
      [*] = ;if (iOldVisual<>iVisual) and (not bship) then
      [*] = ;begin
      [*] = ;   if (iVisual<>gc_statetag_visual_stage_0) then
      [*] = ;   SetGameObjectMyAnimationCyclesMode('acmLoop');
      [*] = ;end;
      [*] = ;
      [*] = ;procedure DoSetupMoveAnimation(goHnd : Integer);
      [*] = ;begin
      [*] = ;   var bSkip : Boolean;
      [*] = ;   var bUseWeaponAnim : Boolean;
      [*] = ;   var bBlend : Boolean;
      [*] = ;   var animturn : String;
      [*] = ;   var trgHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;   var sndind : Integer;
      [*] = ;   if (iAction<>gc_statetag_action_none) then
      [*] = ;   begin
      [*] = ;      case iAction of
      [*] = ;         gc_statetag_action_extract : begin
      [*] = ;            if (trgHnd<>0) then
      [*] = ;            begin
      [*] = ;               bBlend := True;
      [*] = ;               var pres : Pointer = _res_GetTRes(trgHnd);
      [*] = ;               if (pres<>nil) then
      [*] = ;               case TRes(pres).itype of
      [*] = ;                  gc_resource_type_food : begin
      [*] = ;                     anim := gc_anim_workfood;
      [*] = ;                     //sndind := gc_snd_ind_workfood;
      [*] = ;                  end;
      [*] = ;                  gc_resource_type_wood : begin
      [*] = ;                     anim := gc_anim_workwood;
      [*] = ;                     sndind := gc_snd_ind_workwood;
      [*] = ;                  end;
      [*] = ;                  gc_resource_type_stone : begin
      [*] = ;                     anim := gc_anim_workstone;
      [*] = ;                     sndind := gc_snd_ind_workstone;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         gc_statetag_action_build : begin
      [*] = ;            bBlend := True;
      [*] = ;            anim := gc_anim_construct;
      [*] = ;            sndind := gc_snd_ind_workconstruct;
      [*] = ;         end;
      [*] = ;         gc_statetag_action_attack : begin
      [*] = ;            case iWeapon of
      [*] = ;               gc_statetag_weapon_none, gc_statetag_weapon_0 : anim := gc_anim_attack0;
      [*] = ;               gc_statetag_weapon_1 : anim := gc_anim_attack1;
      [*] = ;               gc_statetag_weapon_2 : anim := gc_anim_attack2;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      if (anim<>'') then
      [*] = ;      begin
      [*] = ;         var bExists : Boolean = _unit_IsAnimationExists(goHnd, anim);
      [*] = ;         if (bExists) then
      [*] = ;         begin
      [*] = ;            if (bship) then
      [*] = ;            begin
      [*] = ;               _unit_SetSTP(goHnd, GetGameObjectPositionXByHandle(goHnd), GetGameObjectPositionZByHandle(goHnd));
      [*] = ;               SetGameObjectMyTrackPointMovementMode('mmNone');
      [*] = ;               SetGameObjectMyTargetRotatingMode('trmNone');
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               if (iOldVisual<>iVisual) then
      [*] = ;               begin
      [*] = ;                  if (iVisual=gc_statetag_visual_stage_0) then
      [*] = ;                  SetGameObjectMyAnimationCyclesMode('acmPlayOnce')
      [*] = ;                  else
      [*] = ;                  SetGameObjectMyAnimationCyclesMode('acmLoop');
      [*] = ;               end;
      [*] = ;
      [*] = ;               if (bBlend) then
      [*] = ;               GameObjectMySwitchToTreeAnimationCyclesBlend(anim, False, False, False, False, 6, 8)
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  //var profHndMisc : Integer = _misc_ProfilerBeginHnd('OnTagStates_SwitchToTree');
      [*] = ;                  GameObjectMySwitchToTreeAnimationCyclesBlend(anim, False, False, False, False, 0, 0); // 0, 0 - dont use blend cause it kills animation
      [*] = ;                  //_misc_ProfilerEndHnd(profHndMisc);
      [*] = ;                  GameObjectResetFrameAnimationBlend(myHnd); // force playing animation as it should looks like
      [*] = ;               end;
      [*] = ;               if (gSoundManager.bprocess) and (sndind<>gc_snd_ind_none) then
      [*] = ;               _unit_RequestPlaySound(myHnd, sndind);
      [*] = ;            end;
      [*] = ;            bSkip := True;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   if (iResource<>gc_statetag_resource_none) then
      [*] = ;   begin
      [*] = ;      case iResource of
      [*] = ;         gc_statetag_resource_food : begin
      [*] = ;            case iMove of
      [*] = ;               gc_statetag_move_idle : animidle := gc_anim_idlefood;
      [*] = ;               gc_statetag_move_turn : animidle := gc_anim_idlefood;
      [*] = ;               gc_statetag_move_walk : animwalk := gc_anim_walkfood;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         gc_statetag_resource_wood : begin
      [*] = ;            case iMove of
      [*] = ;               gc_statetag_move_idle : animidle := gc_anim_idlewood;
      [*] = ;               gc_statetag_move_turn : animidle := gc_anim_idlewood;
      [*] = ;               gc_statetag_move_walk : animwalk := gc_anim_walkwood;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         gc_statetag_resource_stone : begin
      [*] = ;            case iMove of
      [*] = ;               gc_statetag_move_idle : animidle := gc_anim_idlestone;
      [*] = ;               gc_statetag_move_turn : animidle := gc_anim_idlestone;
      [*] = ;               gc_statetag_move_walk : animwalk := gc_anim_walkstone;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      //bUseWeaponAnim := True;
      [*] = ;      case iWeapon of
      [*] = ;         gc_statetag_weapon_none : begin
      [*] = ;            case iMove of
      [*] = ;               gc_statetag_move_idle : animidle := gc_anim_idle;
      [*] = ;               gc_statetag_move_turn : animidle := gc_anim_idle;
      [*] = ;               gc_statetag_move_walk : animwalk := gc_anim_walk;
      [*] = ;            end
      [*] = ;         end;
      [*] = ;         gc_statetag_weapon_0 : begin
      [*] = ;            case iMove of
      [*] = ;               gc_statetag_move_idle : animidle := gc_anim_idle0;
      [*] = ;               gc_statetag_move_turn : animidle := gc_anim_idle0;
      [*] = ;               gc_statetag_move_walk : animwalk := gc_anim_walk0;
      [*] = ;            end
      [*] = ;         end;
      [*] = ;         gc_statetag_weapon_1 : begin
      [*] = ;            case iMove of
      [*] = ;               gc_statetag_move_idle : animidle := gc_anim_idle1;
      [*] = ;               gc_statetag_move_turn : animidle := gc_anim_idle1;
      [*] = ;               gc_statetag_move_walk : animwalk := gc_anim_walk1;
      [*] = ;            end
      [*] = ;         end;
      [*] = ;         gc_statetag_weapon_2 : begin
      [*] = ;            case iMove of
      [*] = ;               gc_statetag_move_idle : animidle := gc_anim_idle2;
      [*] = ;               gc_statetag_move_turn : animidle := gc_anim_idle2;
      [*] = ;               gc_statetag_move_walk : animwalk := gc_anim_walk2;
      [*] = ;            end
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   if (not bSkip) then
      [*] = ;   begin
      [*] = ;      if (iMove=gc_statetag_move_turn) then
      [*] = ;      begin
      [*] = ;         if (bUseWeaponAnim) then
      [*] = ;         anim := animidle
      [*] = ;         else
      [*] = ;         case GetGameObjectMyFrameAnimationName of
      [*] = ;            gc_anim_trans10, gc_anim_trans20, gc_anim_walk0, gc_anim_prepare0, gc_anim_attack0, gc_anim_idle0 : anim := gc_anim_idle0;
      [*] = ;            gc_anim_trans01, gc_anim_trans21, gc_anim_walk1, gc_anim_prepare1, gc_anim_attack1, gc_anim_idle1 : anim := gc_anim_idle1;
      [*] = ;            gc_anim_trans02, gc_anim_trans12, gc_anim_walk2, gc_anim_prepare2, gc_anim_attack2, gc_anim_idle2 : anim := gc_anim_idle2;
      [*] = ;            else
      [*] = ;            if iResource<>gc_statetag_resource_none then
      [*] = ;            anim := animidle
      [*] = ;            else
      [*] = ;            anim := gc_anim_idle;
      [*] = ;         end;
      [*] = ;         if (not _unit_IsAnimationExists(goHnd, animidle)) then
      [*] = ;         anim := gc_anim_idle;
      [*] = ;
      [*] = ;         var bRotate : Boolean;
      [*] = ;         if (trgHnd=0) or (switchEssential=gc_statetag_essential_none) or (gObjProp[arg_obj.cid][arg_obj.id].bshotdirection) then
      [*] = ;         begin
      [*] = ;            var angle : Float = GetGameObjectSTArrowAngleByHandle(myHnd);
      [*] = ;            var dx : Float = 1;
      [*] = ;            var dy, dz : Float;
      [*] = ;            VectorRotateY(dx, dy, dz, angle);
      [*] = ;            bRotate := GameObjectMySetupRotatingToDirection(dx, dy, dz, anim, anim, anim, gObjProp[arg_obj.cid][arg_obj.id].rotatespeed*2.225, 50, GetGameObjectMyEpsilonAngle, True);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            GameObjectResetFrameAnimationBlend(myHnd);
      [*] = ;            bRotate := GameObjectMySetupRotatingToTarget(GetGameObjectPositionXByHandle(trgHnd), GetGameObjectPositionYByHandle(trgHnd), GetGameObjectPositionZByHandle(trgHnd), anim, anim, anim, gObjProp[arg_obj.cid][arg_obj.id].rotatespeed*2.225, 50, GetGameObjectMyEpsilonAngle, True);
      [*] = ;
      [*] = ;            if (bRotate) then // prevent situation, when unit play unprepare1 and i ask to shot other unit, and unit hangs
      [*] = ;            begin
      [*] = ;               //SetGameObjectMyTargetObjectByHandle(trgHnd); // crashes if target is field, and field game object removed. here is invalid handle.
      [*] = ;               //SetGameObjectMyTargetRotatingMode('trmToTargetObject');
      [*] = ;               SetGameObjectMyTargetRotatingMode('trmToTargetCoordinates');
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         if (not bRotate) then
      [*] = ;         begin
      [*] = ;            SetGameObjectMyTargetRotatingMode('trmNone');
      [*] = ;            newState := (newState and not gc_statetag_move) or gc_statetag_move_idle;
      [*] = ;            // added to prevent situation, when brotate=false but unit in some cases continue playing previous attack animation, without changing it to idle, as it should
      [*] = ;            GameObjectResetFrameAnimationBlend(myHnd);
      [*] = ;            GameObjectMySwitchToTreeAnimationCyclesBlend(anim, False, True, False, False, minFrameBlend, maxFrameBlend);
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         if (iMove=gc_statetag_move_idle) then
      [*] = ;         begin
      [*] = ;            if (bUseWeaponAnim) then
      [*] = ;            anim := animidle
      [*] = ;            else
      [*] = ;            case GetGameObjectMyFrameAnimationName of
      [*] = ;               gc_anim_trans10, gc_anim_trans20, gc_anim_walk0, gc_anim_prepare0, gc_anim_attack0, gc_anim_idle0 : anim := gc_anim_idle0;
      [*] = ;               gc_anim_trans01, gc_anim_trans21, gc_anim_walk1, gc_anim_prepare1, gc_anim_attack1, gc_anim_idle1 : anim := gc_anim_idle1;
      [*] = ;               gc_anim_trans02, gc_anim_trans12, gc_anim_walk2, gc_anim_prepare2, gc_anim_attack2, gc_anim_idle2 : anim := gc_anim_idle2;
      [*] = ;               else
      [*] = ;               if iResource<>gc_statetag_resource_none then
      [*] = ;               anim := animidle
      [*] = ;               else
      [*] = ;               anim := gc_anim_idle;
      [*] = ;            end;
      [*] = ;            if (not _unit_IsAnimationExists(goHnd, animidle)) then
      [*] = ;            anim := gc_anim_idle;
      [*] = ;
      [*] = ;            if (anim<>GetGameObjectMyFrameAnimationName) then
      [*] = ;            begin
      [*] = ;               GameObjectResetFrameAnimationBlend(myHnd);
      [*] = ;               GameObjectMySwitchToTreeAnimationCyclesBlend(anim, True, True, False, False, 0, 0); // 0, 0 - to reset blend. needed to prevent 2 shooting animation effect
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (iMove=gc_statetag_move_walk) then
      [*] = ;         begin
      [*] = ;            if (_unit_IsAnimationExists(goHnd, animwalk)) then
      [*] = ;            anim := animwalk
      [*] = ;            else
      [*] = ;            anim := gc_anim_walk;
      [*] = ;            if (anim<>GetGameObjectMyFrameAnimationName) then
      [*] = ;            begin
      [*] = ;               animinterval := gObjProp[arg_obj.cid][arg_obj.id].walkintervalfactor;
      [*] = ;               GameObjectMySwitchToTreeAnimationCyclesBlend(anim, False, True, False, False, minFrameBlend, maxFrameBlend);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   if (_net_IsClient or _net_IsReplay) then // fix bug when shooters shoot backward or random direction
      [*] = ;   begin
      [*] = ;      var dirX : Float = GetGameObjectTransformedVirtualDirectionXByHandle(myHnd);
      [*] = ;      var dirZ : Float = GetGameObjectTransformedVirtualDirectionZByHandle(myHnd);
      [*] = ;      var angle : Float = _misc_GetDirAngleToXVector(dirX, dirZ);
      [*] = ;      var sta : Float = GetGameObjectSTArrowAngleByHandle(myHnd);
      [*] = ;      if Abs(angle-sta)>GetGameObjectMyEpsilonAngle then
      [*] = ;      begin
      [*] = ;         var dx : Float = 1;
      [*] = ;         var dy, dz : Float;
      [*] = ;         VectorRotateY(dx, dy, dz, sta);
      [*] = ;         GameObjectMySetupRotatingToDirection(dx, dy, dz, anim, anim, anim, gObjProp[arg_obj.cid][arg_obj.id].rotatespeed*2.225, 50, GetGameObjectMyEpsilonAngle, True);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_peasant) and (GetGameObjectMyCountChild>0) and (anim<>'') then
      [*] = ;   _unit_ManagePeasantAttach(myHnd, anim);
      [*] = ;end;
      [*] = ;
      [*] = ;{if (switchVisual = gc_statetag_visual_hide) and (iOldVisual = gc_statetag_visual_none) then
      [*] = ;begin
      [*] = ;   var trgHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;   SetGameObjectVisibleByHandle(myHnd, False);
      [*] = ;   //SetGameObjectPositionByHandle(myHnd, GetGameObjectPositionXByHandle(trgHnd), GetGameObjectPositionYByHandle(trgHnd), GetGameObjectPositionZByHandle(trgHnd));
      [*] = ;   _misc_UnitRemoveMiniMapPrimitive(myHnd);
      [*] = ;   var pobjinside : Pointer = _misc_GetObjectArgData(trgHnd, gc_argunit_inside);
      [*] = ;   if (pobjinside<>nil) then
      [*] = ;   TIntegerList(pobjinside).Add(myHnd);
      [*] = ;end
      [*] = ;else}
      [*] = ;if (switchVisual=gc_statetag_visual_none) and (iOldVisual=gc_statetag_visual_hide) and (switchEssential<>gc_statetag_essential_death) then
      [*] = ;_misc_UnitCreateMiniMapPrimitive(myHnd, False)
      [*] = ;else
      [*] = ;if (switchEssential=gc_statetag_essential_death) then
      [*] = ;begin
      [*] = ;   if (not arg_obj.bdead) and ((not bship) or (arg_obj.bbuilt)) then
      [*] = ;   GameObjectExecuteStateByHandle(myHnd, 'OnDeath');
      [*] = ;   if (iVisual=gc_statetag_visual_hide) then // inside mine or transport
      [*] = ;   begin
      [*] = ;      DoDeath(myHnd);
      [*] = ;      SetGameObjectMyAnimationCyclesMode('acmNone');
      [*] = ;      SetGameObjectMyAnimationMode('aamNone');
      [*] = ;      SetGameObjectOnStateDestroyByHandle(myHnd, '');
      [*] = ;      GameObjectMyCancelDelayExecuteState;
      [*] = ;      GameObjectMyDelayExecuteState('DeathStage4', gc_unit_deathtime_3); // destroy on delay, to fix lan unsync if destory on tag change immidiatly, some time dont send tagstate to client.
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      if (arg_obj.bbuilt) then
      [*] = ;      begin
      [*] = ;         GameObjectMyCancelDelayExecuteState;
      [*] = ;         GameObjectMyDelayExecuteState('DeathStage1', _misc_SwitchFloat(gc_unit_deathtime_0, gc_unit_deathtime_0ship, bship));
      [*] = ;         if (bship) then
      [*] = ;         begin
      [*] = ;            DoDeath(myHnd);
      [*] = ;            if (iOldEssential=gc_statetag_essential_none) then
      [*] = ;            begin
      [*] = ;               _unit_DoExplosion(myHnd);
      [*] = ;               if (gObjProp[arg_obj.cid][arg_obj.id].usage<>gc_obj_usage_fisher) then
      [*] = ;               _unit_RequestPlaySound(myHnd, gc_snd_ind_deathship);
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            case iWeapon of
      [*] = ;               gc_statetag_weapon_none : anim := gc_anim_death;
      [*] = ;               gc_statetag_weapon_0 : anim := gc_anim_death0;
      [*] = ;               gc_statetag_weapon_1 : anim := gc_anim_death1;
      [*] = ;               gc_statetag_weapon_2 : anim := gc_anim_death2;
      [*] = ;            end;
      [*] = ;            if (iWeapon<>gc_statetag_weapon_none) and (not _unit_IsAnimationExists(myHnd, anim)) then
      [*] = ;            anim := gc_anim_death;
      [*] = ;
      [*] = ;            DoDeath(myHnd); // run animation after do death
      [*] = ;            var basename : String = GetGameObjectMyBaseName;
      [*] = ;            var bartillery : Boolean = (_misc_GetUnitType(myHnd)=gc_result_unittype_art);
      [*] = ;            if (bartillery) then
      [*] = ;            ReloadGameObjectProperties(myHnd, '', basename+'_death');
      [*] = ;            GameObjectMySwitchToTreeAnimationCyclesBlend(anim, False, False, False, False, minFrameBlend, maxFrameBlend);
      [*] = ;            if (bartillery) then
      [*] = ;            GameObjectResetFrameAnimationBlend(myHnd); // reset after switch
      [*] = ;            SetGameObjectIntervalFactorByHandle(myHnd, 0.7+random*0.6); // units die at different animation speed, to desyncronise visual part
      [*] = ;
      [*] = ;            if (gSoundManager.bprocess) then
      [*] = ;            begin
      [*] = ;               var sndind : Integer = gc_snd_ind_none;
      [*] = ;               var unittype : Integer = _misc_GetUnitType(myHnd);
      [*] = ;               case unittype of
      [*] = ;                  gc_result_unittype_inf : sndind := gc_snd_ind_deathinf;
      [*] = ;                  gc_result_unittype_cav : sndind := gc_snd_ind_deathcav;
      [*] = ;                  gc_result_unittype_art : sndind := gc_snd_ind_deathart;
      [*] = ;               end;
      [*] = ;               if (sndind<>gc_snd_ind_none) then
      [*] = ;               _unit_RequestPlaySound(myHnd, sndind);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (bship) then // not bbuilt
      [*] = ;      begin
      [*] = ;         SetGameObjectOnStateDestroyByHandle(myHnd, '');
      [*] = ;         //GameObjectRequestToDestroyByHandle(myHnd);
      [*] = ;         GameObjectMyCancelDelayExecuteState;
      [*] = ;         GameObjectMyDelayExecuteState('DeathStage4', gc_unit_deathtime_3); // destroy on delay, to fix lan unsync if destory on tag change immidiatly, some time dont send tagstate to client.
      [*] = ;         SetGameObjectVisibleByHandle(myHnd, False);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end
      [*] = ;else
      [*] = ;if (iEssential<>gc_statetag_essential_death) then
      [*] = ;begin
      [*] = ;   var forcemove: Boolean;
      [*] = ;
      [*] = ;   if (newState and gc_statetag_sync_stp=gc_statetag_sync_stp) and (_net_IsClient or _net_IsReplay) then
      [*] = ;   begin
      [*] = ;      if (GetGameObjectTrackPointMovementModeIntByHandle(myHnd)=1) then
      [*] = ;      begin
      [*] = ;         var tpCount : Integer = GetGameObjectTrackPointCountByHandle(myHnd);
      [*] = ;         if (tpCount>0) then
      [*] = ;         begin
      [*] = ;            if (arg_obj.bpathrequested) then
      [*] = ;            _unit_PathListRemove(myHnd);
      [*] = ;
      [*] = ;            var dist : Float = VectorDistance(GetGameObjectMyPositionX, 0, GetGameObjectMyPositionZ, GetGameObjectMyStateTargetPositionX, 0, GetGameObjectMyStateTargetPositionZ);
      [*] = ;            var allowdistmult : Float;
      [*] = ;            if (iAction=gc_statetag_action_build) or (iAction=gc_statetag_action_extract) then
      [*] = ;            allowdistmult := 1
      [*] = ;            else
      [*] = ;            if (iAction=gc_statetag_action_attack) then
      [*] = ;            begin
      [*] = ;               if _unit_IsMelee(myHnd) then
      [*] = ;               allowdistmult := 5
      [*] = ;               else
      [*] = ;               allowdistmult := 8;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            allowdistmult := 4;
      [*] = ;
      [*] = ;            if (dist>(gc_objectEpsilonDist*allowdistmult)) then
      [*] = ;            begin
      [*] = ;               syncstp := True;
      [*] = ;               GameObjectTrackPointClearByHandle(myHnd);
      [*] = ;               GameObjectTrackPointAddByHandle(myHnd, GetGameObjectMyStateTargetPositionX, GetGameObjectMyStateTargetPositionY, GetGameObjectMyStateTargetPositionZ);
      [*] = ;               SetGameObjectTrackPointCurrentPointIndexByHandle(myHnd, 0);
      [*] = ;            end
      [*] = ;            else
      [*] = ;            SetGameObjectTrackPointMovementModeByHandle(myHnd, 'mmNone');
      [*] = ;         end;
      [*] = ;         newState := newState and not gc_statetag_sync_stp;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (newState and gc_statetag_sync_endpoint=gc_statetag_sync_endpoint) then begin
      [*] = ;      if (_net_IsClient or _net_IsReplay) and (switchExecute<>gc_statetag_execute_move) and (not arg_obj.bpathrequested) and (GetGameObjectTrackPointMovementModeIntByHandle(myHnd)=1) then
      [*] = ;      forcemove:=true;
      [*] = ;
      [*] = ;      {if (iMove<>gc_statetag_move_turn) then
      [*] = ;      begin
      [*] = ;         //Log('im<>t,set no sync uid = '+IntToStr(GetGameObjectUniqueIdByHandle(myHnd)));
      [*] = ;         newState:=(newState and not gc_statetag_sync_endpoint);
      [*] = ;      end;}
      [*] = ;   end;
      [*] = ;
      [*] = ;   if not forcemove then begin
      [*] = ;      if (switchExecute=gc_statetag_execute_move) then
      [*] = ;      begin
      [*] = ;         newState := (newState and not gc_statetag_execute) or gc_statetag_execute_none;
      [*] = ;         newState := (newState and not gc_statetag_move) or gc_statetag_move_walk;
      [*] = ;
      [*] = ;         if _net_IsServer or _net_IsRecord then // uncomment after fix in multiplayer for proper work of replay (if it will be needed)
      [*] = ;         newState := (newState and not gc_statetag_sync_stp);
      [*] = ;
      [*] = ;         if _net_debug_log_statestag_path then _net_Log('Added to PathList');
      [*] = ;
      [*] = ;         _unit_PathListAdd(myHnd);
      [*] = ;
      [*] = ;         var trgHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;         var myPlHnd : Integer = GetGameObjectPlayerHandleByHandle(myHnd);
      [*] = ;         var trgPlHnd : Integer = GetGameObjectPlayerHandleByHandle(trgHnd);
      [*] = ;         if (trgHnd <> 0) and (trgPlHnd = myPlHnd) then
      [*] = ;         begin
      [*] = ;            var pTrgObj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;            var pTrgObjProp : Pointer = _unit_GetObjProp(trgHnd);
      [*] = ;            if (pTrgObj <> nil) and (pTrgObjProp <> nil) then
      [*] = ;            begin
      [*] = ;               if (_unit_IsBuilding(trgHnd) and TObj(pTrgObj).bbuilt) or (TObjProp(pTrgObjProp).transport > 0) then
      [*] = ;               begin
      [*] = ;                  var pObj : Pointer = _unit_GetTObj(myHnd);
      [*] = ;                  TObj(pObj).trgHnd := trgHnd;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (iAction<>0) and ((switchAction<>0)) then
      [*] = ;      begin
      [*] = ;         if (bship) then
      [*] = ;         begin
      [*] = ;            if (switchAction=gc_statetag_action_attack) then
      [*] = ;            begin
      [*] = ;               if (iMove<>0) and (switchMove<>0) then
      [*] = ;               DoSetupMoveAnimation(myHnd);
      [*] = ;               var bProcess : Boolean = not (_net_IsClient or _net_IsReplay);
      [*] = ;               if (bProcess) then
      [*] = ;               begin
      [*] = ;                  if (arg_obj.orders[0].itype=gc_obj_order_type_attackobj) or (arg_obj.orders[0].itype=gc_obj_order_type_attackpoint) then
      [*] = ;                  begin
      [*] = ;                     var weapind : Integer;
      [*] = ;                     case _unit_GetTagStateByType(myHnd, gc_statetag_weapon) of
      [*] = ;                        gc_statetag_weapon_none : weapind := 0;
      [*] = ;                        gc_statetag_weapon_0 : weapind := 0;
      [*] = ;                        gc_statetag_weapon_1 : weapind := 1;
      [*] = ;                        gc_statetag_weapon_2 : weapind := 2;
      [*] = ;                     end;
      [*] = ;                     if (arg_obj.attackdelay=0) and _unit_EnoughResForShot(myHnd, weapInd) then
      [*] = ;                     begin
      [*] = ;                        var weaponid : Integer = gObjProp[arg_obj.cid][arg_obj.id].weapon[weapind].weaponid;
      [*] = ;                        var trgHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;                        if (trgHnd<>0) then
      [*] = ;                        begin
      [*] = ;                           if (weaponid=0) then
      [*] = ;                           begin
      [*] = ;                              _unit_ApplyAttackPause(myHnd, weapind);
      [*] = ;                              var damage : Integer = gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id].weapon[weapind].damage;
      [*] = ;                              _misc_DoDamage(myHnd, trgHnd, damage, weapind, gObjProp[arg_obj.cid][arg_obj.id].weapon[weapind].kind);
      [*] = ;                           end
      [*] = ;                           else
      [*] = ;                           begin
      [*] = ;                              var sx, sy, sz : Float;
      [*] = ;                              var tx, ty, tz : Float;
      [*] = ;                              _weapon_GetUnitShotStartPosition(myHnd, sx, sy, sz);
      [*] = ;                              _weapon_GetUnitShotEndPosition(trgHnd, tx, ty, tz);
      [*] = ;                              var bAllowShot : Boolean = _weapon_PredictShot(myHnd, weaponid, sx, sy, sz, tx, ty, tz);
      [*] = ;                              var FriendOnLine : Integer;
      [*] = ;                              if (not gWeapons[weaponid].bcheckfriendonline) then
      [*] = ;                              FriendOnLine := 0
      [*] = ;                              else
      [*] = ;                              FriendOnLine := _misc_IsBuildingInRay(myHnd, sx, sy+0.5, sz, tx, ty+0.5, tz);
      [*] = ;                              if (FriendOnLine<>0) then
      [*] = ;                              begin
      [*] = ;                                 _unit_RemoveOrder(myHnd, _unit_GetCurrentOrder(myHnd));
      [*] = ;                                 _unit_SetSTO(myHnd, 0);
      [*] = ;                                 arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                                 arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;                              end
      [*] = ;                              else
      [*] = ;                              if (bAllowShot) then
      [*] = ;                              begin
      [*] = ;                                 _unit_ApplyAttackPause(myHnd, weapind);
      [*] = ;                                 _unit_ApplyWeaponCost(myHnd, weapInd);
      [*] = ;                                 var flyweaponid : Integer = _weapon_GetFlyChild(weaponid);
      [*] = ;                                 if (gWeapons[flyweaponid].propagation=gc_weapon_propagation_immediate) then
      [*] = ;                                 begin
      [*] = ;                                    var damage : Integer = gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id].weapon[weapind].damage;
      [*] = ;                                    _misc_DoDamage(myHnd, trgHnd, damage, weapind, gObjProp[arg_obj.cid][arg_obj.id].weapon[weapind].kind);
      [*] = ;                                 end;
      [*] = ;                                 _unit_DoProjectile(myHnd, 0, False, sx, sy, sz, tx, ty, tz, weapind)
      [*] = ;                              end
      [*] = ;                              else
      [*] = ;                              begin
      [*] = ;                                 arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                                 arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        if (arg_obj.orders[0].itype=gc_obj_order_type_attackpoint) then
      [*] = ;                        begin
      [*] = ;                           if (weaponid<>0) then
      [*] = ;                           begin
      [*] = ;                              var sx, sy, sz : Float;
      [*] = ;                              _weapon_GetUnitShotStartPosition(myHnd, sx, sy, sz);
      [*] = ;                              var tx : Float = arg_obj.orders[0].info.dx;
      [*] = ;                              var tz : Float = arg_obj.orders[0].info.dy;
      [*] = ;                              var ty : Float;
      [*] = ;                              if (not RayCastWater(tx, tz, ty)) then // is water, then ty is water height
      [*] = ;                              ty := RayCastHeight(tx, tz);
      [*] = ;
      [*] = ;                              var bAllowShot : Boolean = _weapon_PredictShot(myHnd, weaponid, sx, sy, sz, tx, ty, tz);
      [*] = ;                              var FriendOnLine : Integer;
      [*] = ;                              if (not gWeapons[weaponid].bcheckfriendonline) then
      [*] = ;                              FriendOnLine := 0
      [*] = ;                              else
      [*] = ;                              FriendOnLine := _misc_IsBuildingInRay(myHnd, sx, sy+0.5, sz, tx, ty+0.5, tz);
      [*] = ;                              if (FriendOnLine<>0) then
      [*] = ;                              begin
      [*] = ;                                 _unit_RemoveOrder(myHnd, _unit_GetCurrentOrder(myHnd));
      [*] = ;                                 _unit_SetSTO(myHnd, 0);
      [*] = ;                                 arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                                 arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;                              end
      [*] = ;                              else
      [*] = ;                              if (bAllowShot) then
      [*] = ;                              begin
      [*] = ;                                 _unit_ApplyAttackPause(myHnd, weapind);
      [*] = ;                                 _unit_ApplyWeaponCost(myHnd, weapInd);
      [*] = ;                                 _unit_DoProjectile(myHnd, 0, False, sx, sy, sz, tx, ty, tz, weapind);
      [*] = ;                              end
      [*] = ;                              else
      [*] = ;                              begin
      [*] = ;                                 arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                                 arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;                              end;
      [*] = ;                           end
      [*] = ;                           else
      [*] = ;                           ErrorLog('OnTagStates : trgHnd=0 and weaponid=0');
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                           arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;                  newState := (newState and not gc_statetag_action) or gc_statetag_action_none;
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            DoSetupMoveAnimation(myHnd);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         DoSetupMoveAnimation(myHnd)
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (iMove<>0) and (switchMove<>0) then
      [*] = ;      DoSetupMoveAnimation(myHnd)
      [*] = ;      else
      [*] = ;      if (iMove<>gc_statetag_move_idle) then
      [*] = ;      DoSetupMoveAnimation(myHnd)
      [*] = ;      else
      [*] = ;      if (iWeapon<>0) and (switchWeapon<>0) then
      [*] = ;      DoSetupMoveAnimation(myHnd);
      [*] = ;
      [*] = ;      if (switchVisual = gc_statetag_visual_hide) then
      [*] = ;      begin
      [*] = ;         //if forcemove then
      [*] = ;         //ErrorLog('OnTagState forcemove=True and switchVisual = gc_statetag_visual_hide, previously unit wont add to mine list that may lead to crash in rep or client');
      [*] = ;         // bHideRequested flag is needed when on high speed or after lag on client side unit goes to mine and
      [*] = ;         // first process visual_hide state tag and then calculates path - wrong sequence of actions due to this flag is corrected
      [*] = ;         if arg_obj.bPathRequested then
      [*] = ;         arg_obj.bHideRequested := true
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            SetGameObjectVisibleByHandle(myHnd, False);
      [*] = ;            SetGameObjectCollisionDetectionByHandle(myHnd, False);
      [*] = ;            var trgHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;            if (trgHnd<>0) and (_misc_GetBaseObjID(trgHnd)=gc_baseid_obj) then
      [*] = ;            begin
      [*] = ;               //SetGameObjectPositionByHandle(myHnd, GetGameObjectPositionXByHandle(trgHnd), GetGameObjectPositionYByHandle(trgHnd), GetGameObjectPositionZByHandle(trgHnd));
      [*] = ;               _misc_UnitRemoveMiniMapPrimitive(myHnd);
      [*] = ;               var pobjinside : Pointer = _misc_GetObjectArgData(trgHnd, gc_argunit_inside);
      [*] = ;               var pTrgObj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;               if (pobjinside <> nil) and (gObjProp[TObj(pTrgObj).cid][TObj(pTrgObj).id].peasantAbsorber + gObjProp[TObj(pTrgObj).cid][TObj(pTrgObj).id].transport > 0) then
      [*] = ;               begin
      [*] = ;                  arg_obj.insideofuid := GetGameObjectUniqueIdByHandle(trgHnd);
      [*] = ;                  if TIntegerList(pobjinside).IndexOf(myHnd)<0 then
      [*] = ;                  TIntegerList(pobjinside).Add(myHnd);
      [*] = ;                  if (_net_IsClient) or (_net_IsReplay) then
      [*] = ;                  _unit_ClearOrders(myHnd);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            //else
      [*] = ;            //ErrorLog('OnTagStates, unit try to hide in mine, but its sto is not right, basename='+GetGameObjectMyBaseName+' sto = '+IntToStr(trgHnd)+' x='+FloatToStr(GetGameObjectMyPositionX)+' z='+FloatToStr(GetGameObjectMyPositionZ)+' iOldVisual='+IntToStr(iOldVisual)+' iOldEssential='+IntToStr(iOldEssential)+' iNewEssential='+IntToStr(iEssential));
      [*] = ;         end;
      [*] = ;         if (iEssential=gc_statetag_essential_none) then
      [*] = ;         ErrorLog('OnTagState : something gone wrong and unit got visual_hide and essential_none. that should be impossible. and may lead to crash. I fix some things here');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (switchEssential=gc_statetag_essential_birth) then
      [*] = ;begin
      [*] = ;   SetGameObjectMyPlayableObject(false);
      [*] = ;   if (not bship) then
      [*] = ;   begin
      [*] = ;      SetGameObjectCIIntersectRadiusByHandle(myHnd, gc_collision_radius_default*0.4);
      [*] = ;      SetGameObjectCollisionInertiaByHandle(myHnd, False);
      [*] = ;   end;
      [*] = ;   //SetGameObjectCollisionDetectionByHandle(myHnd, False); // to allow raycasting with ferry collder mesh
      [*] = ;   SetGameObjectPickedByHandle(myHnd, False);
      [*] = ;   gSelectedObjects.Remove(myHnd);
      [*] = ;   gGuiUpdateHighlights.Add(myHnd);
      [*] = ;   _unit_ScanGridRemoveUnit(myHnd);
      [*] = ;   _unit_SelectedGroupRemoveUnit(myHnd);
      [*] = ;end
      [*] = ;else
      [*] = ;if (switchEssential=gc_statetag_essential_none) then
      [*] = ;begin
      [*] = ;   SetGameObjectMyPlayableObject(true);
      [*] = ;   if (not bship) then
      [*] = ;   begin
      [*] = ;      SetGameObjectCollisionInertiaByHandle(myHnd, True);
      [*] = ;      SetGameObjectCIIntersectRadiusByHandle(myHnd, gc_collision_radius_default); // change ci intersect radius AFTER collisioninertia enabled, or radius will be 0.8
      [*] = ;   end;
      [*] = ;   SetGameObjectCollisionDetectionByHandle(myHnd, True);
      [*] = ;   gGuiUpdateHighlights.Add(myHnd);
      [*] = ;   _unit_SetSTO(myHnd, 0);
      [*] = ;   if (not arg_obj.bdead) then
      [*] = ;   _unit_ScanGridProgress(myHnd)
      [*] = ;   else
      [*] = ;   ErrorLog('ACHTUNG, ontagstates : call GEC. this is epic fail. dead unit should not be in scan grid');
      [*] = ;
      [*] = ;   //ships only create childs when in none:
      [*] = ;   if (bship) then
      [*] = ;   _unit_CreateShipChilds(myHnd);
      [*] = ;end;
      [*] = ;if (switchAction=gc_statetag_action_build) then
      [*] = ;begin
      [*] = ;   SetGameObjectCIMovableByHandle(myHnd, False);
      [*] = ;   //SetGameObjectCollisionInertiaByHandle(myHnd, False);
      [*] = ;   SetGameObjectCIIntersectRadiusByHandle(myHnd, 0);
      [*] = ;end
      [*] = ;else
      [*] = ;if (iOldAction=gc_statetag_action_build) and (iAction<>gc_statetag_action_build) then
      [*] = ;begin
      [*] = ;   SetGameObjectCIMovableByHandle(myHnd, True);
      [*] = ;   //SetGameObjectCollisionInertiaByHandle(myHnd, True);
      [*] = ;   SetGameObjectCIIntersectRadiusByHandle(myHnd, gc_collision_radius_default);
      [*] = ;end;
      [*] = ;
      [*] = ;if (animinterval<>GetGameObjectIntervalFactorByHandle(myHnd)) and (switchEssential<>gc_statetag_essential_death) then
      [*] = ;SetGameObjectIntervalFactorByHandle(myHnd, animinterval);
      [*] = ;
      [*] = ;if (newState<>GetGameObjectMyStatesTag) then
      [*] = ;SetGameObjectMyStatesTagDirect(newState); // use this, instead of SetGameObjectMyStatesTag - to fix problem, when ontagstates spams with packages, and break sync_endpoint mechanics on turn
      [*] = ;
      [*] = ;if (bship) then
      [*] = ;begin
      [*] = ;   if (switchEssential<>0) then
      [*] = ;   _misc_ApplyKeyColorRecursiveChild(myHnd, gMap.players[arg_obj.pl].color);
      [*] = ;   var actor : String;
      [*] = ;   var stage : Integer;
      [*] = ;   case iEssential of
      [*] = ;      gc_statetag_essential_birth : begin
      [*] = ;         case iVisual of
      [*] = ;            gc_statetag_visual_stage_0 : stage := 1;
      [*] = ;            gc_statetag_visual_stage_1 : stage := 2;
      [*] = ;            gc_statetag_visual_stage_2 : stage := 3;
      [*] = ;            gc_statetag_visual_stage_3 : stage := 4;
      [*] = ;         end;
      [*] = ;         SetGameObjectAutoOffsetByHandle(myHnd, false);
      [*] = ;         _unit_SetShipDummyOffset(myHnd);
      [*] = ;         actor := GetGameObjectMyLODActorName+IntToStr(stage);
      [*] = ;         SetGameObjectActorNameByHandle(myHnd, actor+'.mesh');
      [*] = ;
      [*] = ;         if (GetGameObjectCountChildByHandle(myHnd)>0) then
      [*] = ;         begin
      [*] = ;            var child : Integer = GetGameObjectGOHandleChildByHandle(myHnd, 0);
      [*] = ;            SetGameObjectActorNameByHandle(child, actor+'a.mesh');
      [*] = ;            SetGameObjectVisibleByHandle(child, True);
      [*] = ;         end;
      [*] = ;         var baseName : String = GetGameObjectBaseNameByHandle(myHnd);
      [*] = ;         if SameText(baseName,'xebec') then
      [*] = ;         if (stage=1) then
      [*] = ;         SetGameObjectMaterialNameByHandle(myHnd, 'xebeca')
      [*] = ;         else
      [*] = ;         SetGameObjectMaterialNameByHandle(myHnd, 'xebec');
      [*] = ;      end;
      [*] = ;      gc_statetag_essential_death : begin
      [*] = ;         case iVisual of
      [*] = ;            gc_statetag_visual_stage_0 : stage := 1;
      [*] = ;            gc_statetag_visual_stage_1 : stage := 2;
      [*] = ;         end;
      [*] = ;         var parentactor : String = GetGameObjectMyLODActorName;
      [*] = ;         var actor : String = parentactor+'_death'+IntToStr(stage);
      [*] = ;         SetGameObjectActorNameByHandle(myHnd, actor+'.mesh');
      [*] = ;         if (GetGameObjectCountChildByHandle(myHnd)>0) then
      [*] = ;         begin
      [*] = ;            var child : Integer = GetGameObjectGOHandleChildByHandle(myHnd, 0);
      [*] = ;            SetGameObjectActorNameByHandle(child, actor+'a.mesh');
      [*] = ;            SetGameObjectVisibleByHandle(child, True);
      [*] = ;
      [*] = ;            if (parentactor='yacht') then // HARD CODE
      [*] = ;            SetGameObjectMaterialNameByHandle(child, parentactor);
      [*] = ;            //SetGameObjectMaterialNameByHandle(child, 'debris');
      [*] = ;
      [*] = ;            var i : Integer;
      [*] = ;            for i:=1 to GetGameObjectCountChildByHandle(myHnd)-1 do
      [*] = ;            begin
      [*] = ;               var child : Integer = GetGameObjectGOHandleChildByHandle(myHnd, i);
      [*] = ;               SetGameObjectVisibleByHandle(child, False);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;//_misc_ProfilerEndHnd(profHnd);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

