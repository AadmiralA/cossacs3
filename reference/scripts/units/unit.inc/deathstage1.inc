section.begin
   Name = DeathStage1
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;procedure DoSemiTransparent(goHnd : Integer; totaltime, maxalpha, minalpha : Float; bJustUpdate : Boolean);
      [*] = ;begin
      [*] = ;   var effHnd : Integer;
      [*] = ;   if (bJustUpdate) then
      [*] = ;   begin
      [*] = ;      effHnd := GetEffectByKey(goHnd, 'death');
      [*] = ;      if (effHnd<>0) then
      [*] = ;      begin
      [*] = ;         SetBehaviourFloatProperty(effHnd, 'AlphaChannel[0]', maxalpha);
      [*] = ;         SetBehaviourFloatProperty(effHnd, 'AlphaChannel[1]', minalpha);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      ErrorLog('DoSemiTransparent (bJustUpdate=True) : effHnd=0');
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      BehaviourDestroy(GetEffectByKey(goHnd, 'death'));
      [*] = ;      effHnd := EffectCreateWithKey(goHnd, 'TXLifeColorEffect', 'death', false);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseAmbient', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseDiffuse', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseEmission', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseSpecular', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseAlphaChannel', True);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseShininess', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseTexOffset', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseTexScale', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseShaderID', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseColor', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseJustColorIfNoLight', True);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseFrontFace', True);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseBackFace', True);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseIgnoreFog', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseNoLighting', False);
      [*] = ;      SetBehaviourIntProperty(effHnd,  'CurrIndex', 0);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'Enable', True);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'Loop', False); // False
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'LoopBackward', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'BounceBackward', False);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'ZWrite', True);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'ZTest', True);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseBlendMode', True);
      [*] = ;      SetBehaviourBoolProperty(effHnd, 'UseCullFace', True);
      [*] = ;      SetBehaviourIntProperty(effHnd, 'BlendingMode', 1); // bmTransparency
      [*] = ;      SetBehaviourIntProperty(effHnd, 'FaceCulling', 0);  // fcCull
      [*] = ;      
      [*] = ;      SetBehaviourStringProperty(effHnd, 'Add', '');
      [*] = ;      SetBehaviourFloatProperty(effHnd, 'AlphaChannel[0]', maxalpha);
      [*] = ;      SetBehaviourFloatProperty(effHnd, 'Time[0]', totalTime);
      [*] = ;      SetBehaviourStringProperty(effHnd, 'Add', '');
      [*] = ;      SetBehaviourFloatProperty(effHnd, 'AlphaChannel[1]', minalpha);
      [*] = ;      SetBehaviourFloatProperty(effHnd, 'Time[1]', totalTime);
      [*] = ;      var i : Integer;
      [*] = ;      for i:=GetGameObjectCountChildByHandle(goHnd)-1 downto 0 do
      [*] = ;      DoSemiTransparent(GetGameObjectGOHandleChildByHandle(goHnd, i), totaltime, maxalpha, minalpha, bJustUpdate);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;const maxalpha = 1;
      [*] = ;const minalpha = 0.4;
      [*] = ;var bship : Boolean = _unit_IsWaterUnit(myHnd);
      [*] = ;var time : Float = _misc_SwitchFloat(gc_unit_deathtime_1, gc_unit_deathtime_1ship, bship);
      [*] = ;
      [*] = ;// dispertion
      [*] = ;var unittype : Integer = _misc_GetUnitType(GetGameObjectMyHandle);
      [*] = ;if (not bship) and (unittype<>gc_result_unittype_art) then
      [*] = ;time := 2+time-random*(time/1);
      [*] = ;
      [*] = ;if (not bship) then
      [*] = ;DoSemiTransparent(GetGameObjectMyHandle, time, maxalpha, minalpha, False);
      [*] = ;GameObjectMyDelayExecuteState('DeathStage2', time);
      [*] = ;
      [*] = ;if (bship) then
      [*] = ;_unit_SetTagStates(myHnd, gc_statetag_visual_stage_1);
      [*] = ;
      [*] = ;SetGameObjectMyStateMachineInterval(0);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

