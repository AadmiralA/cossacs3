section.begin
   Name = OnTagStates
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var oldState : Integer = GetGameObjectMyPrevStatesTag;
      [*] = ;var newState : Integer = GetGameObjectMyStatesTag;
      [*] = ;
      [*] = ;var iOldEssential : Integer = oldState and gc_statetag_essential;
      [*] = ;var iEssential : Integer = newState and gc_statetag_essential;
      [*] = ;var iOldVisual : Integer = oldState and gc_statetag_visual;
      [*] = ;var iVisual : Integer = newState and gc_statetag_visual;
      [*] = ;var switchEssential, switchVisual : Integer;
      [*] = ;if (iOldEssential<>iEssential) then switchEssential := iEssential;
      [*] = ;if (iOldVisual<>iEssential) then switchVisual := iEssential;
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
      [*] = ;if (iEssential=gc_statetag_essential_birth) then
      [*] = ;begin
      [*] = ;   case iVisual of
      [*] = ;      gc_statetag_visual_stage_0 : begin  //initial anim
      [*] = ;         SetGameObjectMyAnimationCyclesMode('acmPlayOnce');
      [*] = ;         GameObjectMySwitchToTreeAnimationCycles('transform', false, false, false, false);
      [*] = ;      end;
      [*] = ;      gc_statetag_visual_stage_1 : begin //anim+ropes fade-out
      [*] = ;         const maxalpha = 1;
      [*] = ;         const minalpha = 0.4;
      [*] = ;         SetGameObjectMyAnimationCyclesMode('acmPlayOnce');
      [*] = ;         GameObjectMySwitchToTreeAnimationCycles('rope', false, false, false, false);
      [*] = ;         var animTime : Float = 1.1;
      [*] = ;         var i : Integer;
      [*] = ;         for i:=GetGameObjectCountChildByHandle(myHnd)-1 downto 0 do
      [*] = ;         DoSemiTransparent(GetGameObjectGOHandleChildByHandle(myHnd, i), animTime, maxalpha, minalpha, false);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

