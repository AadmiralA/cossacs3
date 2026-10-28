section.begin
   Name = Nothing
   UsedFileName = 
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;args arg_stolist : TIntegerList;
      [*] = ;args arg_attlist : TIntegerList;
      [*] = ;args arg_ordlist : TIntegerList;
      [*] = ;args arg_inside : TIntegerList;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var progresstick : Integer = arg_obj.progresstick;
      [*] = ;const cProgressRallyPointTick = 31;
      [*] = ;const cProgressCheckCapture = 19;
      [*] = ;const cProgressSmokeIncrease = 7;
      [*] = ;
      [*] = ;if ((_net_IsClient and (IsLanReady)) or _net_IsReplay) then
      [*] = ;begin
      [*] = ;   if (not arg_obj.bdead) then
      [*] = ;   begin
      [*] = ;      if (arg_obj.bbuilt) then
      [*] = ;      begin
      [*] = ;         var pobjprop : Pointer = gObjProp[arg_obj.cid][arg_obj.id];
      [*] = ;         //_unit_ControlBuildProgress(myHnd);
      [*] = ;
      [*] = ;         var lastprogresstime : Float = arg_obj.lastprogresstime;
      [*] = ;         var deltatime : Float = GetGameTime-lastprogresstime;
      [*] = ;         if (arg_obj.attackdelay>0) then
      [*] = ;         arg_obj.attackdelay := _misc_MaxFloat(arg_obj.attackdelay-deltatime, 0);
      [*] = ;
      [*] = ;         const cSmokeHPLimit = 0.85;
      [*] = ;         var maxhp : Integer = gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id].maxhp;
      [*] = ;         var smokeHPMaxIntense : Integer;
      [*] = ;         if (TObjProp(pobjprop).bslowdeath) then
      [*] = ;         smokeHPMaxIntense := Max(gc_BuildingSlowDeathHP, maxhp div 4)
      [*] = ;         else
      [*] = ;         smokeHPMaxIntense := maxhp div 4;
      [*] = ;         if (arg_obj.smokecount>0) or ((arg_obj.hp-smokeHPMaxIntense)<((maxhp-smokeHPMaxIntense)*cSmokeHPLimit)) then
      [*] = ;         begin
      [*] = ;            var smokeCount : Integer = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokeCount;
      [*] = ;            if (smokeCount>0) then
      [*] = ;            begin
      [*] = ;               var maxind : Integer = floor((1-((arg_obj.hp-smokeHPMaxIntense)/cSmokeHPLimit)/((maxhp-smokeHPMaxIntense)))*smokeCount);
      [*] = ;               if (maxind<0) then
      [*] = ;               maxind := -1;
      [*] = ;
      [*] = ;               var behHnd : Integer = GetBehaviourByClassName(myHnd, 'TXProgressChildrenBehaviour');
      [*] = ;               if (maxind>=0) and (behHnd=0) then
      [*] = ;               BehaviourCreateWithKey(myHnd, 'TXProgressChildrenBehaviour', 'prgchild', False)
      [*] = ;               else
      [*] = ;               if (maxind<0) and (behHnd<>0) and (arg_obj.smokecount<=1) then
      [*] = ;               BehaviourDestroy(behHnd);
      [*] = ;
      [*] = ;               var lastind : Integer = arg_obj.smokecount-1;
      [*] = ;               if (maxind<>lastind) then
      [*] = ;               begin
      [*] = ;                  var plHnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;                  if (maxind>lastind) and ((progresstick mod cProgressSmokeIncrease)=0) then
      [*] = ;                  begin
      [*] = ;                     var diff : Integer = maxind-arg_obj.smokecount+1;
      [*] = ;                     var freq : Float = MinFloat((1-0.5*diff/smokeCount)*0.8, 0.8);
      [*] = ;                     if (random>freq) then
      [*] = ;                     begin
      [*] = ;                        var i : Integer;
      [*] = ;                        for i:=arg_obj.smokecount to maxind do
      [*] = ;                        begin
      [*] = ;                           var x : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].x;
      [*] = ;                           var y : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].y;
      [*] = ;                           var z : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].z;
      [*] = ;                           var nx : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].nx;
      [*] = ;                           var ny : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].ny;
      [*] = ;                           var nz : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].nz;
      [*] = ;                           var px, py, pz : Float;
      [*] = ;                           GetGameObjectAbsolutePositionByHandle(myHnd, px, py, pz);
      [*] = ;                           var effHnd : Integer = CreatePlayerGameObjectHandleByHandle(plHnd, gc_racename_misc, 'point', px+x, py+y, pz+z);
      [*] = ;                           SetGameObjectOrientationByHandle(effHnd, nx, ny, nz, 0, 1, 0);
      [*] = ;                           GameObjectParentToChildAbsolute(effHnd, myHnd);
      [*] = ;                           gEffectLibrary.GetOrCreate('buildingsmoke', effHnd);
      [*] = ;                           break;
      [*] = ;                        end;
      [*] = ;                        arg_obj.smokecount := arg_obj.smokecount+1;
      [*] = ;                     end;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  if (maxind<lastind) and (random>0.5) then
      [*] = ;                  begin
      [*] = ;                     var deltaind : Integer = lastind-maxind;
      [*] = ;                     if (deltaind>0) then
      [*] = ;                     begin
      [*] = ;                        var curCount : Integer;
      [*] = ;                        var i : Integer;
      [*] = ;                        for i:=0 to GetGameObjectCountChildByHandle(myHnd)-1 do
      [*] = ;                        begin
      [*] = ;                           var childHnd : Integer = GetGameObjectGOHandleChildByHandle(myHnd, i);
      [*] = ;                           if (GetGameObjectRaceTag(childHnd)=gc_racetag_misc) then
      [*] = ;                           curCount := curCount+1;
      [*] = ;                        end;
      [*] = ;                        var newdeltaind : Integer = (curCount-1)-maxind;
      [*] = ;                        for i:=GetGameObjectCountChildByHandle(myHnd)-1 downto 0 do
      [*] = ;                        begin
      [*] = ;                           if (newdeltaind<=0) then
      [*] = ;                           break;
      [*] = ;                           var childHnd : Integer = GetGameObjectGOHandleChildByHandle(myHnd, i);
      [*] = ;                           if (GetGameObjectRaceTag(childHnd)=gc_racetag_misc) then
      [*] = ;                           begin
      [*] = ;                              BehaviourClear(childHnd);
      [*] = ;                              EffectClear(childHnd);
      [*] = ;                              GameObjectRequestToDestroyByHandle(childHnd);
      [*] = ;                              newdeltaind := newdeltaind-1;
      [*] = ;                              break;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                     arg_obj.smokecount := arg_obj.smokecount-1;
      [*] = ;                  end;
      [*] = ;                  //arg_obj.smokecount := maxind+1;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (TObjProp(pobjprop).bslowdeath) and (arg_obj.hp<=gc_BuildingSlowDeathHP) and (arg_obj.hp>10) then
      [*] = ;         begin
      [*] = ;            arg_obj.hp := arg_obj.hp-(floor(deltatime*gc_BuildingSlowDeathSpeed)+1);
      [*] = ;            if (arg_obj.hp<10) then
      [*] = ;            arg_obj.hp := 10;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (arg_obj.orders[0].itype<>gc_obj_order_type_none) then
      [*] = ;         ExecuteState('DoProgressOrders');
      [*] = ;
      [*] = ;         if (arg_obj.topZone < 0) and (not TObjProp(pobjprop).bWall) then
      [*] = ;         _unit_TopologyAdd(myHnd);
      [*] = ;      end;
      [*] = ;
      [*] = ;      arg_obj.lastprogresstime := GetGameTime;
      [*] = ;      arg_obj.progresstick := progresstick+1;
      [*] = ;   end;
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   function GetSpiralStepsByRadius(const radius : Integer) : Integer;
      [*] = ;   begin
      [*] = ;      Result := (radius*2+1)*(radius*2+1)+3+(radius-1)*4;
      [*] = ;   end;
      [*] = ;
      [*] = ;   function ProcessSpiralIdleGridSearch(const rx, ry : Float; const maxcount, minind, maxind, radius : Integer; var x, y : Integer) : Boolean;
      [*] = ;   begin
      [*] = ;      var hmapx : Integer = (GetMapWidth div 2);
      [*] = ;      var hmapy : Integer = (GetMapHeight div 2);
      [*] = ;      Result := False;
      [*] = ;      var px, py : Integer;
      [*] = ;      _misc_PosToIdleGridIndices(rx, ry, px, py);
      [*] = ;      x := px;
      [*] = ;      y := py;
      [*] = ;      var tmpind : Integer = 1;
      [*] = ;      var i, rad : Integer;
      [*] = ;      for [MAIN]rad:=1 to radius do
      [*] = ;      begin
      [*] = ;         for i:=-rad to rad do
      [*] = ;         begin
      [*] = ;            if (tmpind>=minind) and (tmpind<maxind) then
      [*] = ;            begin
      [*] = ;               x := px-rad;
      [*] = ;               y := py+i;
      [*] = ;               if (x>=0) and (x<gc_idlegrid_countx) and (y>=0) and (y<gc_idlegrid_county) and (gIdleGrid[x, y]<maxcount) and (GetMapCollisionTag(x*gc_idlegrid_size-hmapx, y*gc_idlegrid_size-hmapy, True)=0) then
      [*] = ;               begin
      [*] = ;                  Result := True;
      [*] = ;                  break(MAIN);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            tmpind := tmpind+1;
      [*] = ;            if (tmpind>=minind) and (tmpind<maxind) then
      [*] = ;            begin
      [*] = ;               x := px+rad;
      [*] = ;               y := py+i;
      [*] = ;               if (x>=0) and (x<gc_idlegrid_countx) and (y>=0) and (y<gc_idlegrid_county) and (gIdleGrid[x, y]<maxcount) and (GetMapCollisionTag(x*gc_idlegrid_size-hmapx, y*gc_idlegrid_size-hmapy, True)=0) then
      [*] = ;               begin
      [*] = ;                  Result := True;
      [*] = ;                  break(MAIN);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            tmpind := tmpind+1;
      [*] = ;         end;
      [*] = ;         for i:=-rad to rad do
      [*] = ;         begin
      [*] = ;            if (tmpind>=minind) and (tmpind<maxind) then
      [*] = ;            begin
      [*] = ;               x := px+i;
      [*] = ;               y := py-rad;
      [*] = ;               if (x>=0) and (x<gc_idlegrid_countx) and (y>=0) and (y<gc_idlegrid_county) and (gIdleGrid[x, y]<maxcount) and (GetMapCollisionTag(x*gc_idlegrid_size-hmapx, y*gc_idlegrid_size-hmapy, True)=0) then
      [*] = ;               begin
      [*] = ;                  Result := True;
      [*] = ;                  break(MAIN);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            tmpind := tmpind+1;
      [*] = ;            if (tmpind>=minind) and (tmpind<maxind) then
      [*] = ;            begin
      [*] = ;               x := px+i;
      [*] = ;               y := py+rad;
      [*] = ;               if (x>=0) and (x<gc_idlegrid_countx) and (y>=0) and (y<gc_idlegrid_county) and (gIdleGrid[x, y]<maxcount) and (GetMapCollisionTag(x*gc_idlegrid_size-hmapx, y*gc_idlegrid_size-hmapy, True)=0) then
      [*] = ;               begin
      [*] = ;                  Result := True;
      [*] = ;                  break(MAIN);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            tmpind := tmpind+1;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   function FindBestPosition(goHnd : Integer; var px, pz : Float) : Boolean;
      [*] = ;   begin
      [*] = ;      Result := False;
      [*] = ;      var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;      if (pobj<>nil) then
      [*] = ;      begin
      [*] = ;         const maxdensityinrally = 1;//7;
      [*] = ;         const maxdensityingrid = 1;
      [*] = ;         const cUnitColRadius = 0.32;
      [*] = ;         var rx : Float = TObj(pobj).rallyx;
      [*] = ;         var ry : Float = TObj(pobj).rallyy;
      [*] = ;         var bOnCollision : Boolean = (GetMapCollisionTag(rx, ry, True)>0);
      [*] = ;         gTmpIntList.Clear;
      [*] = ;         //_misc_MakeListObjectsInRadius(rx, ry, 1, true, (1 shl gc_obj_material_body), 0, True, list);
      [*] = ;         _misc_MakeListObjectsInRadiusOptimized(rx, ry, 1, true, (1 shl gc_obj_material_body), 0, gTmpIntList);
      [*] = ;         var count : Integer = gTmpIntList.GetCount;
      [*] = ;         if (count>maxdensityinrally) or (bOnCollision) then
      [*] = ;         begin
      [*] = ;            var i : Integer;
      [*] = ;            var standcount : Integer;
      [*] = ;            for i:=0 to count-1 do
      [*] = ;            begin
      [*] = ;               var trgHnd : Integer = gTmpIntList.Get(i);
      [*] = ;               var ptrgobj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;               if (ptrgobj<>nil) and (TObj(ptrgobj).standtime>=3) then
      [*] = ;               begin
      [*] = ;                  standcount := standcount+1;
      [*] = ;                  if (standcount>maxdensityinrally) then
      [*] = ;                  break;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            if (standcount>maxdensityinrally) or (bOnCollision) then
      [*] = ;            begin
      [*] = ;               var minx, maxx, miny, maxy, x, y : Integer;
      [*] = ;               var radius : Integer = 14;
      [*] = ;               const cSmallRadius = 2;
      [*] = ;               var minind : Integer = 1+floor(random*8);
      [*] = ;               var maxind : Integer = GetSpiralStepsByRadius(cSmallRadius)-1;
      [*] = ;               if (not ProcessSpiralIdleGridSearch(rx, ry, maxdensityingrid, minind, maxind, cSmallRadius, x, y)) then
      [*] = ;               _misc_GetPositionOnLine(rx, ry, GetGameObjectPositionXByHandle(goHnd), GetGameObjectPositionZByHandle(goHnd), gc_idlegrid_size*cSmallRadius, rx, ry);
      [*] = ;               maxind := GetSpiralStepsByRadius(radius)-1;
      [*] = ;               if (ProcessSpiralIdleGridSearch(rx, ry, maxdensityingrid, minind, maxind, radius, x, y)) then
      [*] = ;               begin
      [*] = ;                  px := (x*gc_idlegrid_size)-(GetMapWidth div 2)+random*gc_idlegrid_size;
      [*] = ;                  pz := (y*gc_idlegrid_size)-(GetMapHeight div 2)+random*gc_idlegrid_size;
      [*] = ;                  if (GetMapCollisionTag(px, pz, True)=0) then
      [*] = ;                  begin
      [*] = ;                     Result := True;
      [*] = ;                     gbool_gui_update_rallypoints := True;
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  var rnddir : Float = random*360;
      [*] = ;                  var vecy : Float;
      [*] = ;                  px := radius;
      [*] = ;                  pz := 0;
      [*] = ;                  VectorRotateY(px, vecy, pz, rnddir);
      [*] = ;                  px := rx+px+random*gc_idlegrid_size;
      [*] = ;                  pz := ry+pz+random*gc_idlegrid_size;
      [*] = ;                  if (GetMapCollisionTag(px, pz, True)=0) then
      [*] = ;                  begin
      [*] = ;                     Result := True;
      [*] = ;                     gbool_gui_update_rallypoints := True;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            px := rx+(0.5-random);
      [*] = ;            pz := ry+(0.5-random);
      [*] = ;            if (GetMapCollisionTag(px, pz, True)=0) then
      [*] = ;            begin
      [*] = ;               Result := True;
      [*] = ;               gbool_gui_update_rallypoints := True;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (not arg_obj.bdead) then
      [*] = ;   begin
      [*] = ;      // Обновления прогресса строительства недостроенного здания (изменение стадий постройки и пересчет progress'а). Регистрация завершенного здания в счетчиках (число построенных здания, количество жилых мест, мельница садит пшеницу, выдача сообщения игроку, что здание построено.
      [*] = ;      var gametime : Float = GetGameTime;
      [*] = ;      var gametimewithrnd : Float = gametime+arg_obj.uniqrnd;
      [*] = ;      if (not arg_obj.bbuilt) then
      [*] = ;      begin
      [*] = ;         if gametime>arg_obj.lasttimecheckcapture then
      [*] = ;         begin
      [*] = ;            arg_obj.lasttimecheckcapture := gametimewithrnd+gc_unit_TimeCheckCapture;
      [*] = ;            _misc_CheckCapture(myHnd);
      [*] = ;         end;
      [*] = ;
      [*] = ;         _unit_ControlBuildProgress(myHnd);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         var pobjprop : Pointer = gObjProp[arg_obj.cid][arg_obj.id];
      [*] = ;         if (TObjProp(pobjprop).bcapture) then
      [*] = ;         begin
      [*] = ;            if gametime>arg_obj.lasttimecheckcapture then
      [*] = ;            begin
      [*] = ;               arg_obj.lasttimecheckcapture := gametimewithrnd+gc_unit_TimeCheckCapture;
      [*] = ;               case gMap.settings.additional.capture of
      [*] = ;                  gc_mapsettings_capture_default, gc_mapsettings_capture_nopeasants : _misc_CheckCapture(myHnd);
      [*] = ;                  gc_mapsettings_capture_nocenterspeasants : begin
      [*] = ;                     if (TObjProp(pobjprop).usage<>gc_obj_usage_center) then
      [*] = ;                     _misc_CheckCapture(myHnd);
      [*] = ;                  end;
      [*] = ;                  gc_mapsettings_capture_onlyartillery : begin
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         var lastprogresstime : Float = arg_obj.lastprogresstime;
      [*] = ;         var deltatime : Float = GetGameTime-lastprogresstime;
      [*] = ;         if (arg_obj.attackdelay>0) then
      [*] = ;         arg_obj.attackdelay := _misc_MaxFloat(arg_obj.attackdelay-deltatime, 0);
      [*] = ;
      [*] = ;         if (arg_obj.exitDelay>0) then
      [*] = ;         arg_obj.exitDelay := _misc_MaxFloat(arg_obj.exitDelay-deltatime, 0);
      [*] = ;
      [*] = ;         const cSmokeHPLimit = 0.85;
      [*] = ;         var maxhp : Integer = gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id].maxhp;
      [*] = ;         var smokeHPMaxIntense : Integer;
      [*] = ;         if (TObjProp(pobjprop).bslowdeath) then
      [*] = ;         smokeHPMaxIntense := Max(gc_BuildingSlowDeathHP, maxhp div 4)
      [*] = ;         else
      [*] = ;         smokeHPMaxIntense := maxhp div 4;
      [*] = ;         if (arg_obj.smokecount>0) or ((arg_obj.hp-smokeHPMaxIntense)<((maxhp-smokeHPMaxIntense)*cSmokeHPLimit)) then
      [*] = ;         begin
      [*] = ;            var smokeCount : Integer = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokeCount;
      [*] = ;            if (smokeCount>0) then
      [*] = ;            begin
      [*] = ;               var maxind : Integer = floor((1-((arg_obj.hp-smokeHPMaxIntense)/cSmokeHPLimit)/((maxhp-smokeHPMaxIntense)))*smokeCount);
      [*] = ;               if (maxind<0) then
      [*] = ;               maxind := -1;
      [*] = ;
      [*] = ;               var behHnd : Integer = GetBehaviourByClassName(myHnd, 'TXProgressChildrenBehaviour');
      [*] = ;               if (maxind>=0) and (behHnd=0) then
      [*] = ;               BehaviourCreateWithKey(myHnd, 'TXProgressChildrenBehaviour', 'prgchild', False)
      [*] = ;               else
      [*] = ;               if (maxind<0) and (behHnd<>0) and (arg_obj.smokecount<=1) then
      [*] = ;               BehaviourDestroy(behHnd);
      [*] = ;
      [*] = ;               var lastind : Integer = arg_obj.smokecount-1;
      [*] = ;               if (maxind<>lastind) then
      [*] = ;               begin
      [*] = ;                  var plHnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;                  if (maxind>lastind) and ((progresstick mod cProgressSmokeIncrease)=0) then
      [*] = ;                  begin
      [*] = ;                     var diff : Integer = maxind-arg_obj.smokecount+1;
      [*] = ;                     var freq : Float = MinFloat((1-0.5*diff/smokeCount)*0.8, 0.8);
      [*] = ;                     if (random>freq) then
      [*] = ;                     begin
      [*] = ;                        var i : Integer;
      [*] = ;                        for i:=arg_obj.smokecount to maxind do
      [*] = ;                        begin
      [*] = ;                           var x : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].x;
      [*] = ;                           var y : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].y;
      [*] = ;                           var z : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].z;
      [*] = ;                           var nx : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].nx;
      [*] = ;                           var ny : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].ny;
      [*] = ;                           var nz : Float = gCustomObjPoints[arg_obj.cid][arg_obj.id].smokePoints[i].nz;
      [*] = ;                           var px, py, pz : Float;
      [*] = ;                           GetGameObjectAbsolutePositionByHandle(myHnd, px, py, pz);
      [*] = ;                           var effHnd : Integer = CreatePlayerGameObjectHandleByHandle(plHnd, gc_racename_misc, 'point', px+x, py+y, pz+z);
      [*] = ;                           SetGameObjectOrientationByHandle(effHnd, nx, ny, nz, 0, 1, 0);
      [*] = ;                           GameObjectParentToChildAbsolute(effHnd, myHnd);
      [*] = ;                           gEffectLibrary.GetOrCreate('buildingsmoke', effHnd);
      [*] = ;                           break;
      [*] = ;                        end;
      [*] = ;                        arg_obj.smokecount := arg_obj.smokecount+1;
      [*] = ;                     end;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  if (maxind<lastind) and (random>0.5) then
      [*] = ;                  begin
      [*] = ;                     var deltaind : Integer = lastind-maxind;
      [*] = ;                     if (deltaind>0) then
      [*] = ;                     begin
      [*] = ;                        var curCount : Integer;
      [*] = ;                        var i : Integer;
      [*] = ;                        for i:=0 to GetGameObjectCountChildByHandle(myHnd)-1 do
      [*] = ;                        begin
      [*] = ;                           var childHnd : Integer = GetGameObjectGOHandleChildByHandle(myHnd, i);
      [*] = ;                           if (GetGameObjectRaceTag(childHnd)=gc_racetag_misc) then
      [*] = ;                           curCount := curCount+1;
      [*] = ;                        end;
      [*] = ;                        var newdeltaind : Integer = (curCount-1)-maxind;
      [*] = ;                        for i:=GetGameObjectCountChildByHandle(myHnd)-1 downto 0 do
      [*] = ;                        begin
      [*] = ;                           if (newdeltaind<=0) then
      [*] = ;                           break;
      [*] = ;                           var childHnd : Integer = GetGameObjectGOHandleChildByHandle(myHnd, i);
      [*] = ;                           if (GetGameObjectRaceTag(childHnd)=gc_racetag_misc) then
      [*] = ;                           begin
      [*] = ;                              BehaviourClear(childHnd);
      [*] = ;                              EffectClear(childHnd);
      [*] = ;                              GameObjectRequestToDestroyByHandle(childHnd);
      [*] = ;                              newdeltaind := newdeltaind-1;
      [*] = ;                              break;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                     arg_obj.smokecount := arg_obj.smokecount-1;
      [*] = ;                  end;
      [*] = ;                  //arg_obj.smokecount := maxind+1;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (TObjProp(pobjprop).searchradius>0) and (arg_obj.bsearchenemy) then
      [*] = ;         begin
      [*] = ;            var stoHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;            if (stoHnd=0) and (arg_obj.orders[0].itype=gc_obj_order_type_none) then
      [*] = ;            begin
      [*] = ;               if (arg_obj.orders[0].itype=gc_obj_order_type_attackobj) then
      [*] = ;               stoHnd := arg_obj.orders[0].info.trg;
      [*] = ;               if (stoHnd=0) then
      [*] = ;               _unit_DoSearchVictim(myHnd, false);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (arg_obj.orders[0].itype<>gc_obj_order_type_none) then
      [*] = ;         ExecuteState('DoProgressOrders');
      [*] = ;
      [*] = ;         if (TObjProp(pobjprop).bcansetrally) and (TObjProp(pobjprop).usage<>gc_obj_usage_center) and (TObjProp(pobjprop).usage<>gc_obj_usage_port) and ((arg_obj.progresstick mod cProgressRallyPointTick)=0) then
      [*] = ;         begin
      [*] = ;            var rallyx, rallyy : Float;
      [*] = ;            rallyx := arg_obj.rallyx;
      [*] = ;            rallyy := arg_obj.rallyy;
      [*] = ;            if (FindBestPosition(myHnd, rallyx, rallyy)) then
      [*] = ;            begin
      [*] = ;               arg_obj.rallytmpx := rallyx;
      [*] = ;               arg_obj.rallytmpy := rallyy;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (arg_obj.topZone < 0) and (not TObjProp(pobjprop).bWall) then
      [*] = ;         _unit_TopologyAdd(myHnd);
      [*] = ;
      [*] = ;         if (TObjProp(pobjprop).bslowdeath) and (arg_obj.hp<=gc_BuildingSlowDeathHP) then
      [*] = ;         begin
      [*] = ;            arg_obj.hp := arg_obj.hp-(floor(deltatime*gc_BuildingSlowDeathSpeed)+1);
      [*] = ;            if (arg_obj.hp<10) then
      [*] = ;            begin
      [*] = ;               var plIndIO : Integer = GetPlayerIndexInterfaceIO;
      [*] = ;               var isEnemy : Boolean = (gPlayer[plIndIO].enemyplmask and gPlayer[arg_obj.pl].myplmask)<>0;
      [*] = ;               if (arg_obj.pl<>plIndIO) and (isEnemy=True) then
      [*] = ;               begin
      [*] = ;                  gProfileUserStruct.achs.ach[gc_ach_destroy_buildings10].cur := Min(gProfileUserStruct.achs.ach[gc_ach_destroy_buildings10].cur+1, 10);
      [*] = ;                  gProfileUserStruct.achs.ach[gc_ach_destroy_buildings100].cur := Min(gProfileUserStruct.achs.ach[gc_ach_destroy_buildings100].cur+1, 100);
      [*] = ;               end;
      [*] = ;               _unit_SetTagStates(myHnd, gc_statetag_essential_death);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (arg_obj.bbuilt) and (gObjProp[arg_obj.cid][arg_obj.id].usage=gc_obj_usage_mine) then
      [*] = ;         begin
      [*] = ;            if (arg_inside.GetCount)>(gObjProp[arg_obj.cid][arg_obj.id].peasantabsorber+arg_obj.individual.addpeasantabsorber) then
      [*] = ;            begin
      [*] = ;               gIntegerList.Clear;
      [*] = ;               gIntegerList.Add(arg_inside.Get(0));
      [*] = ;               _unit_DoUnitsGoOutside(gIntegerList, false, true);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      const cProgressSyncUnitsParams = 15;
      [*] = ;      if (progresstick mod cProgressSyncUnitsParams=0) and (_net_IsOnline or _net_IsRecord) then
      [*] = ;      begin
      [*] = ;         var uid : Integer = GetGameObjectUniqueIdByHandle(myHnd);
      [*] = ;         if gLanSyncUnitsParamsUIDList.IndexOf(uid)<0 then
      [*] = ;         gLanSyncUnitsParamsUIDList.Add(GetGameObjectUniqueIdByHandle(myHnd));
      [*] = ;      end;
      [*] = ;
      [*] = ;      arg_obj.lastprogresstime := GetGameTime;
      [*] = ;      arg_obj.progresstick := progresstick+1;
      [*] = ;   end;
      [*] = ;end;
   struct.end
section.end

