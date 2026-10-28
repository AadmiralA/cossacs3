section.begin
   Name = DeathStage2
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;var unittype : Integer = _misc_GetUnitType(GetGameObjectMyHandle);
      [*] = ;
      [*] = ;procedure SetupBehaviour(goHnd : Integer; disp : Float);
      [*] = ;begin
      [*] = ;   GameObjectDoRayCastTerrainByHandle(goHnd);
      [*] = ;   SetGameObjectAutoOffsetByHandle(goHnd, false);
      [*] = ;   var posX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;   var posY : Float = GetGameObjectPositionYByHandle(goHnd);
      [*] = ;   var posZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;   
      [*] = ;   var behHnd : Integer = BehaviourCreate(goHnd, 'TXMoveRotateWaveBehaviour', true, false);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'OffsetTime', 0);
      [*] = ;   SetBehaviourBoolProperty(behHnd, 'Enabled', true);
      [*] = ;   
      [*] = ;   SetBehaviourStringProperty(behHnd, 'ClearPaths', '');
      [*] = ;   SetBehaviourStringProperty(behHnd, 'MovementPathPosition', 'mprUse');
      [*] = ;   SetBehaviourStringProperty(behHnd, 'AddPath', '');
      [*] = ;   
      [*] = ;   var i : Integer;
      [*] = ;   var step : Float = 0.05;
      [*] = ;   var speed : Float = 0.011/disp;
      [*] = ;   if (unittype=gc_result_unittype_art) then
      [*] = ;   begin
      [*] = ;      step := 0.125;
      [*] = ;      speed := 0.022/disp;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   if (unittype=gc_result_unittype_ship) then
      [*] = ;   begin
      [*] = ;      step := 0.125*5;
      [*] = ;      speed := 0.022*2.5/disp;
      [*] = ;   end;
      [*] = ;   if (GetTimeSpeedFactor>0) then
      [*] = ;   speed := speed*(GetTimeSpeedFactor/10);
      [*] = ;   var count : Integer = 10;
      [*] = ;   for i:=0 to count-1 do
      [*] = ;   begin
      [*] = ;      SetBehaviourStringProperty(behHnd, 'PathsAddNode[0]', '');
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesX[0,'+IntToStr(i)+']', posX);
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesY[0,'+IntToStr(i)+']', posY-step*i);
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesZ[0,'+IntToStr(i)+']', posZ);
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesSpeed[0,'+IntToStr(i)+']', speed);
      [*] = ;   end;
      [*] = ;   
      [*] = ;   SetBehaviourStringProperty(behHnd, 'PathsPathSplineMode[0]', 'lsmBezierSpline'); // lsmLines, lsmCubicSpline, lsmBezierSpline, lsmNURBSCurve, lsmSegments
      [*] = ;   
      [*] = ;   SetBehaviourIntProperty(behHnd, 'ActivePathIndex', 0);
      [*] = ;   SetBehaviourBoolProperty(behHnd, 'PathsLooped[0]', false);
      [*] = ;   SetBehaviourStringProperty(behHnd, 'StartPathTravel', '');
      [*] = ;   
      [*] = ;   var effHnd : Integer = EffectCreateWithKey(goHnd, 'TXLifeColorEffect', 'death', false);
      [*] = ;   var deltatime : Float = GetBehaviourFloatProperty(effHnd, 'DeltaTime');;
      [*] = ;   if (effHnd<>0) and (deltatime<0.1) then
      [*] = ;   begin
      [*] = ;      SetBehaviourFloatProperty(effHnd, 'DeltaTime', 0.99);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;var bship : Boolean = _unit_IsWaterUnit(myHnd);
      [*] = ;var deftime : Float = gc_unit_deathtime_2;
      [*] = ;if (unittype=gc_result_unittype_art) then
      [*] = ;deftime := gc_unit_deathtime_2art
      [*] = ;else
      [*] = ;deftime := _misc_SwitchFloat(deftime, gc_unit_deathtime_2ship, bship);
      [*] = ;
      [*] = ;// dispertion
      [*] = ;var disp : Float;
      [*] = ;if (not bship) and (unittype<>gc_result_unittype_art) then
      [*] = ;disp := random*(deftime/1.2);
      [*] = ;var time : Float = deftime-disp;
      [*] = ;
      [*] = ;SetupBehaviour(myHnd, time/deftime);
      [*] = ;GameObjectMyDelayExecuteState('DeathStage3', time);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

