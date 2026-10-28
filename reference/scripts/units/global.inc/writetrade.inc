section.begin
   Name = WriteTrade
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var sell: Byte = arg_plr.fbyte0;
      [*] = ;var buy : Byte = arg_plr.fbyte1;
      [*] = ;var amount: Integer = arg_plr.famount;
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then begin
      [*] = ;   if (amount>not gPlayer[plind].res[sell]) then
      [*] = ;   amount := not gPlayer[plind].res[sell];
      [*] = ;   _res_MarketTradeResources(plhnd, sell, buy, amount);
      [*] = ;end;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadTrade') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadTrade'')');
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;   RecordCustomWriteByte(sell);
      [*] = ;   RecordCustomWriteByte(buy);
      [*] = ;   RecordCustomWriteInteger(amount);
      [*] = ;
      [*] = ;   if _net_debug_log_trade then
      [*] = ;      _net_Log('writetrade(pl='+IntToStr(plind)+';sell='+IntToStr(sell)+';buy='+IntToStr(buy)+';amount='+IntToStr(amount));
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;      _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

