section.begin
   Name = SoundControlCounters
   Code : struct.begin
      [*] = ;// is in lib script
      [*] = ;Log('SoundControlCounters');
      [*] = ;exit;
      [*] = ;
      [*] = ;// _sound_EmitterPlaySound
      [*] = ;//
      [*] = ;function _sound_EmitterPlayWalkSound(const emitterHnd : Integer; const libind, sndlibind : Integer; vol, interval : Float; bplayuniq : Boolean) : Integer;
      [*] = ;begin
      [*] = ;   if (emitterHnd<>0) then
      [*] = ;   begin
      [*] = ;      var newind : Integer;
      [*] = ;      var rndind : Integer;
      [*] = ;      var maxind : Integer = gSoundLibrary[sndlibind].count-1;
      [*] = ;      if (maxind>0) then
      [*] = ;      rndind := floor(random*(maxind+1));
      [*] = ;
      [*] = ;      // test hack, make different counters for different types and support several counters on one type at once
      [*] = ;      if maxind<0 then
      [*] = ;      ErrorLog('_sound_EmitterPlayWalkSound maxind<0');
      [*] = ;      rndind := gint_soundsteptick mod (maxind+1);
      [*] = ;      gint_soundsteptick := gint_soundsteptick+1;
      [*] = ;      //
      [*] = ;
      [*] = ;      var i : Integer;
      [*] = ;      for i:=0 to maxind do
      [*] = ;      begin
      [*] = ;         newind := rndind+i;
      [*] = ;         if (newind>maxind) then
      [*] = ;         newind := newind-maxind-1;
      [*] = ;         var libname : String;
      [*] = ;         case libind of
      [*] = ;            gc_sndlib_ind_units : libname := 'units';
      [*] = ;            gc_sndlib_ind_interface : libname := 'interface';
      [*] = ;            gc_sndlib_ind_ambient : libname := 'ambient';
      [*] = ;         end;
      [*] = ;         var sndTag : Integer;
      [*] = ;         if (not bplayuniq) then
      [*] = ;         sndtag := _sound_GetTagByIndexes(sndlibind, newind)
      [*] = ;         else
      [*] = ;         sndtag := 1000000+sndlibind;
      [*] = ;         Result := sndtag;
      [*] = ;
      [*] = ;         var sndHnd : Integer = SndGetOrCreateSound(sndtag, libname, emitterHnd);
      [*] = ;         if (not GetSndSoundPlaying(sndHnd)) then
      [*] = ;         begin
      [*] = ;            SetSndSoundSourceName(gSoundLibrary[sndlibind].list[newind], sndHnd);
      [*] = ;            SetSndSoundVolume(vol, sndHnd);
      [*] = ;            //log('_sound_EmitterPlaySound : '+gSoundLibrary[sndlibind].list[newind]);
      [*] = ;            SetSndSoundRadius(8000, sndHnd); //80 - for 3D sound, 8000 for isometric sound
      [*] = ;            SetSndSoundPlaying(True, sndHnd);
      [*] = ;            break;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         SetSndSoundVolume(vol, sndHnd);
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   Result := 0;
      [*] = ;end;
      [*] = ;
      [*] = ;procedure GetScreenListenerPos(var lx, ly, lz : Float);
      [*] = ;begin
      [*] = ;   var rx, ry, rz : Float;
      [*] = ;   var px, py, pz : Float;
      [*] = ;   ScreenToWorldRayCast(GetViewerWidth div 2, GetViewerHeight div 2{floor((GetViewerHeight div 7)*3)}, rx, ry, rz);
      [*] = ;   GetCameraTargetPosition(px, py, pz);
      [*] = ;   var dst : Float = VectorDistance(px, py, pz, rx, ry, rz);
      [*] = ;   var dirx : Float = rx-px;
      [*] = ;   var diry : Float = ry-py;
      [*] = ;   var dirz : Float = rz-pz;
      [*] = ;   VectorNormalize(dirx, diry, dirz);
      [*] = ;   // listner pos
      [*] = ;   lx := rx-dst*dirx*0.3;
      [*] = ;   ly := ry-dst*diry*0.15;
      [*] = ;   lz := rz-dst*dirz*(0.0);
      [*] = ;end;
      [*] = ;
      [*] = ;function GetOrCreateSoundEmitterInListenerPos(lx, ly, lz : Float) : Integer;
      [*] = ;begin
      [*] = ;   Result := gSoundManager.emitterhnd;
      [*] = ;   if (Result=0) then
      [*] = ;   begin
      [*] = ;      if (GetCountOfPlayers>=gc_playerind_misc) then
      [*] = ;      begin
      [*] = ;         var miscplhnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;         Result := CreatePlayerGameObjectHandleByHandle(miscplhnd, gc_racename_misc, 'soundemitter', lx, ly, lz);
      [*] = ;         SetGameObjectVisibleByHandle(Result, False);
      [*] = ;         gSoundManager.emitterhnd := Result;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   SetGameObjectPositionByHandle(Result, lx, ly, lz);
      [*] = ;end;
      [*] = ;
      [*] = ;var lastprogresstime : Float = gProgress.soundcounterlastprogresstime;
      [*] = ;var deltatime : Float = GetCurrentTime-lastprogresstime;
      [*] = ;
      [*] = ;if (deltatime>0) and ((MinFloat((GetTimeSpeedFactor/10), 2)*deltatime)>0.09) then // not on pause
      [*] = ;begin
      [*] = ;   var gametime : Float = GetGameTime;
      [*] = ;   var lx, ly, lz : Float;
      [*] = ;   var lheight : Float;
      [*] = ;   GetScreenListenerPos(lx, ly, lz);
      [*] = ;
      [*] = ;   var listenerEmitterHnd : Integer = GetOrCreateSoundEmitterInListenerPos(lx, ly, lz);
      [*] = ;   var basevol : Float;
      [*] = ;   var progresstick : Integer = gProgress.soundcounterprogresstick;
      [*] = ;   const minind = 0;
      [*] = ;   const maxind = 7;
      [*] = ;   var i : Integer;
      [*] = ;   for i:=minind to maxind do
      [*] = ;   begin
      [*] = ;      var counter, sndlibind, ifreq : Integer;
      [*] = ;      var dst : Float;
      [*] = ;      case i of
      [*] = ;         0 : counter := gSoundManager.counter.walkinfcloth;
      [*] = ;         1 : counter := gSoundManager.counter.walkinfmetal;
      [*] = ;         2 : counter := gSoundManager.counter.walkartwheels;
      [*] = ;         3 : counter := gSoundManager.counter.walkartmortar;
      [*] = ;         4 : counter := gSoundManager.counter.walkcavfast;
      [*] = ;         5 : counter := gSoundManager.counter.walkcavslow;
      [*] = ;         6 : counter := gSoundManager.counter.walkpea;
      [*] = ;         7 : counter := gSoundManager.counter.walkpeastone;
      [*] = ;      end;
      [*] = ;      case i of
      [*] = ;         0 : dst := gSoundManager.counter.walkdstinfcloth;
      [*] = ;         1 : dst := gSoundManager.counter.walkdstinfmetal;
      [*] = ;         2 : dst := gSoundManager.counter.walkdstartwheels;
      [*] = ;         3 : dst := gSoundManager.counter.walkdstartmortar;
      [*] = ;         4 : dst := gSoundManager.counter.walkdstcavfast;
      [*] = ;         5 : dst := gSoundManager.counter.walkdstcavslow;
      [*] = ;         6 : dst := gSoundManager.counter.walkdstpea;
      [*] = ;         7 : dst := gSoundManager.counter.walkdstpeastone;
      [*] = ;      end;
      [*] = ;      case i of
      [*] = ;         0 : sndlibind := gc_snd_ind_walkinfcloth;
      [*] = ;         1 : sndlibind := gc_snd_ind_walkinfmetal;
      [*] = ;         2 : sndlibind := gc_snd_ind_walkartwheels;
      [*] = ;         3 : sndlibind := gc_snd_ind_walkartmortar;
      [*] = ;         4 : sndlibind := gc_snd_ind_walkcavfast;
      [*] = ;         5 : sndlibind := gc_snd_ind_walkcavslow;
      [*] = ;         6 : sndlibind := gc_snd_ind_walkpea;
      [*] = ;         7 : sndlibind := gc_snd_ind_walkpeastone;
      [*] = ;      end;
      [*] = ;      case i of
      [*] = ;         0 : ifreq := 3;
      [*] = ;         1 : ifreq := Max(3-((counter-1) div 5), 2);
      [*] = ;         2 : ifreq := Max(5-((counter-1) div 3), 3);
      [*] = ;         3 : ifreq := Max(5-((counter-1) div 3), 3);
      [*] = ;         4 : ifreq := Max(5-counter+1, 3);
      [*] = ;         5 : ifreq := Max(6-counter+1, 4);
      [*] = ;         6 : ifreq := 3;
      [*] = ;         7 : ifreq := 3;
      [*] = ;      end;
      [*] = ;      case i of
      [*] = ;         0 : basevol := 1;
      [*] = ;         1 : basevol := 1;
      [*] = ;         2 : basevol := 0.35;
      [*] = ;         3 : basevol := 0.35;
      [*] = ;         4 : basevol := 1;
      [*] = ;         5 : basevol := 1;
      [*] = ;         6 : basevol := 0.1;
      [*] = ;         7 : basevol := 1;
      [*] = ;      end;
      [*] = ;      // temp fix
      [*] = ;      ifreq := 3;
      [*] = ;      if (counter>0) then
      [*] = ;      begin
      [*] = ;         if gametime>gProgress.lastsoundprogresscounterfreqtime[i] then
      [*] = ;         begin
      [*] = ;            gProgress.lastsoundprogresscounterfreqtime[i] := gametime+ifreq*gc_progress_TimeSoundProgressCounterFreq;
      [*] = ;            const interval = 100;
      [*] = ;            var vol : Float = basevol*((gSoundManager.frustumrad-(dst/counter))/gSoundManager.frustumrad);
      [*] = ;            vol := Clamp(vol, 0, 1);
      [*] = ;            var sndtag : Integer = _sound_EmitterPlayWalkSound(listenerEmitterHnd, gc_sndlib_ind_units, sndlibind, vol, interval, False);
      [*] = ;            // log
      [*] = ;            gint_dbg_logsoundcount := gint_dbg_logsoundcount+1;
      [*] = ;            if False then
      [*] = ;            begin
      [*] = ;               var sndHnd : Integer = SndGetOrCreateSound(sndtag, 'units', listenerEmitterHnd);
      [*] = ;               SetSndSoundVolume(vol, sndHnd);
      [*] = ;               var sndName : String = GetSndSoundSourceName(sndHnd);
      [*] = ;               if sndName<>'' then
      [*] = ;               TimeLog('listenerEmitterHnd:'+' sndlibind='+IntToStr(sndlibind)+' count='+IntToStr(counter)+' vol='+FloatToStr(vol)+' sndname='+sndName);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   gProgress.soundcounterprogresstick := gProgress.soundcounterprogresstick + 1;
      [*] = ;   gProgress.soundcounterlastprogresstime := GetCurrentTime;
      [*] = ;end;
   struct.end
section.end

