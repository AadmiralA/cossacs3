section.begin
   Name = WriteTradeResources
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var plind : Integer = arg_plr.fbyte0;
      [*] = ;var targetplayer : Byte = arg_plr.fbyte1;
      [*] = ;var resource: Byte = arg_plr.fbyte2;
      [*] = ;var amount: Integer = arg_plr.famount;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then begin
      [*] = ;   if (amount>not gPlayer[plind].res[resource]) then
      [*] = ;      amount := not gPlayer[plind].res[resource];
      [*] = ;   _res_AddResToPlayerByIndex(targetplayer, resource, amount);
      [*] = ;   _res_AddResToPlayerByIndex(plind, resource, -amount);
      [*] = ;   if _net_IsServer then
      [*] = ;   _misc_LanForceSyncRes;
      [*] = ;end;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   if not RecordCustomBegin('ReadTradeResources') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadTradeResources'')');
      [*] = ;
      [*] = ;   RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;   RecordCustomWriteByte(plind);
      [*] = ;   RecordCustomWriteByte(targetplayer);
      [*] = ;   RecordCustomWriteByte(resource);
      [*] = ;   RecordCustomWriteInteger(amount);
      [*] = ;
      [*] = ;   if _net_debug_log_trade then
      [*] = ;      _net_Log('writetraderesources(pl='+IntToStr(plind)+';resource='+IntToStr(resource)+';targetplayer='+IntToStr(targetplayer)+';amount='+IntToStr(amount));
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;      _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

