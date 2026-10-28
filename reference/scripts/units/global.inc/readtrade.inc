section.begin
   Name = ReadTrade
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bFromServer: Boolean = RecordCustomReadBoolean;
      [*] = ;var sell: Byte = RecordCustomReadByte;
      [*] = ;var buy : Byte = RecordCustomReadByte;
      [*] = ;var amount: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;if _net_IsServer then begin
      [*] = ;   if (amount>not gPlayer[plind].res[sell]) then
      [*] = ;   amount := not gPlayer[plind].res[sell];
      [*] = ;   arg_plr.fbyte0:=sell;
      [*] = ;   arg_plr.fbyte1:=buy;
      [*] = ;   arg_plr.famount:=amount;
      [*] = ;   ExecuteState('WriteTrade');
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;begin // _net_IsClient or _net_IsReplay
      [*] = ;   _res_MarketTradeResources(plHnd, sell, buy, amount); // doesnt change real amount, just update statistics and market prices
      [*] = ;   if _net_debug_log_trade then
      [*] = ;      _net_Log('readtrade(pl='+IntToStr(plind)+';sell='+IntToStr(sell)+';buy='+IntToStr(buy)+';amount='+IntToStr(amount));
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

