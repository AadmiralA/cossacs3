section.begin
   Name = WriteStats
   Code : struct.begin
      [*] = ;var bSendData : Boolean = (_net_IsOffline and _net_IsRecord) or (_net_IsOnline and IsLanReady); // Lan Client Execute not needed
      [*] = ;
      [*] = ;var i, j : Integer;
      [*] = ;if (bSendData) then
      [*] = ;begin
      [*] = ;   if not RecordCustomBegin('ReadStats') then
      [*] = ;   _net_ErrorLog('RecordCustomBegin(''ReadStats'')');
      [*] = ;
      [*] = ;   var mask1, mask2 : Integer;
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   if gPlayer[i].stat.resonlife[gc_resource_type_food]>0 then
      [*] = ;   mask1 := mask1 or (1 shl i);
      [*] = ;
      [*] = ;   const cCount = 6;
      [*] = ;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   if gPlayer[i].stat.resonlife[gc_resource_type_food]>0 then
      [*] = ;   begin
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if gPlayer[i].stat.restotal[j]<>0 then
      [*] = ;      mask1 := mask1 or (1 shl (gc_MaxPlayerCount+j));
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if gPlayer[i].stat.resonupgrade[j]<>0 then
      [*] = ;      mask1 := mask1 or (1 shl (gc_MaxPlayerCount+cCount+j));
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if gPlayer[i].stat.resonmines[j]<>0 then
      [*] = ;      mask1 := mask1 or (1 shl (gc_MaxPlayerCount+cCount*2+j));
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if gPlayer[i].stat.resonunits[j]<>0 then
      [*] = ;      mask2 := mask2 or (1 shl j);
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if gPlayer[i].stat.resonbuildings[j]<>0 then
      [*] = ;      mask2 := mask2 or (1 shl (cCount+j));
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if gPlayer[i].stat.resonlife[j]<>0 then
      [*] = ;      mask2 := mask2 or (1 shl (cCount*2+j));
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if gPlayer[i].stat.resbuy[j]<>0 then
      [*] = ;      mask2 := mask2 or (1 shl (cCount*3+j));
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if gPlayer[i].stat.ressell[j]<>0 then
      [*] = ;      mask2 := mask2 or (1 shl (cCount*4+j));
      [*] = ;   end;
      [*] = ;   RecordCustomWriteInteger(mask1);
      [*] = ;   RecordCustomWriteInteger(mask2);
      [*] = ;
      [*] = ;   for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;   if ((mask1 and (1 shl i))<>0) then
      [*] = ;   begin
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if (mask1 and (1 shl (gc_MaxPlayerCount+j))<>0) then
      [*] = ;      RecordCustomWriteInteger(gPlayer[i].stat.restotal[j]);
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if (mask1 and (1 shl (gc_MaxPlayerCount+cCount+j))<>0) then
      [*] = ;      RecordCustomWriteInteger(gPlayer[i].stat.resonupgrade[j]);
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if (mask1 and (1 shl (gc_MaxPlayerCount+cCount*2+j))<>0) then
      [*] = ;      RecordCustomWriteInteger(gPlayer[i].stat.resonmines[j]);
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if (mask2 and (1 shl j)<>0) then
      [*] = ;      RecordCustomWriteInteger(gPlayer[i].stat.resonunits[j]);
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if (mask2 and (1 shl (cCount*1+j))<>0) then
      [*] = ;      RecordCustomWriteInteger(gPlayer[i].stat.resonbuildings[j]);
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if (mask2 and (1 shl (cCount*2+j))<>0) then
      [*] = ;      RecordCustomWriteInteger(gPlayer[i].stat.resonlife[j]);
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if (mask2 and (1 shl (cCount*3+j))<>0) then
      [*] = ;      RecordCustomWriteInteger(gPlayer[i].stat.resbuy[j]);
      [*] = ;
      [*] = ;      for j:=gc_resource_type_food to gc_ResCount-1 do
      [*] = ;      if (mask2 and (1 shl (cCount*4+j))<>0) then
      [*] = ;      RecordCustomWriteInteger(gPlayer[i].stat.ressell[j]);
      [*] = ;   end;
      [*] = ;
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;   _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

