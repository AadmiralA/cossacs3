section.begin
   Name = ReadStats
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;const cCount = 6;
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var mask1 : Integer = RecordCustomReadInteger;
      [*] = ;var mask2 : Integer = RecordCustomReadInteger;
      [*] = ;var i, j : Integer;
      [*] = ;var plcount : Integer = gc_MaxPlayerCount;
      [*] = ;if (_net_IsReplay) then
      [*] = ;begin
      [*] = ;   if (gRecordGeneratorVersion<53) then
      [*] = ;   plcount := 8
      [*] = ;   else
      [*] = ;   if (gRecordGeneratorVersion<89) then
      [*] = ;   plcount := 9;
      [*] = ;end;
      [*] = ;for i:=0 to plcount-1 do
      [*] = ;if ((mask1 and (1 shl i))<>0) then
      [*] = ;begin
      [*] = ;   for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;   if (mask1 and (1 shl (plcount+j))<>0) then
      [*] = ;   gPlayer[i].stat.restotal[j] := RecordCustomReadInteger;
      [*] = ;   
      [*] = ;   for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;   if (mask1 and (1 shl (plcount+cCount+j))<>0) then
      [*] = ;   gPlayer[i].stat.resonupgrade[j] := RecordCustomReadInteger;
      [*] = ;   
      [*] = ;   for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;   if (mask1 and (1 shl (plcount+cCount*2+j))<>0) then
      [*] = ;   gPlayer[i].stat.resonmines[j] := RecordCustomReadInteger;
      [*] = ;   
      [*] = ;   for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;   if (mask2 and (1 shl j)<>0) then
      [*] = ;   gPlayer[i].stat.resonunits[j] := RecordCustomReadInteger;
      [*] = ;   
      [*] = ;   for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;   if (mask2 and (1 shl (cCount*1+j))<>0) then
      [*] = ;   gPlayer[i].stat.resonbuildings[j] := RecordCustomReadInteger;
      [*] = ;   
      [*] = ;   for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;   if (mask2 and (1 shl (cCount*2+j))<>0) then
      [*] = ;   gPlayer[i].stat.resonlife[j] := RecordCustomReadInteger;
      [*] = ;   
      [*] = ;   for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;   if (mask2 and (1 shl (cCount*3+j))<>0) then
      [*] = ;   gPlayer[i].stat.resbuy[j] := RecordCustomReadInteger;
      [*] = ;   
      [*] = ;   for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;   if (mask2 and (1 shl (cCount*4+j))<>0) then
      [*] = ;   gPlayer[i].stat.ressell[j] := RecordCustomReadInteger;
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

