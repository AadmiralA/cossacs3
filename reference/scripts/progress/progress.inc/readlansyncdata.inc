section.begin
   Name = ReadLanSyncData
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;var playerstosync : Word = RecordCustomReadWord;
      [*] = ;var economyfieldstosync : Byte;
      [*] = ;var plind : Word;
      [*] = ;// read economy data from package
      [*] = ;var i,j : Integer;
      [*] = ;for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;begin
      [*] = ;   if ((playerstosync and (1 shl i))<>0) then
      [*] = ;   begin
      [*] = ;      economyfieldstosync := RecordCustomReadByte;
      [*] = ;      if (economyfieldstosync>0) then
      [*] = ;      begin
      [*] = ;         var plind : Byte = RecordCustomReadByte;
      [*] = ;         if ((economyfieldstosync and (1 shl 0))<>0) then
      [*] = ;         gPlayer[plind].counter.idlepeasants := RecordCustomReadWord;
      [*] = ;         if ((economyfieldstosync and (1 shl 1))<>0) then
      [*] = ;         gPlayer[plind].counter.idlemines := RecordCustomReadWord;
      [*] = ;         if ((economyfieldstosync and (1 shl 2))<>0) then
      [*] = ;         gPlayer[plind].counter.workersonres[gc_resource_type_food] := RecordCustomReadWord;
      [*] = ;         if ((economyfieldstosync and (1 shl 3))<>0) then
      [*] = ;         gPlayer[plind].counter.workersonres[gc_resource_type_wood] := RecordCustomReadWord;
      [*] = ;         if ((economyfieldstosync and (1 shl 4))<>0) then
      [*] = ;         gPlayer[plind].counter.workersonres[gc_resource_type_stone] := RecordCustomReadWord;
      [*] = ;         if ((economyfieldstosync and (1 shl 5))<>0) then
      [*] = ;         gPlayer[plind].counter.workersonres[gc_resource_type_gold] := RecordCustomReadWord;
      [*] = ;         if ((economyfieldstosync and (1 shl 6))<>0) then
      [*] = ;         gPlayer[plind].counter.workersonres[gc_resource_type_iron] := RecordCustomReadWord;
      [*] = ;         if ((economyfieldstosync and (1 shl 7))<>0) then
      [*] = ;         gPlayer[plind].counter.workersonres[gc_resource_type_coal] := RecordCustomReadWord;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;begin
      [*] = ;   var squadssize : Integer = RecordCustomReadInteger;
      [*] = ;   if (squadssize>0) then
      [*] = ;   begin
      [*] = ;      var squaduid : Integer;
      [*] = ;      for j:=0 to squadssize-1 do
      [*] = ;      begin
      [*] = ;         squaduid := RecordCustomReadInteger;
      [*] = ;         var psquad : Pointer = gPlayer[i].squads.GetByUID(squaduid);
      [*] = ;         if (psquad<>nil) then
      [*] = ;         begin
      [*] = ;            var bHoldMode : Boolean = RecordCustomReadBoolean;
      [*] = ;            if (not bHoldMode) then
      [*] = ;            begin
      [*] = ;               var squadtime : Word = RecordCustomReadWord;
      [*] = ;               TSquad(psquad).fHoldModeProgress := squadtime/255;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               TSquad(psquad).fHoldMode := true;
      [*] = ;               TSquad(psquad).fHoldModeProgress := 1;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

