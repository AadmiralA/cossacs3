section.begin
   Name = OnDestroy
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;if (not arg_obj.bdead) then
      [*] = ;ExecuteState('OnDeath');
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   var phnd: Integer = GetGameObjectPlayerHandleByHandle(myHnd);
      [*] = ;   if (arg_obj.pl=gc_playerind_misc) then
      [*] = ;   phnd:=GetPlayerHandleByIndex(0);
      [*] = ;   var parg: Pointer = StateMachineGetArgDataByInd(GetPlayerStateMachineHandle(phnd), gc_argplayer_plr);
      [*] = ;   if parg<>nil then begin
      [*] = ;      TPlayerArgs(parg).fhandle:=myHnd;
      [*] = ;      PlayerExecuteStateByHandle(phnd, 'WriteFree');
      [*] = ;   end else _net_ErrorLog('invalid pointer, parg=nil');
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

