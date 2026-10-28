section.begin
   Name = Initial
   UsedFileName = 
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;args arg_stolist : TIntegerList;
      [*] = ;args arg_attlist : TIntegerList;
      [*] = ;args arg_ordlist : TIntegerList;
      [*] = ;args arg_inside : TIntegerList;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var basename : String = GetGameObjectMyBaseName;
      [*] = ;
      [*] = ;SetGameObjectMyOnStateEndFrameReached('');
      [*] = ;SetGameObjectOnStateDestroyByHandle(myHnd, 'OnDestroy');
      [*] = ;
      [*] = ;SetGameObjectMinFrameBlendByHandle(myHnd, 6);
      [*] = ;SetGameObjectMaxFrameBlendByHandle(myHnd, 8);
      [*] = ;SetGameObjectDeferredFrameBlendByHandle(myHnd, false);
      [*] = ;
      [*] = ;procedure SetCustomCollisionInertia(hnd : Integer; IntersectRadiusFactor, MassFactor, DeltaStepFactor : Float; bMovable : Boolean);
      [*] = ;begin
      [*] = ;   SetGameObjectCollisionInertiaByHandle(hnd, IntersectRadiusFactor>0);
      [*] = ;   if (IntersectRadiusFactor<=0) then
      [*] = ;   exit;
      [*] = ;   SetGameObjectCIAvoidPointMaxAngleByHandle(hnd, 180);
      [*] = ;   SetGameObjectCIMassByHandle(hnd, 1*massFactor);
      [*] = ;   SetGameObjectCIIntersectRadiusByHandle(hnd, gc_collision_radius_default*IntersectRadiusFactor);
      [*] = ;   SetGameObjectCIMaxDistKoefByHandle(hnd, 2);
      [*] = ;   if MassFactor<=0 then
      [*] = ;   SetGameObjectCIDeltaStepByHandle(hnd, 0)
      [*] = ;   else
      [*] = ;   SetGameObjectCIDeltaStepByHandle(hnd, 0.005/DeltaStepFactor);
      [*] = ;   SetGameObjectCIRotationSpeedByHandle(hnd, 5);
      [*] = ;   SetGameObjectCIStuckAngleByHandle(hnd, 0); // 0.5 in rw2to
      [*] = ;   SetGameObjectCIEpsilonAngleByHandle(hnd, 4);
      [*] = ;   SetGameObjectCIEpsilonShiftByHandle(hnd, 0.001);
      [*] = ;   SetGameObjectCIEpsilonMoveByHandle(hnd, 0.02);
      [*] = ;   SetGameObjectCIDistExtPointEpsilonByHandle(hnd, 0.005);
      [*] = ;   //misc
      [*] = ;   SetGameObjectCIMovableByHandle(hnd, bMovable);
      [*] = ;   SetGameObjectCIBuildExtPointsByHandle(hnd, True);
      [*] = ;   SetGameObjectCIMaxCollideCounterByHandle(hnd, 7*3);
      [*] = ;   SetGameObjectCIMaxProcessObjectByHandle(hnd, 4*3);
      [*] = ;   var minx, miny, minz, maxx, maxy, maxz : Float;
      [*] = ;   GetGameObjectCustomBoundingAABBByHandle(hnd, minx, miny, minz, maxx, maxy, maxz);
      [*] = ;   if (minx>(-gc_collision_radius_default*IntersectRadiusFactor)) then
      [*] = ;   minx := -gc_collision_radius_default*IntersectRadiusFactor;
      [*] = ;   if (maxx<(gc_collision_radius_default*IntersectRadiusFactor)) then
      [*] = ;   maxx := gc_collision_radius_default*IntersectRadiusFactor;
      [*] = ;   if (minz>(-gc_collision_radius_default*IntersectRadiusFactor)) then
      [*] = ;   minz := -gc_collision_radius_default*IntersectRadiusFactor;
      [*] = ;   if (maxz<(gc_collision_radius_default*IntersectRadiusFactor)) then
      [*] = ;   maxz := gc_collision_radius_default*IntersectRadiusFactor;
      [*] = ;   SetGameObjectCustomBoundingAABBByHandle(hnd, minx, miny, minz, maxx, maxy, maxz);
      [*] = ;end;
      [*] = ;
      [*] = ;procedure UseParentTLFBehaviour(const parentHnd : Integer);
      [*] = ;begin
      [*] = ;   var bHnd : Integer = BehaviourCreate(parentHnd, 'TXTLFAnimationBehaviour', true, false);
      [*] = ;   SetBehaviourIntProperty(bHnd, 'CleanObjects', 0);
      [*] = ;   SetBehaviourIntProperty(bHnd, 'SetTLFObject', parentHnd);
      [*] = ;   var i : Integer;
      [*] = ;   for i := 0 to GetGameObjectCountChildByHandle(parentHnd)-1 do
      [*] = ;   begin
      [*] = ;      var cHnd : Integer = GetGameObjectGOHandleChildByHandle(parentHnd, i);
      [*] = ;      SetGameObjectUseIdentityMatrix(cHnd, false);
      [*] = ;      SetBehaviourIntProperty(bHnd, 'AddObject', cHnd);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;procedure InitRollBehaviour;
      [*] = ;begin
      [*] = ;   case basename of
      [*] = ;      'cannon', 'howitzer', 'mortar', 'multicannon', 'framegun' : begin
      [*] = ;         //SetGameObjectMyTrackPointTurnStepInterval(20);
      [*] = ;         //SetGameObjectMyTrackPointTurnStep(1);
      [*] = ;         SetGameObjectMyTrackPointMoveStep(0.01);
      [*] = ;         var pxl, pxr, py, py2, pz, radius : Float;
      [*] = ;         var wheelcount : Integer = 2;
      [*] = ;         case basename of
      [*] = ;            'cannon' : begin
      [*] = ;               radius := 0.34;
      [*] = ;               pxl := 0.68;
      [*] = ;               pxr := -pxl;
      [*] = ;               py := -0.0; //-0.35
      [*] = ;               pz := 0.675;
      [*] = ;            end;
      [*] = ;            'multicannon' : begin
      [*] = ;               radius := 0.34;
      [*] = ;               pxl := 0.718;
      [*] = ;               pxr := -pxl;
      [*] = ;               py := 0;//-0.5725;
      [*] = ;               pz := 0.675;
      [*] = ;            end;
      [*] = ;            'howitzer' : begin
      [*] = ;               radius := 0.34;
      [*] = ;               pxl := 0.6515;
      [*] = ;               pxr:=-pxl;
      [*] = ;               py := 0;//-0.5845;
      [*] = ;               pz := 0.675;
      [*] = ;            end;
      [*] = ;            'mortar' : begin
      [*] = ;               radius := 0.078;
      [*] = ;               pxl := 0.599;
      [*] = ;               pxr:=-pxl;
      [*] = ;               py := -0.93;
      [*] = ;               py2 := 0.93;
      [*] = ;               pz := 0.16;
      [*] = ;               wheelcount := 4;
      [*] = ;            end;
      [*] = ;            'framegun' : begin
      [*] = ;               radius := 0.34;
      [*] = ;               pxl := 0.68;
      [*] = ;               pxr := -pxl;
      [*] = ;               py := -0.0; //-0.35
      [*] = ;               pz := 0.675;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         pz:=radius*2;
      [*] = ;
      [*] = ;         var bhHnd : Integer;
      [*] = ;         var i : Integer;
      [*] = ;         for i:=0 to (wheelcount div 2)-1 do
      [*] = ;         begin
      [*] = ;            var suffix : String;
      [*] = ;            if (i=1) then
      [*] = ;            begin
      [*] = ;               suffix := '_rear';
      [*] = ;               py := py2;
      [*] = ;               //radius:=0.01;
      [*] = ;            end;
      [*] = ;            var chHnd : Integer = GetGameObjectGOHandleChildByCustomName(myHnd, basename+'_wheel_left'+suffix);
      [*] = ;            if (chHnd<>0) then
      [*] = ;            begin
      [*] = ;               bhHnd := BehaviourCreate(chHnd, 'TXRollBehaviour', True, False);
      [*] = ;               SetBehaviourFloatProperty(bhHnd, 'Radius', radius);
      [*] = ;               SetBehaviourAffineVectorProperty(bhHnd, 'MoveDir', 0, 1, 0);
      [*] = ;               SetBehaviourAffineVectorProperty(bhHnd, 'RollDir', 1, 0, 0);
      [*] = ;               SetGameObjectPositionByHandle(chHnd, pxl, py, pz);
      [*] = ;            end;
      [*] = ;
      [*] = ;            chHnd := GetGameObjectGOHandleChildByCustomName(myHnd, basename+'_wheel_right'+suffix);
      [*] = ;            if (chHnd<>0) then
      [*] = ;            begin
      [*] = ;               bhHnd := BehaviourCreate(chHnd, 'TXRollBehaviour', True, False);
      [*] = ;               SetBehaviourFloatProperty(bhHnd, 'Radius', radius);
      [*] = ;               SetBehaviourAffineVectorProperty(bhHnd, 'MoveDir', 0, 1, 0);
      [*] = ;               SetBehaviourAffineVectorProperty(bhHnd, 'RollDir', 1, 0, 0);
      [*] = ;               SetGameObjectPositionByHandle(chHnd, pxr, py, pz);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (gSerialMode=0) and (GetGameObjectUniqueIdByHandle(myHnd)=0) then
      [*] = ;begin
      [*] = ;   GameObjectMakeUniqId(myHnd);
      [*] = ;
      [*] = ;   SetGameObjectMyOnStateEndPointReached('OnEndPointReached');
      [*] = ;   SetGameObjectMyOnStateDirectionReached('OnDirectionReached');
      [*] = ;   SetGameObjectMyOnStateEndCyclesReached(''); // now works in .acl execute state trigger, which execute state OnAclAnimationReached
      [*] = ;   //SetGameObjectMyOnStateEndFrameReached('');
      [*] = ;   //SetGameObjectOnStateDestroyByHandle(myHnd, 'OnDestroy');
      [*] = ;   SetGameObjectTrackPointOnStatePresetSwitchReached(myHnd, '');
      [*] = ;
      [*] = ;   SetGameObjectMyStateTargetPosition(GetGameObjectMyPositionX, GetGameObjectMyPositionY, GetGameObjectMyPositionZ);
      [*] = ;   GameObjectMyTrackPointClear;
      [*] = ;   SetGameObjectMyTrackPointMovementMode('mmNone');
      [*] = ;   SetGameObjectMyTargetRotatingMode('trmNone');
      [*] = ;   SetGameObjectMyAnimationControlerEnabled(true);
      [*] = ;   SetGameObjectMyAnimationCyclesMode('acmLoop');
      [*] = ;   SetGameObjectMyAnimationMode('aamLoop');
      [*] = ;   SetGameObjectMyOnStateDistanceToPoint('', 0);
      [*] = ;   SetGameObjectMyTrackPointUseAnimationCycles(true);
      [*] = ;   _unit_SetSTO(myHnd, 0);
      [*] = ;   SetGameObjectMyPlayableObject(False); // will be enabled in OnTagStates
      [*] = ;   SetGameObjectMyCollisionDetection(True);
      [*] = ;   SetGameObjectCollisionInertiaByHandle(myHnd, True);
      [*] = ;   SetGameObjectMyTrackPointAnimationNames(gc_anim_walk, gc_anim_idle);
      [*] = ;   //SetGameObjectMinFrameBlendByHandle(myHnd, 6);
      [*] = ;   //SetGameObjectMaxFrameBlendByHandle(myHnd, 8);
      [*] = ;   //SetGameObjectDeferredFrameBlendByHandle(myHnd, false);
      [*] = ;
      [*] = ;   SetGameObjectMyStatesTag(gc_statetag_essential_none or gc_statetag_resource_none or gc_statetag_move_idle or gc_statetag_weapon_none or gc_statetag_action_none or gc_statetag_execute_none or gc_statetag_visual_none);
      [*] = ;   SetGameObjectMyStatesTagForceUpdate;
      [*] = ;   ResetGameObjectPrevStatesTagByHandle(myHnd);
      [*] = ;
      [*] = ;   GameObjectMySwitchToTreeAnimationCycles(gc_anim_idle, false, false, false, false);
      [*] = ;   SetGameObjectMyCurrentFrame(GetGameObjectStartFrameByHandle(myHnd)+floor(Random*(GetGameObjectEndFrameByHandle(myHnd)-GetGameObjectStartFrameByHandle(myHnd))));
      [*] = ;
      [*] = ;   var plHnd : Integer = GetGameObjectPlayerHandleByHandle(myHnd);
      [*] = ;   var plInd : Integer = GetPlayerIndexByHandle(plHnd);
      [*] = ;   SetGameObjectPickedByHandle(myHnd, False);
      [*] = ;
      [*] = ;   // определение нации, которой принадлежит новопостроенный юнит. Нация соответсвует нации объекта, который построил юнита.
      [*] = ;
      [*] = ;   var bIsShipDummy : Boolean;
      [*] = ;   if (gbool_unit_isshipdummy) then
      [*] = ;   begin
      [*] = ;      bIsShipDummy := True;
      [*] = ;      gbool_unit_isshipdummy := False;
      [*] = ;   end;
      [*] = ;
      [*] = ;   var cid : Integer;
      [*] = ;   if (gint_unit_parentcid=-1) then
      [*] = ;   cid := gPlayer[plInd].cid
      [*] = ;   else
      [*] = ;   cid := gint_unit_parentcid;
      [*] = ;   if (GetProjectModeInt=gc_project_mode_editor) and ((cid=-1) or (not _country_IsCountryMember(cid, basename))) then
      [*] = ;   begin
      [*] = ;      var newcid : Integer = _unit_FindMemberNearestCountryID(basename);
      [*] = ;      if (cid=-1) then
      [*] = ;      ErrorLog('unit='+basename+' cid=-1')
      [*] = ;      else
      [*] = ;      ErrorLog('unit='+basename+' not a country member. function set nearest cid='+IntToStr(newcid));
      [*] = ;      if (newcid=-1) then
      [*] = ;      cid := 0
      [*] = ;      else
      [*] = ;      cid := newcid;
      [*] = ;   end;
      [*] = ;
      [*] = ;   _unit_InitObj(myHnd, cid, basename, arg_obj);
      [*] = ;   if (bIsShipDummy) then
      [*] = ;   arg_obj.bbuilt := False;
      [*] = ;   gint_unit_parentcid := -1;
      [*] = ;
      [*] = ;   // enabling speed upgrades for all the units
      [*] = ;   var speed : Float = gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id].speed;
      [*] = ;   if (speed>0) and (speed<>1) then
      [*] = ;   begin
      [*] = ;      var movestepinterval : Float = GetGameObjectMyTrackPointMoveStepInterval;
      [*] = ;      SetGameObjectMyTrackPointMoveStepInterval(Floor(movestepinterval*speed));
      [*] = ;      var turnstepinterval : Float = GetGameObjectMyTrackPointTurnStepInterval;
      [*] = ;      SetGameObjectMyTrackPointTurnStepInterval(Floor(turnstepinterval*speed));
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (_unit_IsWaterUnit(myHnd)) then
      [*] = ;   begin
      [*] = ;      case basename of
      [*] = ;         'ferry' : SetCustomCollisionInertia(myHnd, 11, 32, 0.1, not bIsShipDummy);
      [*] = ;         'battleship' : SetCustomCollisionInertia(myHnd, 11, 32, 0.1, not bIsShipDummy);
      [*] = ;         'frigate', 'xebec' : SetCustomCollisionInertia(myHnd, 9, 16, 0.1, not bIsShipDummy);
      [*] = ;         'galley' : SetCustomCollisionInertia(myHnd, 7, 12, 0.1, not bIsShipDummy);
      [*] = ;         'chaika' : SetCustomCollisionInertia(myHnd, 6, 8, 0.1, not bIsShipDummy);
      [*] = ;         'yacht', 'yachttur' : SetCustomCollisionInertia(myHnd, 6, 8, 0.1, not bIsShipDummy);
      [*] = ;         'fishboat' : SetCustomCollisionInertia(myHnd, 3, 1, 0.1, not bIsShipDummy);
      [*] = ;         {'ferry' : SetCustomCollisionInertia(myHnd, 14, 32, 0.1);
      [*] = ;         'battleship' : SetCustomCollisionInertia(myHnd, 14, 32, 0.1);
      [*] = ;         'frigate', 'xebec' : SetCustomCollisionInertia(myHnd, 11, 16, 0.1);
      [*] = ;         'galley' : SetCustomCollisionInertia(myHnd, 9, 12, 0.1);
      [*] = ;         'chaika' : SetCustomCollisionInertia(myHnd, 8, 8, 0.1);
      [*] = ;         'yacht', 'yachttur' : SetCustomCollisionInertia(myHnd, 8, 8, 0.1);
      [*] = ;         'fishboat' : SetCustomCollisionInertia(myHnd, 4, 1, 0.1);}
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   if (not bIsShipDummy) then
      [*] = ;   begin
      [*] = ;      _unit_AddObjToPlayerCounters(myHnd, True, False, False);
      [*] = ;      _unit_FullClearOrders(myHnd);
      [*] = ;
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].bshowdelay) then
      [*] = ;      arg_obj.attackmaxdelay := 1;
      [*] = ;
      [*] = ;      // для крестьян, прячем все аттачи инструментов
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_peasant) and (GetGameObjectMyCountChild>0) then
      [*] = ;      begin
      [*] = ;         var childHnd : Integer = GetGameObjectGOHandleChildByHandle(myHnd, 0);
      [*] = ;         SetGameObjectVisibleByHandle(childHnd, False);
      [*] = ;      end;
      [*] = ;
      [*] = ;      // создание колес артиллерии, живущих на behaviour'е вращения
      [*] = ;      InitRollBehaviour;
      [*] = ;
      [*] = ;      SetGameObjectMyTrackPointSkipPoints(true);
      [*] = ;
      [*] = ;      var quadTree : Integer;
      [*] = ;      case gObjProp[arg_obj.cid][arg_obj.id].media of
      [*] = ;         gc_obj_media_land : quadTree := TopologyGetPathQuadTree;
      [*] = ;         gc_obj_media_water : begin
      [*] = ;            quadTree := TopologyGetTopologyQuadTree;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      SetGameObjectMyTrackPointSkipQuadTree(quadTree);
      [*] = ;      SetGameObjectMyTrackPointSkipFactor(1);
      [*] = ;      SetGameObjectMyTrackPointSkipEpsilon(0.1);
      [*] = ;
      [*] = ;      gGuiUpdateHighlights.Add(myHnd);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (gObjProp[arg_obj.cid][arg_obj.id].media<>gc_obj_media_water) then
      [*] = ;begin
      [*] = ;   // raycast with collision mesh like ferry, gates, bridges
      [*] = ;   SetGameObjectBVTestEnabledByHandle(myHnd, True);
      [*] = ;   SetGameObjectBVTestShpereRadiusByHandle(myHnd, 0.0125*4);
      [*] = ;end;
      [*] = ;
      [*] = ;if (gObjProp[arg_obj.cid][arg_obj.id].media<>gc_obj_media_water) then
      [*] = ;SetGameObjectCollisionPriorityByHandle(myHnd, 100000000);
      [*] = ;
      [*] = ;if not GetGameObjectMyCollisionExecAsFunc then
      [*] = ;   SetGameObjectMyCollisionExecAsFunc(true);
      [*] = ;SetGameObjectMyRuleCollidedExecFr(4, 35.0, False); // no collide event with friendly
      [*] = ;SetGameObjectMyRuleCollidedExecEn(2, 90.0, False); // collide event with enemy in front for 90 fov
      [*] = ;SetGameObjectMyRuleCollidedExecNl(4, 35.0, False); // no collide event with neutral
      [*] = ;if gbool_use_collision then begin
      [*] = ;   if GetGameObjectMyCollidedStateName='' then
      [*] = ;      SetGameObjectMyCollidedStateName('_misc_Collided');
      [*] = ;   if GetGameObjectMyUncollidedStateName<>'' then
      [*] = ;      SetGameObjectMyUncollidedStateName(''); // leave empty
      [*] = ;end else begin
      [*] = ;   if GetGameObjectMyCollidedStateName<>'' then
      [*] = ;      SetGameObjectMyCollidedStateName('');
      [*] = ;   if GetGameObjectMyUncollidedStateName<>'' then
      [*] = ;      SetGameObjectMyUncollidedStateName('');
      [*] = ;end;
      [*] = ;
      [*] = ;arg_obj.lasttimecheckcapture := random*gc_unit_TimeCheckCapture;
      [*] = ;arg_obj.lasttimeidlegrid := random*gc_unit_TimeIdleGrid;
      [*] = ;arg_obj.lasttimescangrid := random*gc_unit_TimeScanGrid;
      [*] = ;arg_obj.lasttimetopology := random*gc_unit_TimeTopology;
      [*] = ;arg_obj.lasttimebestposition := random*gc_unit_TimeFindBestPosition;
      [*] = ;arg_obj.lastsearchenemy := random;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

