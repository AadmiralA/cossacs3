section.begin
   Name = OnAclAnimationEnd
   Code : struct.begin
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var tag : Integer = GetGameObjectStatesTagByHandle(myHnd);
      [*] = ;var iEssential : Integer = tag and gc_statetag_essential;
      [*] = ;var iVisual : Integer = tag and gc_statetag_visual;
      [*] = ;
      [*] = ;var newVisual : Integer = gc_statetag_visual_none;
      [*] = ;if (iVisual=gc_statetag_visual_stage_0) then
      [*] = ;newVisual := gc_statetag_visual_stage_1;
      [*] = ;
      [*] = ;if (newVisual<>gc_statetag_visual_none) then
      [*] = ;_unit_SetTagStates(myHnd, newVisual);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

