section.begin
   Name = ReadPeaceTime
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bstate: Boolean = RecordCustomReadBoolean;
      [*] = ;
      [*] = ;if (_net_IsOnline) and (gbool_peacemode) and (not bstate) and (gMap.brating) and (LanPublicServerProfCountry<>'') then
      [*] = ;begin
      [*] = ;   var country : String = LanPublicServerProfCountry;
      [*] = ;   if (StrExists(country, '|f=')) then
      [*] = ;   begin
      [*] = ;      var pos : Integer = StrPos('|f=', country);
      [*] = ;      var lm : String = SubStr(country, pos+3, 1);
      [*] = ;      country := StrReplace(country, '|f='+lm, '');
      [*] = ;      LanSetMyCountry(country);
      [*] = ;      LanPublicServerRegister;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (gbool_peacemode<>bstate) then
      [*] = ;gbool_peacemode := bstate
      [*] = ;else
      [*] = ;_net_ErrorLog('ReadPeaceTime(gbool_peacemode=bstate='+BoolToStr(bstate)+')');
      [*] = ;
      [*] = ;GUIMiniMapPrimitivesClear;
      [*] = ;_misc_UpdateMiniMapPrimitives(True);
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

