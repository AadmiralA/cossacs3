section.begin
   Name = ReadDeath
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bFromServer: Boolean = RecordCustomReadBoolean;
      [*] = ;var mode: Integer = RecordCustomReadInteger;
      [*] = ;var randkey: Float = RecordCustomReadFloat;
      [*] = ;var count: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;if (_net_IsServer) then
      [*] = ;begin
      [*] = ;   gDeathUnits.Clear();
      [*] = ;   var i: Integer;
      [*] = ;   for i:=0 to count-1 do begin
      [*] = ;      var uid: Integer = RecordCustomReadInteger;
      [*] = ;      var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;      if hnd<>0 then
      [*] = ;      gDeathUnits.Add(hnd)
      [*] = ;      else _net_ErrorLog('readdeath invalid uid('+IntToStr(uid)+')');
      [*] = ;   end;
      [*] = ;   arg_plr.fmode := mode;
      [*] = ;   arg_plr.frnd := randkey;
      [*] = ;   arg_plr.fintlst := gDeathUnits;
      [*] = ;   ExecuteState('WriteDeath');
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;begin // _net_IsClient or _net_IsReplay
      [*] = ;   var prevrandkey : Float = GetRandomKey;
      [*] = ;   SetRandomKey(floor(Abs(randkey)));
      [*] = ;   var i: Integer;
      [*] = ;   for i:=0 to count-1 do begin
      [*] = ;      var uid: Integer = RecordCustomReadInteger;
      [*] = ;      var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;      if hnd<>0 then begin
      [*] = ;         if (mode=0) then
      [*] = ;         begin
      [*] = ;            var pobj : Pointer = _unit_GetTObj(hnd);
      [*] = ;            if (pobj<>nil) then
      [*] = ;            begin
      [*] = ;               var pobjprop : Pointer = _unit_GetObjProp(hnd);
      [*] = ;               if (TObjProp(pobjprop).bbuilding) and (TObjProp(pobjprop).bslowdeath) and (TObj(pobj).bbuilt) then
      [*] = ;               begin
      [*] = ;                  if (TObj(pobj).hp>gc_BuildingSlowDeathHP) then
      [*] = ;                  begin
      [*] = ;                     TObj(pobj).hp := gc_BuildingSlowDeathHP-floor(RandomExt*gc_BuildingSlowDeathRandom);
      [*] = ;                     if (gPlayer[TObj(pobj).pl].lists.deluids.IndexOf(uid)<0) then
      [*] = ;                     gPlayer[TObj(pobj).pl].lists.deluids.Add(uid);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else _net_ErrorLog('readdeath invalid uid('+IntToStr(uid)+')');
      [*] = ;   end;
      [*] = ;   SetRandomKey(floor(Abs(prevrandkey)));
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   var i: Integer;
      [*] = ;   for i:=0 to count-1 do // prevent from assert
      [*] = ;   RecordCustomReadInteger;
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_death then
      [*] = ;_net_Log('readdeath(count='+IntToStr(count)+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

