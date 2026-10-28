section.begin
   Name = ReadPackage
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var bFromServer: Boolean = RecordCustomReadBoolean;
      [*] = ;var str: String = RecordCustomReadString;
      [*] = ;
      [*] = ;if _net_IsServer then
      [*] = ;begin
      [*] = ;   arg_plr.fbasename := str;
      [*] = ;   ExecuteState('WritePackage');
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;_misc_ProcessNetMessage(str);
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

