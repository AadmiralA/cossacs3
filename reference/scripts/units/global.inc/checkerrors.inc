section.begin
   Name = CheckErrors
   Code : struct.begin
      [*] = ;var plHnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plInd: Integer = GetPlayerIndexByHandle(plHnd);
      [*] = ;
      [*] = ;procedure CheckList(plInd : Integer; var list : TIntegerList; listName : String);
      [*] = ;begin
      [*] = ;   var i, j : Integer;
      [*] = ;   var count : Integer = list.GetCount;
      [*] = ;   for i := 0 to count-1 do
      [*] = ;   begin
      [*] = ;      var goHnd : Integer = list.Get(i);
      [*] = ;      if goHnd = 0 then
      [*] = ;      begin
      [*] = ;         ErrorLog('Player [' + IntToStr(plInd) + '].aiData.' + listName + '[' + IntToStr(i)+ '] = 0');
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         if not IsGameObjectByHandle(goHnd) then
      [*] = ;         begin
      [*] = ;            ErrorLog('Player [' + IntToStr(plInd) + '] has wrong game object [' + IntToStr(i) +
      [*] = ;            '] in aiData.' + listName + ' list (hnd = ' + IntToStr(goHnd) + ')');
      [*] = ;            SetProgressingPauseMode(true);
      [*] = ;            //var uid : Integer = GetGameObjectUniqueIdByHandle(goHnd);
      [*] = ;            //Log('UID = ' + IntToStr(uid));
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;            var customName : String = GetGameObjectCustomNameByHandle(goHnd);
      [*] = ;            if TObj(pObj).squad >= 0 then
      [*] = ;            begin
      [*] = ;               var err : Boolean = true;
      [*] = ;               var pSquad : Pointer = gPlayer[plInd].squads.Get(TObj(pObj).squad);
      [*] = ;               if TSquad(pSquad).fArmy >= 0 then
      [*] = ;               begin
      [*] = ;                  var pArmy : Pointer = gPlayer[plInd].aiData.armyList.Get(TSquad(pSquad).fArmy);
      [*] = ;                  if TArmy(pArmy).fSpec = gc_ai_armyspec_squad then
      [*] = ;                  err := false;
      [*] = ;               end;
      [*] = ;
      [*] = ;               if err then
      [*] = ;               ErrorLog('Player [' + IntToStr(plInd) + '].aiData.' + listName + '[' + IntToStr(i) + '] is in squad (customName = ' + customName + ')');
      [*] = ;            end;
      [*] = ;
      [*] = ;            for j := list.GetCount-1 downto i+1 do
      [*] = ;            begin
      [*] = ;               var goHnd1 : Integer = list.Get(j);
      [*] = ;               if goHnd = goHnd1 then
      [*] = ;               begin
      [*] = ;                  ErrorLog('Player [' + IntToStr(plInd) + '].aiData.' + listName + '[' + IntToStr(i) + '] is duplicated in [' +
      [*] = ;                  IntToStr(j) + '] (hnd = ' + IntToStr(goHnd) + ')');
      [*] = ;                  SetProgressingPauseMode(true);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;var i, j, k : Integer;
      [*] = ;
      [*] = ;for i := GetPlayerGameObjectsCountByHandle(plHnd)-1 downto 0 do
      [*] = ;begin
      [*] = ;   var goHnd : Integer = GetGameObjectHandleByIndex(i, plHnd);
      [*] = ;   var customName : String = GetGameObjectCustomNameByHandle(goHnd);
      [*] = ;   var pObj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;   var pOrdList : Pointer = _misc_GetObjectArgData(goHnd, gc_argunit_ordlist);
      [*] = ;   var pSTOList : Pointer = _misc_GetObjectArgData(goHnd, gc_argunit_stolist);
      [*] = ;
      [*] = ;   var trgHnd : Integer = GetGameObjectSTOHandleByHandle(goHnd);
      [*] = ;   if trgHnd <> 0 then
      [*] = ;   begin
      [*] = ;      var tCustomName : String = GetGameObjectCustomNameByHandle(trgHnd);
      [*] = ;      var pTrgSTOList : Pointer = _misc_GetObjectArgData(trgHnd, gc_argunit_stolist);
      [*] = ;      if pTrgSTOList <> nil then
      [*] = ;      begin
      [*] = ;         var count : Integer;
      [*] = ;         for j := TIntegerList(pTrgSTOList).GetCount-1 downto 0 do
      [*] = ;         if TIntegerList(pTrgSTOList).Get(j) = goHnd then
      [*] = ;         count := count + 1;
      [*] = ;
      [*] = ;         if count < 0 then
      [*] = ;         ErrorLog(tCustomName + ' is a target for ' + customName + ' but doesn''t have it in STO list')
      [*] = ;         else
      [*] = ;         if count > 1 then
      [*] = ;         ErrorLog(tCustomName + ' is a target for ' + customName + ' and has it STO list more than 1 time');
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         ErrorLog(tCustomName + ' STO list = nil');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if pStoList <> nil then
      [*] = ;   begin
      [*] = ;      for j := 0 to TIntegerList(pSTOList).GetCount-1 do
      [*] = ;      begin
      [*] = ;         var trgHnd : Integer = TIntegerList(pSTOList).Get(j);
      [*] = ;         var tCustomName : String = GetGameObjectCustomNameByHandle(trgHnd);
      [*] = ;         if GetGameObjectSTOHandleByHandle(trgHnd) <> goHnd then
      [*] = ;         begin
      [*] = ;            ErrorLog(customName + ' is not a target for ' + tCustomName);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if pOrdList <> nil then
      [*] = ;   begin
      [*] = ;      for j := 0 to TIntegerList(pOrdList).GetCount-1 do
      [*] = ;      begin
      [*] = ;         var trgHnd : Integer = TIntegerList(pOrdList).Get(j);
      [*] = ;         var tCustomName : String = GetGameObjectCustomNameByHandle(trgHnd);
      [*] = ;         var pTrgObj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;         var count : Integer = 0;
      [*] = ;         for k := 0 to TIntegerList(pOrdList).GetCount-1 do
      [*] = ;         if TIntegerList(pOrdList).Get(k) = trgHnd then
      [*] = ;         count := count+1;
      [*] = ;
      [*] = ;         var ordCount : Integer;
      [*] = ;         for k := 0 to gc_obj_MaxOrderCount-1 do
      [*] = ;         if TObj(pTrgObj).orders[k].info.trg = goHnd then
      [*] = ;         ordCount := ordCount+1;
      [*] = ;
      [*] = ;         if count <> ordCount then
      [*] = ;         begin
      [*] = ;            ErrorLog(customName + ' count of ' + tCustomName + ' in order list = ' + IntToStr(count) +
      [*] = ;            ' order count = ' + IntToStr(ordCount));
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   for j := 0 to gc_obj_MaxOrderCount-1 do
      [*] = ;   begin
      [*] = ;      var pOrder : Pointer = TObj(pObj).orders[j];
      [*] = ;      if TOrder(pOrder).iType = gc_obj_order_type_attackobj then
      [*] = ;      begin
      [*] = ;         var trgHnd : Integer = TOrder(pOrder).info.trg;
      [*] = ;         var plTrgHnd : Integer = GetGameObjectPlayerHandleByHandle(trgHnd);
      [*] = ;         if (trgHnd <> 0) then
      [*] = ;         begin
      [*] = ;            var pTrgObj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;            if (pTrgObj <> nil) then
      [*] = ;            begin
      [*] = ;               var tCustomName : String = GetGameObjectCustomNameByHandle(trgHnd);
      [*] = ;               var inList : Boolean;
      [*] = ;               var dead : Boolean = TObj(pTrgObj).bDead;
      [*] = ;               var pOrdList : Pointer = _misc_GetObjectArgData(trgHnd, gc_argunit_ordlist);
      [*] = ;               if pOrdList <> nil then
      [*] = ;               begin
      [*] = ;                  for k := TIntegerList(pOrdList).GetCount-1 downto 0 do
      [*] = ;                  begin
      [*] = ;                     if TIntegerList(pOrdList).Get(k) = goHnd then
      [*] = ;                     begin
      [*] = ;                        inList := true;
      [*] = ;                        break;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;
      [*] = ;               if not inList then
      [*] = ;               begin
      [*] = ;                  ErrorLog(tCustomName + ' doesn''t have ' + customName + ' in order list');
      [*] = ;                  SetProgressingPauseMode(true);
      [*] = ;               end;
      [*] = ;
      [*] = ;               if dead then
      [*] = ;               ErrorLog(customName + ' has dead target ' + tCustomName);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if gPlayer[plInd].bai then
      [*] = ;begin
      [*] = ;   CheckList(plInd, gPlayer[plInd].aiData.agressors, 'agressors');
      [*] = ;   CheckList(plInd, gPlayer[plInd].aiData.defenders, 'defenders');
      [*] = ;   CheckList(plInd, gPlayer[plInd].aiData.guards, 'guards');
      [*] = ;   CheckList(plInd, gPlayer[plInd].aiData.freeWarriors, 'freeWarriors');
      [*] = ;
      [*] = ;   // check guards list
      [*] = ;   for i := gPlayer[plInd].aiData.guards.GetCount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var goHnd : Integer = gPlayer[plInd].aiData.guards.Get(i);
      [*] = ;      if (goHnd <> 0) and IsGameObjectByHandle(goHnd) then
      [*] = ;      begin
      [*] = ;         var customName : String = GetGameObjectCustomNameByHandle(goHnd);
      [*] = ;         for j := gPlayer[plInd].aiData.defenders.GetCount-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var goHnd1 : Integer = gPlayer[plInd].aiData.defenders.Get(j);
      [*] = ;            if goHnd = goHnd1 then
      [*] = ;            begin
      [*] = ;               ErrorLog('Player [' + IntToStr(plInd) + '] has game object [' + IntToStr(i) +
      [*] = ;               '] in aiData.guards list and in aiData.defenders list (hnd = ' + IntToStr(goHnd) + '), customName = ' + customName);
      [*] = ;               SetProgressingPauseMode(true);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   for i := 0 to gPlayer[plInd].aiData.armyList.GetCount-1 do
      [*] = ;   begin
      [*] = ;      var pArmy : Pointer = gPlayer[plInd].aiData.armyList.Get(i);
      [*] = ;      if TArmy(pArmy).fSquadList.GetCount <= 0 then
      [*] = ;      begin
      [*] = ;         ErrorLog('Player [' + IntToStr(plInd) + '] army [' + IntToStr(i) + '] has 0 squads');
      [*] = ;      end
      [*] = ;   end;
      [*] = ;
      [*] = ;   for i := 0 to gPlayer[plInd].squads.GetCount-1 do
      [*] = ;   begin
      [*] = ;      var pSquad : Pointer = gPlayer[plInd].squads.Get(i);
      [*] = ;      var goCount : Integer = TSquad(pSquad).GetCount;
      [*] = ;      if goCount <= 0 then
      [*] = ;      ErrorLog('Player [' + IntToStr(plInd) + '] squad [' + IntToStr(i) + '].count = ' + IntToStr(goCount));
      [*] = ;
      [*] = ;      if TSquad(pSquad).fArmy < 0 then
      [*] = ;      begin
      [*] = ;         ErrorLog('Player [' + IntToStr(plInd) + '] squad [' + IntToStr(i) + '] is not in army');
      [*] = ;         SetProgressingPauseMode(true);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   var goCount : Integer = GetPlayerGameObjectsCountByHandle(plHnd);
      [*] = ;   for i := 0 to goCount-1 do
      [*] = ;   begin
      [*] = ;      var goHnd : Integer = GetGameObjectHandleByIndex(i, plHnd);
      [*] = ;      var vis : Boolean = GetGameObjectVisibleByHandle(goHnd);
      [*] = ;      var tag : Integer = GetGameObjectStatesTagByHandle(goHnd);
      [*] = ;      if (tag and gc_statetag_visual_hide <> 0) and (tag and gc_statetag_essential_none <> 0) then
      [*] = ;      begin
      [*] = ;         var customName : String = GetGameObjectCustomNameByHandle(goHnd);
      [*] = ;         ErrorLog('Player [' + IntToStr(plInd) + '] has invisible unit [' + IntToStr(i) + '], customName = ' + customName);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

