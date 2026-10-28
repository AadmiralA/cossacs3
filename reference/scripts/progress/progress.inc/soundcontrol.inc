section.begin
   Name = SoundControl
   Code : struct.begin
      [*] = ;// is in lib script
      [*] = ;Log('SoundControl');
      [*] = ;exit;
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
      [*] = ;   //DebugDrawClean('a');
      [*] = ;   //DebugDrawBox('a', lx, ly, lz, 1, 1, 1, 1);
      [*] = ;end;
      [*] = ;
      [*] = ;// _sound_EmitterPlaySound
      [*] = ;//
      [*] = ;function _sound_EmitterPlaySound(const emitterHnd : Integer; const libind, sndlibind : Integer; bplayuniq : Boolean) : Integer;
      [*] = ;begin
      [*] = ;   if (emitterHnd<>0) then
      [*] = ;   begin
      [*] = ;      var newind : Integer;
      [*] = ;      var rndind : Integer;
      [*] = ;      var maxind : Integer = gSoundLibrary[sndlibind].count-1;
      [*] = ;      if (maxind>0) then
      [*] = ;      rndind := floor(random*(maxind+1));
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
      [*] = ;         //var sndtag : Integer = floor(random*1000000);
      [*] = ;         Result := sndtag;
      [*] = ;         //SndRemoveSound(sndtag, emitterHnd);
      [*] = ;
      [*] = ;         var sndHnd : Integer = SndGetOrCreateSound(sndtag, libname, emitterHnd);
      [*] = ;
      [*] = ;         var loudness : Float = gSoundLibrary[sndlibind].loudness*1.25;
      [*] = ;         SetSndSoundRadiuses(0, loudness, sndHnd);
      [*] = ;
      [*] = ;         if (not GetSndSoundPlaying(sndHnd)) then
      [*] = ;         begin
      [*] = ;            SetSndSoundSourceName(gSoundLibrary[sndlibind].list[newind], sndHnd);
      [*] = ;            //log('_sound_EmitterPlaySound : '+gSoundLibrary[sndlibind].list[newind]);
      [*] = ;            SetSndSoundPlaying(True, sndHnd);
      [*] = ;            break;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   Result := 0;
      [*] = ;end;
      [*] = ;
      [*] = ;_misc_ProfilerBegin('SoundControl');
      [*] = ;
      [*] = ;var lastprogresstime : Float = gProgress.soundlastprogresstime;
      [*] = ;var progresstick : Integer = gProgress.soundcounterprogresstick;
      [*] = ;
      [*] = ;procedure IntegerListMove(var fromlist, tolist : TIntegerList);
      [*] = ;begin
      [*] = ;   var i : Integer;
      [*] = ;   for i:=fromlist.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      tolist.Add(fromlist.Get(i));
      [*] = ;      fromlist.Delete(i);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;function GetOrCreateSoundEmitterInPos(posx, posz : Float) : Integer;
      [*] = ;begin
      [*] = ;   Result := 0;
      [*] = ;   GetGameObjectsInRadius(posx, posz, gc_epsilon, false, false, 0, -1, 0, false, false, false, false, false, false);
      [*] = ;   var i : Integer;
      [*] = ;   for i:=GetGameObjectListCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var goHnd : Integer = GetGameObjectListByIndex(i);
      [*] = ;      if (GetGameObjectStringPropertyTag(goHnd)=gc_properties_stringtag_soundemitter) then
      [*] = ;      begin
      [*] = ;         Result := goHnd;
      [*] = ;         break;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   if (Result=0) then
      [*] = ;   begin
      [*] = ;      Result := CreatePoolGameObjectHandle(gc_racename_misc, 'soundemitter', posx, RayCastHeight(posx, posz), posz);
      [*] = ;      GameObjectDelayExecuteStateByHandle(Result, 'FreeToPool', 10);
      [*] = ;      SetGameObjectVisibleByHandle(Result, False);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;function GetOrCreateSoundEmitterInListenerPos(lx, ly, lz : Float) : Integer;
      [*] = ;begin
      [*] = ;   Result := gSoundManager.emitterhnd;
      [*] = ;   if (Result=0) then
      [*] = ;   begin
      [*] = ;      Result := CreatePoolGameObjectHandle(gc_racename_misc, 'soundemitter', lx, ly, lz);
      [*] = ;      //FreeToPoolGameObjectTimeoutHandle(Result, 10);
      [*] = ;      SetGameObjectVisibleByHandle(Result, False);
      [*] = ;      gSoundManager.emitterhnd := Result;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   SetGameObjectPositionByHandle(Result, lx, ly, lz);
      [*] = ;end;
      [*] = ;
      [*] = ;_misc_ProfilerBegin('s0');
      [*] = ;var lx, ly, lz, plx, ply, plz : Float;
      [*] = ;var lheight : Float;
      [*] = ;GetScreenListenerPos(lx, ly, lz);
      [*] = ;GetPosSoundManagerListenerAsObject(plx, ply, plz);
      [*] = ;if (VectorDistance(plx, ply, plz, lx, ly, lz)>1) then
      [*] = ;SetPosSoundManagerListenerAsObject(lx, ly, lz);
      [*] = ;lheight := ly;
      [*] = ;ScreenToWorldRayCast(GetViewerWidth div 2, GetViewerHeight div 2, lx, ly, lz);
      [*] = ;_misc_ProfilerEnd('s0');
      [*] = ;
      [*] = ;//log('----');
      [*] = ;var gridx, gridy : Integer;
      [*] = ;// combine
      [*] = ;const tries = 1;
      [*] = ;_misc_ProfilerBegin('s1');
      [*] = ;var enum : Integer;
      [*] = ;var bfreqok : array [0..3] of Boolean;
      [*] = ;var gametime : Float = GetGameTime;
      [*] = ;var i : Integer;
      [*] = ;for i:=0 to 3 do
      [*] = ;begin
      [*] = ;   if gametime>gProgress.lastsoundprogressfreqtime[i] then
      [*] = ;   begin
      [*] = ;      gProgress.lastsoundprogressfreqtime[i] := gametime+(i+1)*gc_progress_TimeSoundProgressFreq;
      [*] = ;      bfreqok[i] := True;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   bfreqok[i] := False;
      [*] = ;end;
      [*] = ;
      [*] = ;for i:=0 to 3 do
      [*] = ;bfreqok[i] := True;
      [*] = ;
      [*] = ;for enum:=0 to tries-1 do
      [*] = ;begin
      [*] = ;   for gridy:=0 to gc_soundgrid_county-1 do
      [*] = ;   for gridx:=0 to gc_soundgrid_countx-1 do
      [*] = ;   begin
      [*] = ;      if (gSoundGrid[gridx, gridy].bsndrequestexist) then
      [*] = ;      begin
      [*] = ;         var maxdst : Float = 40;
      [*] = ;         var gposx, gposz : Float;
      [*] = ;         _misc_SoundGridIndicesToPos(gridx, gridy, gposx, gposz);
      [*] = ;         var lenx : Float = VectorLength(lx-gposx, 0, 0);
      [*] = ;         var lenz : Float = VectorLength(0, 0, lz-gposz);
      [*] = ;         var dst : Float = VectorDistance(0, 0, 0, lenx, 0, lenz);
      [*] = ;         var m : Integer;
      [*] = ;         {if (dst>(maxdst+lheight)) then
      [*] = ;         begin
      [*] = ;            for m:=gc_snd_ind_count-1 downto 0 do
      [*] = ;            if (gSoundGrid[gridx, gridy].sndrequests[m]>0) then
      [*] = ;            gSoundGrid[gridx, gridy].sndrequests[m] := 0;
      [*] = ;         end
      [*] = ;         else}
      [*] = ;         begin
      [*] = ;            for m:=gc_snd_ind_count-1 downto 0 do
      [*] = ;            begin
      [*] = ;               if (gSoundGrid[gridx, gridy].sndrequests[m]>0) and bfreqok[gSoundLibrary[m].sndfreq] then
      [*] = ;               begin
      [*] = ;                  var loudness : Float = gSoundLibrary[m].loudness*1.25;
      [*] = ;                  if (dst>(maxdst+lheight+loudness)) then
      [*] = ;                  gSoundGrid[gridx, gridy].sndrequests[m] := 0
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     const rad = 4+4;
      [*] = ;                     var maxcount : Integer;
      [*] = ;                     var maxx, maxy : Integer = -1;
      [*] = ;                     var i, j : Integer;
      [*] = ;                     for i:=-rad to rad do
      [*] = ;                     for j:=-rad to rad do
      [*] = ;                     begin
      [*] = ;                        var testx, testy : Integer;
      [*] = ;                        testx := gridx+i;
      [*] = ;                        testy := gridy+j;
      [*] = ;                        if (i=0) and (j=0) then
      [*] = ;                        continue;
      [*] = ;                        if (testx<0) or (testx>=gc_soundgrid_countx) or (testy<0) or (testy>=gc_soundgrid_county) then
      [*] = ;                        continue;
      [*] = ;                        if (gSoundGrid[testx, testy].sndrequests[m]>maxcount) then
      [*] = ;                        begin
      [*] = ;                           maxcount := gSoundGrid[testx, testy].sndrequests[m];
      [*] = ;                           maxx := testx;
      [*] = ;                           maxy := testy;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                     if (maxcount>0) and ((maxx>=0) or (maxx<gc_soundgrid_countx) or (maxy>=0) or (maxy<gc_soundgrid_county)) then
      [*] = ;                     begin
      [*] = ;                        if (maxcount>=gSoundGrid[gridx, gridy].sndrequests[m]) then
      [*] = ;                        begin
      [*] = ;                           gSoundGrid[maxx, maxy].sndrequests[m] := gSoundGrid[maxx, maxy].sndrequests[m]+gSoundGrid[gridx, gridy].sndrequests[m];
      [*] = ;                           gSoundGrid[gridx, gridy].sndrequests[m] := 0;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           gSoundGrid[gridx, gridy].sndrequests[m] := gSoundGrid[gridx, gridy].sndrequests[m]+gSoundGrid[maxx, maxy].sndrequests[m];
      [*] = ;                           gSoundGrid[maxx, maxy].sndrequests[m] := 0;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;_misc_ProfilerEnd('s1');
      [*] = ;// play
      [*] = ;_misc_ProfilerBegin('s2');
      [*] = ;for gridy:=0 to gc_soundgrid_county-1 do
      [*] = ;for gridx:=0 to gc_soundgrid_countx-1 do
      [*] = ;begin
      [*] = ;   if (gSoundGrid[gridx, gridy].bsndrequestexist) then
      [*] = ;   begin
      [*] = ;      var i, j : Integer;
      [*] = ;      for i:=gc_snd_ind_count-1 downto 0 do
      [*] = ;      if (gSoundGrid[gridx, gridy].sndrequests[i]>0) and bfreqok[gSoundLibrary[i].sndfreq] then
      [*] = ;      begin
      [*] = ;         //_misc_ProfilerBegin('s2');
      [*] = ;         var posx, posy, posz : Float;
      [*] = ;         _misc_SoundGridIndicesToPos(gridx, gridy, posx, posz);
      [*] = ;         var emitterHnd : Integer = GetOrCreateSoundEmitterInPos(posx, posz);
      [*] = ;         //var emitterHnd : Integer = GetOrCreateSoundEmitter;
      [*] = ;         var sndlibind : Integer = i;
      [*] = ;
      [*] = ;         var bgroup : Boolean;
      [*] = ;         var c : Integer = gSoundGrid[gridx, gridy].sndrequests[i];
      [*] = ;         for j:=gSoundLibrary[sndlibind].groupscount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            if (gSoundLibrary[sndlibind].groups[j].maxcount>0) and (c>=gSoundLibrary[sndlibind].groups[j].mincount) and (c<=gSoundLibrary[sndlibind].groups[j].maxcount) then
      [*] = ;            begin
      [*] = ;               bgroup := True;
      [*] = ;               sndlibind := gSoundLibrary[sndlibind].groups[j].sndind;
      [*] = ;               break;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;
      [*] = ;         var sndtag : Integer = _sound_EmitterPlaySound(emitterHnd, gc_sndlib_ind_units, sndlibind, False);
      [*] = ;         gSoundGrid[gridx, gridy].sndrequests[i] := 0;
      [*] = ;
      [*] = ;         // log
      [*] = ;         gint_dbg_logsoundcount := gint_dbg_logsoundcount+1;
      [*] = ;         if False then
      [*] = ;         begin
      [*] = ;            var sndHnd : Integer = SndGetOrCreateSound(sndtag, 'units', emitterHnd);
      [*] = ;            var sndName : String = GetSndSoundSourceName(sndHnd);
      [*] = ;            if sndName<>'' then
      [*] = ;            begin
      [*] = ;               if (bgroup) then
      [*] = ;               TimeLog('posx='+FloatToStr(round(posx))+' posz='+FloatToStr(round(posz))+' sndlibind='+IntToStr(sndlibind)+' count='+IntToStr(c)+' sndname='+sndName+' [group]')
      [*] = ;               else
      [*] = ;               TimeLog('posx='+FloatToStr(round(posx))+' posz='+FloatToStr(round(posz))+' sndlibind='+IntToStr(sndlibind)+' count='+IntToStr(c)+' sndname='+sndName);
      [*] = ;               //_misc_ProfilerEnd('s2');
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   gSoundGrid[gridx, gridy].bsndrequestexist := False;
      [*] = ;end;
      [*] = ;_misc_ProfilerEnd('s2');
      [*] = ;
      [*] = ;_misc_ProfilerEnd('SoundControl');
      [*] = ;
      [*] = ;gProgress.soundprogresstick := gProgress.soundprogresstick + 1;
      [*] = ;gProgress.soundlastprogresstime := GetGameTime;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

