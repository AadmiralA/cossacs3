section.begin
   Name = ProgressAI
   Code : struct.begin
      [*] = ;var plHnd : Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plInd : Integer = GetPlayerIndexByHandle(plHnd);
      [*] = ;if (_player_CheckProgressLastTimeAndIncrease(gPlayer[plInd].lastprogressaitime, 2, gc_global_TimeProgressAI)) then
      [*] = ;begin
      [*] = ;   ExecuteState('ProgressEconomicAI');
      [*] = ;   ExecuteState('ProgressWarAI');
      [*] = ;end
      [*] = ;else
      [*] = ;if (gbool_gui_recheckadviseradvice) and (plHnd=GetPlayerHandleInterfaceIO) then
      [*] = ;begin
      [*] = ;   ExecuteState('ProgressEconomicAI');
      [*] = ;   gbool_gui_recheckadviseradvice := False;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

