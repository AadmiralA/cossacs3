section.begin
   Name = ReadRes
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;const csign : Byte = 127;
      [*] = ;var bOldVersion : Boolean = (gRecordGeneratorVersion<89) and (_net_IsReplay);
      [*] = ;var plChangedRes : Word;
      [*] = ;var plCount : Integer;
      [*] = ;if (bOldVersion) then
      [*] = ;begin
      [*] = ;   plChangedRes := RecordCustomReadByte;
      [*] = ;   plCount := gc_MaxNonSpectatorsCount;
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   plChangedRes := RecordCustomReadWord;
      [*] = ;   plCount := gc_MaxPlayerCount;
      [*] = ;end;
      [*] = ;var byteChangedRes : array [0..gc_MaxPlayerCount-1] of Byte;
      [*] = ;var byteCompressionMode : array [0..gc_MaxPlayerCount-1] of Byte;
      [*] = ;
      [*] = ;var i, j : Integer;
      [*] = ;for i:=0 to plCount-1 do
      [*] = ;begin
      [*] = ;   if ((plChangedRes and (1 shl i))<>0) then
      [*] = ;   begin
      [*] = ;      byteChangedRes[i] := RecordCustomReadByte;
      [*] = ;      byteCompressionMode[i] := RecordCustomReadByte;
      [*] = ;      for j:=0 to gc_ResCount-1 do
      [*] = ;      begin
      [*] = ;         if ((byteChangedRes[i] and (1 shl j))<>0) then
      [*] = ;         begin
      [*] = ;            if ((byteCompressionMode[i] and (1 shl j))<>0) then
      [*] = ;            begin
      [*] = ;               var val : Byte = RecordCustomReadByte;
      [*] = ;               if val>csign then
      [*] = ;               gPlayer[i].res[j] := not (not gPlayer[i].res[j]-(val-128))
      [*] = ;               else
      [*] = ;               gPlayer[i].res[j] := not (not gPlayer[i].res[j]+val);
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               var val : Integer = RecordCustomReadInteger;
      [*] = ;               if (bOldVersion) then
      [*] = ;               gPlayer[i].res[j] := not val
      [*] = ;               else
      [*] = ;               gPlayer[i].res[j] := val;
      [*] = ;            end;
      [*] = ;            gPlayer[i].setres[j] := gPlayer[i].res[j];
      [*] = ;            gPlayer[i].lanres[j] := gPlayer[i].res[j];
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_res then
      [*] = ;_net_Log('writeres(plchangedmask='+IntToStr(plChangedRes));
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

