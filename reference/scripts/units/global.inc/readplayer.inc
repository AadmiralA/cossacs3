section.begin
   Name = ReadPlayer
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var uid: Integer = RecordCustomReadInteger;
      [*] = ;var cap: Boolean = RecordCustomReadBoolean;
      [*] = ;var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;
      [*] = ;if hnd<>0 then
      [*] = ;   _misc_ChangePlayer(hnd, GetPlayerCurrentPlHandle, cap, false, false)
      [*] = ;else _net_ErrorLog('change player invalid uid('+IntToStr(uid)+')');
      [*] = ;
      [*] = ;if _net_debug_log_player then
      [*] = ;   _net_Log('readplayer(uid='+IntToStr(uid)+';cap='+BoolToStr(cap)+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

