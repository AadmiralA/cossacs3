section.begin
   Name = OnSave
   UsedFileName = 
   Code : struct.begin
      [*] = ;{if (gScenario.bexists) then
      [*] = ;begin
      [*] = ;   var scenarioname : String = StrReplace(gScenario.settings.filename, '.map', '');
      [*] = ;   var filename : String = GetCurrentMapFileName;
      [*] = ;   var path : String = ExtractFilePath(filename);
      [*] = ;   var langfilepath : String = path+scenarioname+'.lng';
      [*] = ;   var langtable : String = gScenario.settings.langtable;
      [*] = ;   var ind : Integer = IndexOfLocaleTableList(langtable);
      [*] = ;   if (ind>=0) then
      [*] = ;   begin
      [*] = ;      var langfilepathbak : String = path+scenarioname+'.bak';
      [*] = ;      if (IsFileExists(langfilepathbak)) then
      [*] = ;      DeleteFileStream(langfilepathbak);
      [*] = ;      if (IsFileExists(langfilepath)) then
      [*] = ;      RenameFileStream(langfilepath, langfilepathbak);
      [*] = ;      SaveLocaleTableList(ind, langfilepath);
      [*] = ;      Log('Save localetable='+langtable+' to langfilepath='+langfilepath);
      [*] = ;   end;
      [*] = ;end;}
      [*] = ;
      [*] = ;log('OnSave');
   struct.end
section.end

