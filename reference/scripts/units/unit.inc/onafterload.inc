section.begin
   Name = OnAfterLoad
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;var myPlHnd : Integer = GetGameObjectPlayerHandleByHandle(myHnd);
      [*] = ;var myPlInd : Integer = GetPlayerIndexByHandle(myPlHnd);
      [*] = ;
      [*] = ;if _unit_IsWaterUnit(myHnd) then
      [*] = ;_unit_CreateShipChilds(myHnd);
      [*] = ;
      [*] = ;if (arg_obj.bbuilt) then // not shipdummy
      [*] = ;begin
      [*] = ;   if (gObjProp[arg_obj.cid][arg_obj.id].media = gc_obj_media_water) and (not arg_obj.bDead) then
      [*] = ;   begin
      [*] = ;      if arg_obj.squad < 0 then
      [*] = ;      begin
      [*] = ;         gPlayer[myPlInd].aiData.aiList.Clear;
      [*] = ;         gPlayer[myPlInd].aiData.aiList.Add(myHnd);
      [*] = ;         _player_WriteSquadNew(myPlHnd, -1, 0, 0, gPlayer[myPlInd].aiData.aiList, false, false);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end