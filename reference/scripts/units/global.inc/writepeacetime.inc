section.begin
   Name = WritePeaceTime
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadPeaceTime') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadPeaceTime'')');
      [*] = ;
      [*] = ;   var bstate: Boolean = arg_plr.fbstate;
      [*] = ;
      [*] = ;   if (_net_IsOnline) and (not bstate) and (gMap.brating) and (LanPublicServerProfCountry<>'') then
      [*] = ;   begin
      [*] = ;      var country : String = LanPublicServerProfCountry;
      [*] = ;      if (StrExists(country, '|f=')) then
      [*] = ;      begin
      [*] = ;         var pos : Integer = StrPos('|f=', country);
      [*] = ;         var lm : String = SubStr(country, pos+3, 1);
      [*] = ;         country := StrReplace(country, '|f='+lm, '');
      [*] = ;         LanSetMyCountry(country);
      [*] = ;         LanPublicServerRegister;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteBoolean(bstate);
      [*] = ;
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

