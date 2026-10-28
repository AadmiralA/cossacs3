section.begin
   Name = ReadSyncUnitsParams
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;if _net_IsServer then
      [*] = ;_net_ErrorLog('_net_IsServer=true');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var count: Integer = RecordCustomReadInteger;
      [*] = ;var i: Integer;
      [*] = ;for i:=0 to count-1 do begin
      [*] = ;   var uid: Integer = RecordCustomReadInteger;
      [*] = ;   var bvalid: Boolean = RecordCustomReadBoolean;
      [*] = ;    if bvalid then
      [*] = ;   begin
      [*] = ;      var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;      var pobj: Pointer = _unit_GetTObj(hnd);
      [*] = ;      if (pobj<>nil) then
      [*] = ;      begin
      [*] = ;         var hp : Integer = RecordCustomReadInteger;
      [*] = ;         if hp<>TObj(pobj).hp then
      [*] = ;         TObj(pobj).hp := hp;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         ErrorLog('ReadSyncUnitsParams : invalid hnd');
      [*] = ;         RecordCustomReadInteger;
      [*] = ;      end
      [*] = ;   end
      [*] = ;   else
      [*] = ;   ErrorLog('ReadSyncUnitsParams : invalid uid');
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

