section.begin
   Name = SetTagStates
   Code : struct.begin
      [*] = ;var switchState : Integer = gint_unit_tagstate;
      [*] = ;
      [*] = ;var oldState : Integer = GetGameObjectMyStatesTag;
      [*] = ;var newState : Integer = oldState;
      [*] = ;
      [*] = ;var switchEssential : Integer = (switchState and gc_statetag_essential);
      [*] = ;if (switchEssential<>0) then
      [*] = ;newState := (newState and not gc_statetag_essential) or switchEssential;
      [*] = ;
      [*] = ;var switchVisual : Integer = (switchState and gc_statetag_visual);
      [*] = ;if (switchVisual<>0) then
      [*] = ;newState := (newState and not gc_statetag_visual) or switchVisual;
      [*] = ;
      [*] = ;if (newState<>oldState) then
      [*] = ;SetGameObjectMyStatesTag(newState);
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

