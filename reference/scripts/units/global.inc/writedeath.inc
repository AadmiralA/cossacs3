section.begin
   Name = WriteDeath
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var mode: Integer = arg_plr.fmode;
      [*] = ;var randkey: Float = arg_plr.frnd;
      [*] = ;var list: Pointer = arg_plr.fintlst;
      [*] = ;var count: Integer;
      [*] = ;if list<>nil then
      [*] = ;count:=TIntegerList(list).GetCount
      [*] = ;else count:=0;
      [*] = ;
      [*] = ;if _net_IsServer or _net_IsOffline then begin
      [*] = ;   var prevrandkey : Float = GetRandomKey;
      [*] = ;   SetRandomKey(floor(Abs(randkey)));
      [*] = ;   var i : Integer;
      [*] = ;   var pass : Integer;
      [*] = ;   var maxpass : Integer;
      [*] = ;   if (mode=0) then maxpass := 1;
      [*] = ;   for pass:=0 to maxpass do // first pass for random damage building hp to sync dmg through net
      [*] = ;   for i:=0 to TIntegerList(list).GetCount-1 do
      [*] = ;   begin
      [*] = ;      var hnd : Integer = TIntegerList(list).Get(i);
      [*] = ;      var usage : Integer = _unit_GetUsage(hnd);
      [*] = ;      if (mode=1) then
      [*] = ;      begin
      [*] = ;         if (gint_versionid=gc_versionid_dev) or (gProfile.bCheatPlaceUnits) then
      [*] = ;         begin
      [*] = ;            var pobj : Pointer = _unit_GetTObj(hnd);
      [*] = ;            if (pobj<>nil) then
      [*] = ;            begin
      [*] = ;               var pobjprop : Pointer = _unit_GetObjProp(hnd);
      [*] = ;               if (pobjprop<>nil) and (TObjProp(pobjprop).bgate) then
      [*] = ;               _misc_OpenCloseGates(hnd, true, true);
      [*] = ;            end;
      [*] = ;            if (usage=gc_obj_usage_hardwall) or (usage=gc_obj_usage_weakwall) then
      [*] = ;            _misc_DoWallDamage(hnd, gc_wall_damage_cdel)
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               EffectHighlightRemove(hnd, ''); // execute before EffectClear
      [*] = ;               EffectClear(hnd); // execute after EffectHighlightRemove
      [*] = ;               GameObjectRequestToDestroyByHandle(hnd);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         var pobj : Pointer = _unit_GetTObj(hnd);
      [*] = ;         if (pobj<>nil) then
      [*] = ;         begin
      [*] = ;            var pobjprop : Pointer = _unit_GetObjProp(hnd);
      [*] = ;            if (TObjProp(pobjprop).bbuilding) and (TObjProp(pobjprop).bslowdeath) and (TObj(pobj).bbuilt) then
      [*] = ;            begin
      [*] = ;               if (pass=0) and (TObj(pobj).hp>gc_BuildingSlowDeathHP) then
      [*] = ;               begin
      [*] = ;                  TObj(pobj).hp := gc_BuildingSlowDeathHP-floor(RandomExt*gc_BuildingSlowDeathRandom);
      [*] = ;                  var uid : Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;                  if (gPlayer[TObj(pobj).pl].lists.deluids.IndexOf(uid)<0) then
      [*] = ;                  gPlayer[TObj(pobj).pl].lists.deluids.Add(uid);
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               if (pass=1) then
      [*] = ;               begin
      [*] = ;                  if (TObjProp(pobjprop).bbuilding) or ((GetGameObjectStatesTagByHandle(hnd) and gc_statetag_essential_none)<>0) then
      [*] = ;                  _unit_SetTagStates(hnd, gc_statetag_essential_death);
      [*] = ;                  if (usage=gc_obj_usage_hardwall) or (usage=gc_obj_usage_weakwall) then
      [*] = ;                  _misc_DoWallDamage(hnd, gc_wall_damage_del);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   SetRandomKey(floor(Abs(prevrandkey)));
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_IsOnline or _net_IsRecord then begin
      [*] = ;   // create package
      [*] = ;   if not RecordCustomBegin('ReadDeath') then
      [*] = ;   _net_ErrorLog('RecordCustomBegin(''ReadDeath'')');
      [*] = ;
      [*] = ;   // write data to package
      [*] = ;   RecordCustomWriteBoolean(_net_IsServer or _net_IsOffline);
      [*] = ;   RecordCustomWriteInteger(mode);
      [*] = ;   RecordCustomWriteFloat(randkey);
      [*] = ;   RecordCustomWriteInteger(count);
      [*] = ;   var i: Integer;
      [*] = ;   for i:=0 to count-1 do begin
      [*] = ;      var hnd: Integer = TIntegerList(list).Get(i);
      [*] = ;      var uid: Integer = GetGameObjectUniqueIdByHandle(hnd);
      [*] = ;      RecordCustomWriteInteger(uid);
      [*] = ;   end;
      [*] = ;
      [*] = ;   if _net_debug_log_death then
      [*] = ;   _net_Log('writedeath(count='+IntToStr(count)+')');
      [*] = ;
      [*] = ;   if _net_debug_log_size_writer then
      [*] = ;   _net_Log_SizeWriter;
      [*] = ;
      [*] = ;   // finish package
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

