section.begin
   Name = ReadTradeResources
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bFromServer: Boolean = RecordCustomReadBoolean;
      [*] = ;var plind : Integer = RecordCustomReadByte;
      [*] = ;var targetplayer : Byte = RecordCustomReadByte;
      [*] = ;var resource : Byte = RecordCustomReadByte;
      [*] = ;var amount: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;if _net_IsServer then begin
      [*] = ;   if (amount>not gPlayer[plind].res[resource]) then
      [*] = ;      amount := not gPlayer[plind].res[resource];
      [*] = ;   arg_plr.fbyte0 := plind;
      [*] = ;   arg_plr.fbyte1 := targetplayer;
      [*] = ;   arg_plr.fbyte2 := resource;
      [*] = ;   arg_plr.famount := amount;
      [*] = ;   ExecuteState('WriteTradeResources');
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then
      [*] = ;begin
      [*] = ;   if _net_debug_log_trade then
      [*] = ;   _net_Log('readtraderesources(pl='+IntToStr(plind)+';resource='+IntToStr(resource)+';targetplayer='+IntToStr(targetplayer)+';amount='+IntToStr(amount));
      [*] = ;end;
      [*] = ;
      [*] = ;// doesnt change real amount, just update statistics, need to have correct values on server and client both
      [*] = ;gPlayer[plInd].stat.restotal[resource] := gPlayer[plInd].stat.restotal[resource]+amount;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

