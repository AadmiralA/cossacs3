section.begin
   Name = ReadRally
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var uid : Integer = RecordCustomReadInteger;
      [*] = ;var rally : Boolean = RecordCustomReadBoolean;
      [*] = ;var posx: Float   = RecordCustomReadFloat;
      [*] = ;var posz: Float   = RecordCustomReadFloat;
      [*] = ;var hnd : Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;
      [*] = ;if _net_IsServer then begin
      [*] = ;   arg_plr.fhandle := hnd;
      [*] = ;   arg_plr.fposx := posx;
      [*] = ;   arg_plr.fposz := posz;
      [*] = ;   arg_plr.fbstate := rally;
      [*] = ;   ExecuteState('WriteRally');
      [*] = ;end;
      [*] = ;if hnd<>0 then begin
      [*] = ;   //if _unit_CanSetRallyPoint(hnd) then
      [*] = ;   begin
      [*] = ;      var pobj: Pointer = _unit_GetTObj(hnd);
      [*] = ;      if pobj<>nil then begin
      [*] = ;         TObj(pobj).brally:=rally;
      [*] = ;         TObj(pobj).rallyx:=posx;
      [*] = ;         TObj(pobj).rallyy:=posz;
      [*] = ;         TObj(pobj).rallytmpx:=posx;
      [*] = ;         TObj(pobj).rallytmpy:=posz;
      [*] = ;      end else _net_ErrorLog('pobj = nil');
      [*] = ;      gbool_gui_update_rallypoints:=true;
      [*] = ;      gbool_gui_setrallypointmode:=false;
      [*] = ;   end;
      [*] = ;end else _net_ErrorLog('rally point invalid uid('+IntToStr(uid)+')');
      [*] = ;
      [*] = ;if _net_debug_log_rally then
      [*] = ;   _net_Log('readrally(uid='+IntToStr(uid)+';coords=['+FloatToStr(posx)+';'+FloatToStr(posz)+'])');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

