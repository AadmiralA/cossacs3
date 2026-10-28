section.begin
   Name = Initial
   UsedFileName = 
   Code : struct.begin
      [*] = ;{procedure AddCollisionObjects(goHnd : Integer);
      [*] = ;begin
      [*] = ;   var posX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;   var posZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;   var w : Integer = GetGameObjectCollisionMaskWidthByHandle(goHnd);
      [*] = ;   var h : Integer = GetGameObjectCollisionMaskHeightByHandle(goHnd);
      [*] = ;   var mapW : Integer = GetMapWidth;
      [*] = ;   var mapH : Integer = GetMapHeight;
      [*] = ;   var i, j : Integer;
      [*] = ;
      [*] = ;   if (GetCountOfPlayers>=gc_playerind_misc) then
      [*] = ;   begin
      [*] = ;      var plHnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;
      [*] = ;      for [MAIN]i := 0 to h-1 do
      [*] = ;      for j := 0 to w-1 do
      [*] = ;      if GetGameObjectCollisionMaskValueByHandle(goHnd, i, j) then
      [*] = ;      begin
      [*] = ;         var x, z : Float;
      [*] = ;         if ((w mod 2)=0) then
      [*] = ;         x := posX+(j-(w-1)/2)/2
      [*] = ;         else
      [*] = ;         x := posX+(j+1-w/2)/2;
      [*] = ;
      [*] = ;         if ((h mod 2)=1) then
      [*] = ;         z := posZ+(i-(h-1)/2)/2
      [*] = ;         else
      [*] = ;         z := posZ+(i-h/2)/2;
      [*] = ;
      [*] = ;         if (x<-mapW/2) or (x>mapW/2) or (z<-mapH/2) or (z>mapH/2) then
      [*] = ;         break
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            x := Floor(2*x)/2+0.25;
      [*] = ;            z := Floor(2*z)/2+.25;
      [*] = ;            var goHnd : Integer = CreatePlayerGameObjectHandleByHandle(plHnd, gc_racename_misc, 'collision', x, 0, z);
      [*] = ;            SetGameObjectCIIntersectRadiusByHandle(goHnd, 0);
      [*] = ;            SetGameObjectPositionByHandle(goHnd, x, 0, z);
      [*] = ;            var scale : Float = GetGameObjectScaleXByHandle(goHnd)*0.001;
      [*] = ;            SetGameObjectScaleByHandle(goHnd, scale, scale, 0.1*scale);
      [*] = ;            SetGameObjectCIIntersectRadiusByHandle(goHnd, 0.2);
      [*] = ;            SetGameObjectCIMassByHandle(goHnd, 1);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;}
      [*] = ;
      [*] = ;procedure AddCICollider(const x, y, z : Float);
      [*] = ;begin
      [*] = ;   var plHnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;   var colHnd : Integer;
      [*] = ;   colHnd :=  CreatePlayerGameObjectHandleByHandle(plHnd, gc_racename_misc, gc_basename_gatecollision, x, y, z);
      [*] = ;   GameObjectMakeUniqId(colHnd);
      [*] = ;   SetGameObjectStateMachineActiveByHandle(colHnd, false);
      [*] = ;   SetGameObjectCollisionInertiaByHandle(colHnd, true); //CIEnabled = True
      [*] = ;   SetGameObjectCIIntersectRadiusByHandle(colHnd, 0.35); //CIIntersectRadius = 0.35
      [*] = ;   SetGameObjectCIMovableByHandle(colHnd, false); //CIMovable = False
      [*] = ;   SetGameObjectCIMassByHandle(colHnd, 10000); //CIMass = 10000
      [*] = ;   SetGameObjectCIMaxDistKoefByHandle(colHnd, 2); //CIMaxDistKoef = 2
      [*] = ;   SetGameObjectCIDeltaStepByHandle(colHnd, 0.005); //CIDeltaStep = 0.005
      [*] = ;   SetGameObjectCIRotationSpeedByHandle(colHnd, 5); //CIRotationSpeed = 5
      [*] = ;   SetGameObjectCIStuckAngleByHandle(colHnd, 5); //CIStuckAngle = 5
      [*] = ;   SetGameObjectCIEpsilonAngleByHandle(colHnd, 4); //CIEpsilonAngle = 4
      [*] = ;   SetGameObjectCIEpsilonShiftByHandle(colHnd, 0.001); //CIEpsilonShift = 0.001
      [*] = ;   SetGameObjectCIEpsilonMoveByHandle(colHnd, 0.02); //CIEpsilonMove = 0.02
      [*] = ;   SetGameObjectCIDistExtPointEpsilonByHandle(colHnd, 0.005); //CIDistExtPointEpsilon = 0.005
      [*] = ;   SetGameObjectCIAvoidPointMaxAngleByHandle(colHnd, 120); //CIAvoidPointMaxAngle = 120
      [*] = ;   SetGameObjectCIMaxCollideCounterByHandle(colHnd, 7); //CIMaxCollideCounter = 7
      [*] = ;end;
      [*] = ;
      [*] = ;procedure SetupCollider(goHnd : Integer; rad : Float);
      [*] = ;begin
      [*] = ;   SetGameObjectCollisionDetectionByHandle(goHnd, False);
      [*] = ;   SetGameObjectCollisionInertiaByHandle(goHnd, True); //CIEnabled = True
      [*] = ;   SetGameObjectCIMovableByHandle(goHnd, False); //CIMovable = False
      [*] = ;   SetGameObjectCIMassByHandle(goHnd, 10000); //CIMass = 10000
      [*] = ;   SetGameObjectCIMaxDistKoefByHandle(goHnd, 2); //CIMaxDistKoef = 2
      [*] = ;   SetGameObjectCIDeltaStepByHandle(goHnd, 0.005); //CIDeltaStep = 0.005
      [*] = ;   SetGameObjectCIRotationSpeedByHandle(goHnd, 5); //CIRotationSpeed = 5
      [*] = ;   SetGameObjectCIStuckAngleByHandle(goHnd, 5); //CIStuckAngle = 5
      [*] = ;   SetGameObjectCIEpsilonAngleByHandle(goHnd, 4); //CIEpsilonAngle = 4
      [*] = ;   SetGameObjectCIEpsilonShiftByHandle(goHnd, 0.001); //CIEpsilonShift = 0.001
      [*] = ;   SetGameObjectCIEpsilonMoveByHandle(goHnd, 0.02); //CIEpsilonMove = 0.02
      [*] = ;   SetGameObjectCIDistExtPointEpsilonByHandle(goHnd, 0.005); //CIDistExtPointEpsilon = 0.005
      [*] = ;   SetGameObjectCIAvoidPointMaxAngleByHandle(goHnd, 120); //CIAvoidPointMaxAngle = 120
      [*] = ;   SetGameObjectCIMaxCollideCounterByHandle(goHnd, 7); //CIMaxCollideCounter = 7
      [*] = ;
      [*] = ;   SetGameObjectCIIntersectRadiusByHandle(goHnd, rad); //CIIntersectRadius = 0.35
      [*] = ;   SetGameObjectCollisionDetectionByHandle(goHnd, True);
      [*] = ;end;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;if (GetPlayerRaceTagByHandle(GetGameObjectPlayerHandleByHandle(myHnd))=gc_racetag_misc) then
      [*] = ;begin
      [*] = ;   SetGameObjectMyPlayableObject(False);
      [*] = ;   SetGameObjectMyCollisionDetection(False);
      [*] = ;   SetGameObjectMyStateMachineActive(False);
      [*] = ;   var basename : String = GetGameObjectMyBaseName;
      [*] = ;   var plInd : Integer = GetPlayerIndexByHandle(GetPlayerHandleInterfaceIO);
      [*] = ;   var cid : Integer;
      [*] = ;   cid := gint_unit_parentcid;
      [*] = ;   if (cid=-1) or (not _country_IsCountryMember(cid, basename)) then
      [*] = ;   begin
      [*] = ;      var newcid : Integer = _unit_FindMemberNearestCountryID(basename);
      [*] = ;      if (newcid=-1) then
      [*] = ;      cid := 0
      [*] = ;      else
      [*] = ;      cid := newcid;
      [*] = ;   end;
      [*] = ;   var id : Integer = _unit_ConvertObjSIDToID(cid, basename);
      [*] = ;   if (cid>=0) and (id>0) then
      [*] = ;   begin
      [*] = ;      if (gCustomObjPoints[cid, id].decal.bexists) then
      [*] = ;      begin
      [*] = ;         var decalname : String = basename+'d';
      [*] = ;         var offx : Float = gCustomObjPoints[cid, id].decal.offx;
      [*] = ;         var offz : Float = gCustomObjPoints[cid, id].decal.offz;
      [*] = ;         var scale : Float = gCustomObjPoints[cid, id].decal.scale;
      [*] = ;         var angle : Float = gCustomObjPoints[cid, id].decal.angle;
      [*] = ;         _unit_AddDecoration(myHnd, decalname, offx, offz, scale, angle);
      [*] = ;      end;
      [*] = ;      SetGameObjectTagFloatByHandle(myHnd, cid*256+id);
      [*] = ;   end;
      [*] = ;
      [*] = ;   var aname : String = GetGameObjectMyLODActorName+'anim';
      [*] = ;   if (aname<>'') then
      [*] = ;   GameObjectAddNewChild(myHnd, gc_racename_buildings, aname);
      [*] = ;   aname := GetGameObjectMyLODActorName+'anim2';
      [*] = ;   if (aname<>'') then
      [*] = ;   GameObjectAddNewChild(myHnd, gc_racename_buildings, aname);
      [*] = ;
      [*] = ;   // set uniform color player
      [*] = ;   _misc_ApplyKeyColorRecursiveChild(myHnd, gMap.players[plInd].color);
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   args arg_obj : TObj;
      [*] = ;   args arg_stolist : TIntegerList;
      [*] = ;   args arg_attlist : TIntegerList;
      [*] = ;   args arg_ordlist : TIntegerList;
      [*] = ;   args arg_inside : TIntegerList;
      [*] = ;
      [*] = ;   SetGameObjectOnStateDestroyByHandle(myHnd, 'OnDestroy');
      [*] = ;
      [*] = ;   if (gSerialMode=0) and (GetGameObjectUniqueIdByHandle(myHnd)=0) then
      [*] = ;   begin
      [*] = ;      GameObjectMakeUniqId(myHnd);
      [*] = ;
      [*] = ;      //SetGameObjectOnStateDestroyByHandle(myHnd, 'OnDestroy');
      [*] = ;      SetGameObjectMyStateTargetPosition(GetGameObjectMyPositionX, GetGameObjectMyPositionY, GetGameObjectMyPositionZ);
      [*] = ;      GameObjectMyTrackPointClear;
      [*] = ;      SetGameObjectMyTrackPointMovementMode('mmNone');
      [*] = ;      SetGameObjectMyTargetRotatingMode('trmNone');
      [*] = ;      _unit_SetSTO(myHnd, 0);
      [*] = ;      SetGameObjectMyPlayableObject(True);
      [*] = ;      SetGameObjectMyCollisionDetection(True);
      [*] = ;
      [*] = ;      SetGameObjectMyStatesTag(0);
      [*] = ;      SetGameObjectMyStatesTagForceUpdate;
      [*] = ;      ResetGameObjectPrevStatesTagByHandle(myHnd);
      [*] = ;
      [*] = ;      var basename : String = GetGameObjectMyBaseName;
      [*] = ;      var plInd : Integer = GetPlayerIndexByHandle(GetGameObjectPlayerHandleByHandle(myHnd));
      [*] = ;      var cid : Integer;
      [*] = ;      if (gint_unit_parentcid=-1) then
      [*] = ;      cid := gPlayer[plInd].cid
      [*] = ;      else
      [*] = ;      cid := gint_unit_parentcid;
      [*] = ;      var tmp : String = SubStr(basename, 4, 3);
      [*] = ;      if (tmp='swa') or (tmp='wwa') or (tmp='sga') or (tmp='wga') then
      [*] = ;      basename := SubStr(basename, 1, 6);
      [*] = ;      if (cid=-1) or (not _country_IsCountryMember(cid, basename)) then
      [*] = ;      begin
      [*] = ;         var newcid : Integer = _unit_FindMemberNearestCountryID(basename);
      [*] = ;         if (cid=-1) then
      [*] = ;         ErrorLog('unit='+basename+' cid=-1')
      [*] = ;         else
      [*] = ;         ErrorLog('unit='+basename+' not a country member. function set nearest cid='+IntToStr(newcid));
      [*] = ;         if (newcid=-1) then
      [*] = ;         cid := 0
      [*] = ;         else
      [*] = ;         cid := newcid;
      [*] = ;      end;
      [*] = ;
      [*] = ;      _unit_InitObj(myHnd, cid, basename, arg_obj);
      [*] = ;      gint_unit_parentcid := -1;
      [*] = ;
      [*] = ;      // изменения счетчиков игрока
      [*] = ;      _unit_AddObjToPlayerCounters(myHnd, False, False, False);
      [*] = ;      gGuiUpdateHighlights.Add(myHnd);
      [*] = ;
      [*] = ;      if (gCustomObjPoints[cid, arg_obj.id].decal.bexists) then
      [*] = ;      begin
      [*] = ;         var decalname : String = basename+'d';
      [*] = ;         var offx : Float = gCustomObjPoints[cid, arg_obj.id].decal.offx;
      [*] = ;         var offz : Float = gCustomObjPoints[cid, arg_obj.id].decal.offz;
      [*] = ;         var scale : Float = gCustomObjPoints[cid, arg_obj.id].decal.scale;
      [*] = ;         var angle : Float = gCustomObjPoints[cid, arg_obj.id].decal.angle;
      [*] = ;         _unit_AddDecoration(myHnd, decalname, offx, offz, scale, angle);
      [*] = ;      end;
      [*] = ;
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].bcansetrally) then
      [*] = ;      begin
      [*] = ;         var exitCount : Integer = gCustomObjPoints[arg_obj.cid, arg_obj.id].exitCount;
      [*] = ;         if exitCount > 0 then
      [*] = ;         begin
      [*] = ;            arg_obj.rallyx := GetGameObjectMyPositionX + gCustomObjPoints[arg_obj.cid, arg_obj.id].exitPoints[exitCount-1].x;
      [*] = ;            arg_obj.rallyy := GetGameObjectMyPositionZ + gCustomObjPoints[arg_obj.cid, arg_obj.id].exitPoints[exitCount-1].z;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            var tempx, tempz : Float;
      [*] = ;            if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) and (_misc_GetPortResourcePoint(myHnd, tempx, tempz, True)) then
      [*] = ;            begin
      [*] = ;               arg_obj.rallyx := tempx;
      [*] = ;               arg_obj.rallyy := tempz;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               arg_obj.rallyx := GetGameObjectMyPositionX;
      [*] = ;               arg_obj.rallyy := GetGameObjectMyPositionZ;
      [*] = ;               ErrorLog('No start point for creating unit! : hnd='+IntToStr(myHnd)+' x='+FloatToStr(GetGameObjectMyPositionX)+' z='+FloatToStr(GetGameObjectMyPositionZ));
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         arg_obj.rallytmpx := arg_obj.rallyx;
      [*] = ;         arg_obj.rallytmpy := arg_obj.rallyy;
      [*] = ;      end;
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_port) then
      [*] = ;      begin
      [*] = ;         var tmpx, tmpz : Float;
      [*] = ;         if (_misc_GetPortResourcePoint(myHnd, tmpx, tmpz, True)) then
      [*] = ;         begin
      [*] = ;            var miscplhnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;            var buiHnd : Integer = CreatePlayerGameObjectHandleByHandle(miscplhnd, gc_racename_misc, gc_basename_bui, tmpx, 0, tmpz);
      [*] = ;            GameObjectSwitchToFrameAnimationByHandle(buiHnd, 'idle', True);
      [*] = ;            SetGameObjectAnimationModeByHandle(buiHnd, 'aamLoop');
      [*] = ;            GameObjectParentToChildAbsolute(buiHnd, myHnd);
      [*] = ;            _misc_ApplyKeyColorRecursiveChild(buiHnd, gMap.players[arg_obj.pl].color);
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].bgate) then
      [*] = ;      begin
      [*] = ;         var x1, z1, x2, z2 : Float;
      [*] = ;         var addCollision : Boolean;
      [*] = ;         basename := GetGameObjectMyBaseName;
      [*] = ;         arg_obj.wallvariation := _misc_GetWallVariationByName(basename);
      [*] = ;         if (StrPos('_14', basename)>0) then
      [*] = ;         begin
      [*] = ;            x1 := 1.06+0.3;
      [*] = ;            z1 := 0.05;
      [*] = ;            x2 := -1.06-0.3;
      [*] = ;            z2 := 0.02;
      [*] = ;            addCollision := True;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (StrPos('_16', basename)>0) then
      [*] = ;         begin
      [*] = ;            x1 := 0.67+0.2;
      [*] = ;            z1 := -0.8;
      [*] = ;            x2 := -0.39-0.4;
      [*] = ;            z2 := 0.79;
      [*] = ;            addCollision := True;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (StrPos('_17', basename)>0) then
      [*] = ;         begin
      [*] = ;            x1 := 0.73;
      [*] = ;            z1 := 0.76;
      [*] = ;            x2 := -0.88;
      [*] = ;            z2 := -0.65;
      [*] = ;            addCollision := True;
      [*] = ;         end;
      [*] = ;         if addCollision then
      [*] = ;         begin
      [*] = ;            //AddCICollider(GetGameObjectMyPositionX+x1, GetGameObjectMyPositionY, GetGameObjectMyPositionZ+z1);
      [*] = ;            //AddCICollider(GetGameObjectMyPositionX+x2, GetGameObjectMyPositionY, GetGameObjectMyPositionZ+z2);
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (gObjProp[arg_obj.cid][arg_obj.id].bgate) then
      [*] = ;         begin
      [*] = ;            _misc_OpenCloseGates(myHnd, False, true);
      [*] = ;            //gate doors:
      [*] = ;            var baseName : String = GetGameObjectMyBaseName;
      [*] = ;            var childHnd : Integer;
      [*] = ;            function AddGate(const goHnd : Integer; const doorname : String; const roll : Float) : Integer;
      [*] = ;            begin
      [*] = ;               var plHnd : Integer = GetGameObjectPlayerHandleByHandle(goHnd);
      [*] = ;               var px, py, pz : Float;
      [*] = ;               var sx, sy, sz : Float;
      [*] = ;               px := GetGameObjectPositionXByHandle(goHnd);
      [*] = ;               py := GetGameObjectPositionYByHandle(goHnd);
      [*] = ;               pz := GetGameObjectPositionZByHandle(goHnd);
      [*] = ;               sx := GetGameObjectScaleXByHandle(goHnd);
      [*] = ;               sy := GetGameObjectScaleYByHandle(goHnd);
      [*] = ;               sz := GetGameObjectScaleZByHandle(goHnd);
      [*] = ;               Result := CreatePlayerGameObjectHandleByHandle(plHnd, gc_racename_buildings, doorname, px, py, pz);
      [*] = ;               GameObjectRollByHandle(Result, roll);
      [*] = ;               GameObjectParentToChildAbsolute(Result, myHnd);
      [*] = ;               SetGameObjectScaleByHandle(Result, 1, 1, 1);
      [*] = ;            end;
      [*] = ;            if StrPos('eur', baseName)>0 then
      [*] = ;            begin
      [*] = ;               if (StrPos('14', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'eurdoor', 90-45)
      [*] = ;               else
      [*] = ;               if (StrPos('15', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'eurdoor', 0-45)
      [*] = ;               else
      [*] = ;               if (StrPos('16', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'eurdoor', 45-45)
      [*] = ;               else
      [*] = ;               if (StrPos('17', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'eurdoor', 135-45);
      [*] = ;            end
      [*] = ;            else
      [*] = ;            if StrPos('tur', baseName)>0 then
      [*] = ;            begin
      [*] = ;               if (StrPos('14', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'turdoor', 90)
      [*] = ;               else
      [*] = ;               if (StrPos('15', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'turdoor', 0)
      [*] = ;               else
      [*] = ;               if (StrPos('16', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'turdoor', 45)
      [*] = ;               else
      [*] = ;               if (StrPos('17', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'turdoor', 135);
      [*] = ;            end
      [*] = ;            else
      [*] = ;            if StrPos('rus', baseName)>0 then
      [*] = ;            begin
      [*] = ;               if (StrPos('14', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'rusdoor', 90)
      [*] = ;               else
      [*] = ;               if (StrPos('15', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'rusdoor', 0)
      [*] = ;               else
      [*] = ;               if (StrPos('16', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'rusdoor', 45)
      [*] = ;               else
      [*] = ;               if (StrPos('17', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'rusdoor', 135);
      [*] = ;            end
      [*] = ;            else
      [*] = ;            if StrPos('ukr', baseName)>0 then
      [*] = ;            begin
      [*] = ;               if (StrPos('14', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'ukrdoor', 90-45)
      [*] = ;               else
      [*] = ;               if (StrPos('15', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'ukrdoor', 0-45)
      [*] = ;               else
      [*] = ;               if (StrPos('16', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'ukrdoor', 45-45)
      [*] = ;               else
      [*] = ;               if (StrPos('17', baseName)>0) then
      [*] = ;               AddGate(myHnd, 'ukrdoor', 135-45);
      [*] = ;            end;
      [*] = ;         end
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (gObjProp[arg_obj.cid][arg_obj.id].bwall) then
      [*] = ;      begin
      [*] = ;         basename := GetGameObjectMyBaseName;
      [*] = ;         arg_obj.wallvariation := _misc_GetWallVariationByName(basename);
      [*] = ;         var pos : Integer = StrPos('_', basename);
      [*] = ;         var ind : Integer = StrToInt(SubStr(basename, pos+1, 2));
      [*] = ;         var rad : Float;
      [*] = ;         if (pos>0) then
      [*] = ;         begin
      [*] = ;            case ind of
      [*] = ;               3, 4 : rad := 0.699;
      [*] = ;               else
      [*] = ;               {1, 2, 5, 6, 7, 8, 9, 10, 11, 12, 13 : }rad := 0.499;
      [*] = ;            end;
      [*] = ;            if (rad>0) then
      [*] = ;            SetupCollider(myHnd, rad);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;arg_obj.lasttimecheckcapture := random*gc_unit_TimeCheckCapture;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

