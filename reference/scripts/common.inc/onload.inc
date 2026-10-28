section.begin
   Name = OnLoad
   UsedFileName = 
   Code : struct.begin
      [*] = ;if (gScenario.bexists) then
      [*] = ;begin
      [*] = ;   var scenarioname : String = StrReplace(gScenario.settings.filename, '.map', '');
      [*] = ;   if (scenarioname='') then
      [*] = ;   ErrorLog('common.aix : OnLoad : scenarioname=nil : FIX THIS !!!');
      [*] = ;   var locale : String;
      [*] = ;   _profile_ConvertConfigValueToLocale(gProfile.lang, locale);
      [*] = ;   var localepath : String = '.\data\locale\'+locale+'\missions\';
      [*] = ;   //gScenario.settings.langfilename;
      [*] = ;   var filename : String = GetCurrentMapFileName;
      [*] = ;   var path : String = ExtractFilePath(filename);
      [*] = ;   var langfilepath : String = localepath+scenarioname+'.lng';;
      [*] = ;   var langtable : String = gScenario.settings.langtable;
      [*] = ;   var ind : Integer = IndexOfLocaleTableList(langtable);
      [*] = ;   if (ind>=0) then
      [*] = ;   DeleteLocaleTableList(ind);
      [*] = ;   if (IsFileExists(langfilepath)) then
      [*] = ;   AddLocaleTableList(langtable, langfilepath)
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      langfilepath := path+scenarioname+'.lng';
      [*] = ;      if (IsFileExists(langfilepath)) then
      [*] = ;      AddLocaleTableList(langtable, langfilepath)
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (not gScenario.bactive) then
      [*] = ;   begin
      [*] = ;      const cTriggerName = 'trigger0';
      [*] = ;      var smhnd : Integer = GetTriggerManagerStateMachineHandle(cTriggerName);
      [*] = ;      if (smhnd=0) then
      [*] = ;      TriggerManagerAdd(cTriggerName);
      [*] = ;      var triggeraixfilepath : String = path+scenarioname+'.aix';
      [*] = ;      if (IsFileExists(triggeraixfilepath)) then
      [*] = ;      begin
      [*] = ;         smhnd := GetTriggerManagerStateMachineHandle(cTriggerName);
      [*] = ;         if (smhnd<>0) then
      [*] = ;         StateMachineLoadFromFile(smhnd, triggeraixfilepath);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   // update queries
      [*] = ;   _scenario_UpdateQueries(gScenario);
      [*] = ;end;
      [*] = ;
      [*] = ;gbool_editor_fogofwar := True;
   struct.end
section.end

