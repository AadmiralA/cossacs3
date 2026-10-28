section.begin
   Name = ReadGate
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var state: Boolean = RecordCustomReadBoolean;
      [*] = ;var count: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;var i: Integer;
      [*] = ;for i:=0 to count-1 do begin
      [*] = ;   var hnduid: Integer = RecordCustomReadInteger;
      [*] = ;   var handle: Integer = GetGameObjectHandleByUniqueId(hnduid);
      [*] = ;   if handle<>0 then
      [*] = ;   begin
      [*] = ;      if (state) then
      [*] = ;      _unit_SetTagStates(handle, gc_statetag_visual_stage_0)
      [*] = ;      else
      [*] = ;      _unit_SetTagStates(handle, gc_statetag_visual_none);
      [*] = ;   end
      [*] = ;   else _net_ErrorLog('readgate invalid uid('+IntToStr(hnduid)+')');
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_gate then
      [*] = ;   _net_Log('readgate(count='+IntToStr(count)+';state='+BoolToStr(state)+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

