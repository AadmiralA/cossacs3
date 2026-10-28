section.begin
   Name = ReadApply
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var plind: Integer = RecordCustomReadInteger;
      [*] = ;var uid: Integer = RecordCustomReadInteger;
      [*] = ;var cid: Integer = RecordCustomReadInteger;
      [*] = ;var ind: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;var plr: Integer = GetPlayerHandleByIndex(plind);
      [*] = ;var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;
      [*] = ;if(hnd<>0) or (plr<>0) then
      [*] = ;   _player_ApplyUpgrade(plr, hnd, cid, ind, false)
      [*] = ;else _net_ErrorLog('readapply invalid uid('+IntToStr(uid)+')');
      [*] = ;
      [*] = ;if _net_debug_log_apply then
      [*] = ;   _net_Log('readapply(uid='+IntToStr(uid)+';cid='+IntToStr(cid)+';ind='+IntToStr(ind)+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end