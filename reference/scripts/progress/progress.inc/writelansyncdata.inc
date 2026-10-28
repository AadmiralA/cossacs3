section.begin
   Name = WriteLanSyncData
   Code : struct.begin
      [*] = ;
      [*] = ;var bSendData : Boolean = (gbool_net_forcesyncdata or (not GetProgressingPauseMode)) and (((_net_IsServer) and (IsLanReady)) or (_net_IsRecord));
      [*] = ;
      [*] = ;var i, j : Integer;
      [*] = ;
      [*] = ;// update squads and put all the updated ones into tmplists
      [*] = ;procedure ProcessSquads(prevdata, newdata : TLanSyncPlayerData);
      [*] = ;begin
      [*] = ;   // we need temporary global lists for uids and time
      [*] = ;   gTmpIntListLANSquadUIDList.Clear;
      [*] = ;   gTmpIntListLANSquadTimeList.Clear;
      [*] = ;   var prevcount, newcount, maxlength : Integer;
      [*] = ;   prevcount := prevdata.squadsuids.GetCount;
      [*] = ;   newcount := newdata.squadsuids.GetCount;
      [*] = ;   maxlength := max(prevcount, newcount);
      [*] = ;   var i : Integer;
      [*] = ;   for i:=0 to maxlength-1 do
      [*] = ;   begin
      [*] = ;      var squaduid1, squaduid2 : Integer;
      [*] = ;      var squadtime1, squadtime2 : Word;
      [*] = ;      if (i<prevcount) then
      [*] = ;      begin
      [*] = ;         squaduid1 := prevdata.squadsuids.Get(i);
      [*] = ;         squadtime1 := prevdata.squadstime.Get(i);
      [*] = ;      end;
      [*] = ;      if (i<newcount) then
      [*] = ;      begin
      [*] = ;         squaduid2 := newdata.squadsuids.Get(i);
      [*] = ;         squadtime2 := newdata.squadstime.Get(i);
      [*] = ;      end;
      [*] = ;      if ((squaduid1<>squaduid2) or (squadtime1<>squadtime2)) and (squaduid2<>0) then
      [*] = ;      begin
      [*] = ;         gTmpIntListLANSquadUIDList.Add(squaduid2);
      [*] = ;         gTmpIntListLANSquadTimeList.Add(squadtime2);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (bSendData) then
      [*] = ;begin
      [*] = ;   gbool_net_forcesyncdata := false;
      [*] = ;
      [*] = ;   gLanSyncData.playerstosync := 0;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   begin
      [*] = ;      gLanSyncData.netplayer[i].SetupPlayerEconomy(gPlayer[i]);
      [*] = ;      gLanSyncData.netplayer[i].SetupSquads(gPlayer[i]);
      [*] = ;   end;
      [*] = ;   gLanSyncData.ProcessEconomy(gLanSyncDataLast);
      [*] = ;
      [*] = ;   if not RecordCustomBegin('ReadLanSyncData') then
      [*] = ;   _net_ErrorLog('RecordCustomBegin(''ReadLanSyncData'')');
      [*] = ;   RecordCustomWriteWord(gLanSyncData.playerstosync);
      [*] = ;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   if ((gLanSyncData.playerstosync and (1 shl i))<>0) then
      [*] = ;   gLanSyncData.netplayer[i].WriteEconomyPackage(gLanSyncData.economyfieldstosync[i]);
      [*] = ;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   begin
      [*] = ;      ProcessSquads(gLanSyncDataLast.netplayer[i], gLanSyncData.netplayer[i]);
      [*] = ;      var squadscount : Integer = gTmpIntListLANSquadUIDList.GetCount;
      [*] = ;      RecordCustomWriteInteger(squadscount);
      [*] = ;      if (squadscount>0) then
      [*] = ;      begin
      [*] = ;         gLanSyncData.playerstosync := gLanSyncData.playerstosync or (1 shl i);
      [*] = ;         var j : Integer;
      [*] = ;         for j:=0 to squadscount-1 do
      [*] = ;         begin
      [*] = ;            var uid : Integer = gTmpIntListLANSquadUIDList.Get(j);
      [*] = ;            var bHoldMode : Boolean = TSquad(gPlayer[i].squads.GetByUID(uid)).fHoldMode;
      [*] = ;            var time : Word = gTmpIntListLANSquadTimeList.Get(j);
      [*] = ;            RecordCustomWriteInteger(uid);
      [*] = ;            RecordCustomWriteBoolean(bHoldMode);
      [*] = ;            if (not bHoldMode) then
      [*] = ;            RecordCustomWriteWord(time);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   RecordCustomEnd;
      [*] = ;
      [*] = ;   // cache current values as previous ones for the next cycle
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   begin
      [*] = ;      if ((gLanSyncData.playerstosync and (1 shl i))<>0) then
      [*] = ;      gLanSyncDataLast.netplayer[i].AssignFrom(gLanSyncData.netplayer[i]);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

