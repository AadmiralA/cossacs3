section.begin
   Name = OnAclAnimationFloat
   Code : struct.begin
      [*] = ;procedure DoCreateBehaviour(const goHnd : Integer; const radius, behangle, scale, speed, x, y, z, xdisp, zdisp, height : Float);
      [*] = ;begin
      [*] = ;   var behHnd : Integer = BehaviourCreate(goHnd, 'TXMoveRotateWaveBehaviour', true, false);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'OffsetTime', RandomExt);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'RollAngle', (0.5-RandomExt)*10);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'WavingAmpX', 0.3+(0.5-RandomExt)*0.2);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'WavingAmpY', 0.3+(0.5-RandomExt)*0.2);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'WavingAmpZ', 0.3+(0.5-RandomExt)*0.2);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'WavingFreqX', 2);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'WavingFreqY', 0.5);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'WavingFreqZ', 2);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'WaveTimeX', 1*pi*0.1);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'WaveTimeY', 2*pi*0.1);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'WaveTimeZ', 1*pi*0.1);
      [*] = ;   var waveSinCos : array [0..4] of String;
      [*] = ;   waveSinCos[0] := 'wscNone';
      [*] = ;   waveSinCos[1] := 'wscSin';
      [*] = ;   waveSinCos[2] := 'wscSin';
      [*] = ;   waveSinCos[3] := 'wscCos';
      [*] = ;   waveSinCos[4] := 'wscCos';
      [*] = ;   SetBehaviourStringProperty(behHnd, 'WaveSinCosX', waveSinCos[Floor(RandomExt*5)]);
      [*] = ;   SetBehaviourStringProperty(behHnd, 'WaveSinCosY', waveSinCos[Floor(RandomExt*5)]);
      [*] = ;   SetBehaviourStringProperty(behHnd, 'WaveSinCosZ', waveSinCos[Floor(RandomExt*5)]);
      [*] = ;
      [*] = ;   SetBehaviourStringProperty(behHnd, 'ClearPaths', '');
      [*] = ;
      [*] = ;   SetBehaviourStringProperty(behHnd, 'MovementPathPosition', 'mppUse');
      [*] = ;   SetBehaviourStringProperty(behHnd, 'MovementPathScale', 'mpsNone');
      [*] = ;   SetBehaviourStringProperty(behHnd, 'MovementPathRotate', 'mprNone');
      [*] = ;
      [*] = ;   SetBehaviourStringProperty(behHnd, 'AddPath', '');
      [*] = ;   var i : Integer;
      [*] = ;   i := 0;
      [*] = ;   SetBehaviourStringProperty(behHnd, 'PathsAddNode[0]', '');
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesX[0,'+IntToStr(i)+']', x);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesY[0,'+IntToStr(i)+']', y);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesZ[0,'+IntToStr(i)+']', z);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesScaleX[0,'+IntToStr(i)+']', scale);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesScaleY[0,'+IntToStr(i)+']', scale);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesScaleZ[0,'+IntToStr(i)+']', scale);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesSpeed[0,'+IntToStr(i)+']', speed);
      [*] = ;
      [*] = ;   {i := 1;
      [*] = ;   SetBehaviourStringProperty(behHnd, 'PathsAddNode[0]', '');
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesX[0,'+IntToStr(i)+']', x);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesY[0,'+IntToStr(i)+']', y+height);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesZ[0,'+IntToStr(i)+']', z);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesScaleX[0,'+IntToStr(i)+']', scale);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesScaleY[0,'+IntToStr(i)+']', scale);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesScaleZ[0,'+IntToStr(i)+']', scale);
      [*] = ;   SetBehaviourFloatProperty(behHnd, 'PathsNodesSpeed[0,'+IntToStr(i)+']', speed); }
      [*] = ;
      [*] = ;
      [*] = ;   for i := 1 to 36 do
      [*] = ;   begin
      [*] = ;      var angle : Float = behangle + i * 10;
      [*] = ;      if (angle>=360) then
      [*] = ;      angle := angle - 360;
      [*] = ;      SetBehaviourStringProperty(behHnd, 'PathsAddNode[0]', '');
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesX[0,'+IntToStr(i)+']', x+radius*Cos(angle*pi/180)*xdisp);
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesY[0,'+IntToStr(i)+']', y+height);
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesZ[0,'+IntToStr(i)+']', z+radius*Sin(angle*pi/180)*zdisp);
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesScaleX[0,'+IntToStr(i)+']', scale);
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesScaleY[0,'+IntToStr(i)+']', scale);
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesScaleZ[0,'+IntToStr(i)+']', scale);
      [*] = ;      SetBehaviourFloatProperty(behHnd, 'PathsNodesSpeed[0,'+IntToStr(i)+']', speed*0.3);
      [*] = ;   end;
      [*] = ;
      [*] = ;   SetBehaviourIntProperty(behHnd, 'ActivePathIndex', 0);
      [*] = ;   SetBehaviourBoolProperty(behHnd, 'PathsShowPath[0]', false);
      [*] = ;   SetBehaviourBoolProperty(behHnd, 'PathsLooped[0]', true);
      [*] = ;   SetBehaviourStringProperty(behHnd, 'StartPathTravel', '');
      [*] = ;
      [*] = ;   SetBehaviourBoolProperty(behHnd, 'Enabled', true);
      [*] = ;end;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var i : Integer;
      [*] = ;for i:=GetGameObjectCountChildByHandle(myHnd)-1 downto 0 do
      [*] = ;SetGameObjectVisibleByHandle(GetGameObjectGOHandleChildByHandle(myHnd, i), false);
      [*] = ;
      [*] = ;DoCreateBehaviour(myHnd, 50, 10, 1, 3, GetGameObjectPositionXByHandle(myHnd), GetGameObjectPositionYByHandle(myHnd), GetGameObjectPositionZByHandle(myHnd), 1, 1, 0);
      [*] = ;GameObjectMySwitchToTreeAnimationCycles('fly', false, false, false, false);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

