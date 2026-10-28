section.begin
   Name = WriteRes
   Code : struct.begin
      [*] = ;var bSendData : Boolean = ((gbool_net_forcesyncres) or (not GetProgressingPauseMode)) and (((_net_IsServer) and (IsLanReady)) or (_net_IsRecord));
      [*] = ;
      [*] = ;var i, j : Integer;
      [*] = ;if (bSendData) then
      [*] = ;begin
      [*] = ;   gbool_net_forcesyncres := False;
      [*] = ;   gSyncRes.Clear;
      [*] = ;   var plChangedRes : Word;
      [*] = ;   var byteChangedRes : array [0..gc_MaxPlayerCount-1] of Byte;
      [*] = ;   var byteCompressionMode : array [0..gc_MaxPlayerCount-1] of Byte;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   begin
      [*] = ;      for j:=0 to gc_ResCount-1 do
      [*] = ;      if (gPlayer[i].lanres[j]<>gPlayer[i].setres[j]) then
      [*] = ;      byteChangedRes[i] := byteChangedRes[i] or (1 shl j);
      [*] = ;
      [*] = ;      if (byteChangedRes[i]<>0) then
      [*] = ;      begin
      [*] = ;         plChangedRes := plChangedRes or (1 shl i);
      [*] = ;         for j:=0 to gc_ResCount-1 do
      [*] = ;         if ((byteChangedRes[i] and (1 shl j))<>0) then
      [*] = ;         begin
      [*] = ;            var delta : Integer = (not gPlayer[i].setres[j])-(not gPlayer[i].lanres[j]);
      [*] = ;            if Abs(delta)<=127 then
      [*] = ;            begin
      [*] = ;               if (delta<0) then
      [*] = ;               delta := 128+floor(Abs(delta)); // one bit for sign
      [*] = ;               byteCompressionMode[i] := byteCompressionMode[i] or (1 shl j);
      [*] = ;               gSyncRes.Add(delta);
      [*] = ;            end
      [*] = ;            else
      [*] = ;            gSyncRes.Add(gPlayer[i].setres[j]);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   if (gSyncRes.GetCount>0) then
      [*] = ;   begin
      [*] = ;      if not RecordCustomBegin('ReadRes') then
      [*] = ;      _net_ErrorLog('RecordCustomBegin(''ReadRes'')');
      [*] = ;
      [*] = ;      // write data to package
      [*] = ;      RecordCustomWriteWord(plChangedRes);
      [*] = ;      var enum : Integer;
      [*] = ;      for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;      begin
      [*] = ;         if ((plChangedRes and (1 shl i))<>0) then
      [*] = ;         begin
      [*] = ;            RecordCustomWriteByte(byteChangedRes[i]);
      [*] = ;            RecordCustomWriteByte(byteCompressionMode[i]);
      [*] = ;            for j:=0 to gc_ResCount-1 do
      [*] = ;            begin
      [*] = ;               if ((byteChangedRes[i] and (1 shl j))<>0) then
      [*] = ;               begin
      [*] = ;                  if ((byteCompressionMode[i] and (1 shl j))<>0) then
      [*] = ;                  begin
      [*] = ;                     var int32 : TInt32;
      [*] = ;                     int32.SetInt32(gSyncRes.Get(enum));
      [*] = ;                     var b : Byte = int32.GetByte0;
      [*] = ;                     RecordCustomWriteByte(gSyncRes.Get(enum));
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  RecordCustomWriteInteger(gSyncRes.Get(enum));
      [*] = ;                  enum := enum+1;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if _net_debug_log_res then
      [*] = ;      _net_Log('progress.aix:writeres(list.count='+IntToStr(gSyncRes.GetCount));
      [*] = ;
      [*] = ;      if _net_debug_log_size_writer then
      [*] = ;      _net_Log_SizeWriter;
      [*] = ;
      [*] = ;      // finish package
      [*] = ;      RecordCustomEnd;
      [*] = ;
      [*] = ;      for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;      for j:=0 to gc_ResCount-1 do
      [*] = ;      gPlayer[i].lanres[j] := gPlayer[i].setres[j];
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;begin
      [*] = ;   for j:=0 to gc_ResCount-1 do
      [*] = ;   gPlayer[i].res[j] := gPlayer[i].setres[j];
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

