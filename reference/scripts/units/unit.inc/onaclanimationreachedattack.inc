section.begin
   Name = OnAclAnimationReachedAttack
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;if (not arg_obj.bdead) then
      [*] = ;begin
      [*] = ;   var bProcess : Boolean = not (_net_IsClient or _net_IsReplay);
      [*] = ;   if (not bProcess) or ((arg_obj.orders[0].itype=gc_obj_order_type_attackobj) or (arg_obj.orders[0].itype=gc_obj_order_type_attackpoint)) and (arg_obj.attackdelay=0) then
      [*] = ;   begin
      [*] = ;      var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;      var statetag : Integer = GetGameObjectStatesTagByHandle(myHnd);
      [*] = ;      if ((statetag and gc_statetag_action_attack)<>0) then
      [*] = ;      begin
      [*] = ;         var weapind : Integer;
      [*] = ;         case (statetag and gc_statetag_weapon) of
      [*] = ;            gc_statetag_weapon_none : weapind := 0;
      [*] = ;            gc_statetag_weapon_0 : weapind := 0;
      [*] = ;            gc_statetag_weapon_1 : weapind := 1;
      [*] = ;            gc_statetag_weapon_2 : weapind := 2;
      [*] = ;         end;
      [*] = ;         var pobjbase : Pointer = gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id];
      [*] = ;         var pobjprop : Pointer = gObjProp[arg_obj.cid][arg_obj.id];
      [*] = ;         var i : Integer;
      [*] = ;         if (bProcess) then
      [*] = ;         _unit_ApplyWeaponCost(myHnd, weapInd);
      [*] = ;
      [*] = ;         _unit_ApplyAttackPause(myHnd, weapind);
      [*] = ;         var weaponid : Integer = TObjProp(pobjprop).weapon[weapind].weaponid;
      [*] = ;         var trgHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;         if (trgHnd<>0) then
      [*] = ;         begin
      [*] = ;            var pobj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;            if (pobj<>nil) then
      [*] = ;            begin
      [*] = ;               if (weaponid=0) then
      [*] = ;               begin
      [*] = ;                  if (gSoundManager.bprocess) then
      [*] = ;                  begin
      [*] = ;                     var sndind : Integer;
      [*] = ;                     case TObjProp(pobjprop).weapon[weapind].kind of
      [*] = ;                        gc_obj_weapon_kind_sword : sndind := gc_snd_ind_sabre;
      [*] = ;                        gc_obj_weapon_kind_pike : begin
      [*] = ;                           if (TObjProp(pobjprop).usage=gc_obj_usage_fasthorse) or (TObjProp(pobjprop).usage=gc_obj_usage_hardhorse) then
      [*] = ;                           sndind := gc_snd_ind_sabre
      [*] = ;                           else
      [*] = ;                           sndind := gc_snd_ind_pike;
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                     if (sndind<>0) then
      [*] = ;                     _unit_RequestPlaySound(myHnd, sndind);
      [*] = ;                  end;
      [*] = ;
      [*] = ;                  _misc_DoDamage(myHnd, trgHnd, TObjBase(pobjbase).weapon[weapind].damage, weapind, TObjProp(pobjprop).weapon[weapind].kind)
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  if (gWeapons[weaponid].propagation=gc_weapon_propagation_immediate) then
      [*] = ;                  begin
      [*] = ;                     _misc_DoDamage(myHnd, trgHnd, TObjBase(pobjbase).weapon[weapind].damage, weapind, TObjProp(pobjprop).weapon[weapind].kind);
      [*] = ;                     // Optimization for shotfog
      [*] = ;                     if (gSoundManager.bprocess) then
      [*] = ;                     _unit_RequestPlaySound(myHnd, gWeapons[weaponid].sndind);
      [*] = ;
      [*] = ;                     if ((gWeapons[weaponid].volumeclippedfreq<=0) or (gWeapons[weaponid].volumeclippedfreq<random) or (gSoundManager.IsObjInFrustum(myHnd))) then // I use general random, cause no need to sync it and randomext may change planned results on dif PCs
      [*] = ;                     begin
      [*] = ;                        var fxshot : String;
      [*] = ;                        var pobjweapon : Pointer = gObjProp[arg_obj.cid][arg_obj.id].weapon[weapind];
      [*] = ;                        if (pobjweapon<>nil) then
      [*] = ;                        fxshot := TObjWeaponStatic(pobjweapon).fxshot;
      [*] = ;                        var ind : Integer = -1;
      [*] = ;                        if (fxshot<>'') then
      [*] = ;                        ind := gEffectLibrary.IndexOf(fxshot);
      [*] = ;                        if (ind>=0) then
      [*] = ;                        begin
      [*] = ;                           //var projHnd : Integer = CreatePlayerGameObjectHandleByHandle(GetPlayerHandleByIndex(gc_playerind_misc), gc_racename_misc, gWeapons[weaponid].projbasename, GetGameObjectMyPositionX, GetGameObjectMyPositionY, GetGameObjectMyPositionZ);
      [*] = ;                           var projHnd : Integer = CreatePoolGameObjectHandle(gc_racename_misc, gWeapons[weaponid].projbasename, GetGameObjectMyPositionX, GetGameObjectMyPositionY, GetGameObjectMyPositionZ);
      [*] = ;                           //BehaviourClearRequested(projHnd);
      [*] = ;                           GameObjectExecuteStateByHandle(projHnd, 'InitialEmpty');
      [*] = ;                           var upx, upy, upz, dirx, diry, dirz : Float;
      [*] = ;                           GetGameObjectOrientationByHandle(myHnd, upx, upy, upz, dirx, diry, dirz);
      [*] = ;                           SetGameObjectOrientationByHandle(projHnd, upx, upy, upz, dirx, diry, dirz);
      [*] = ;                           SetGameObjectScaleByHandle(projHnd, 0.00001, 0.00001, 0.00001);
      [*] = ;
      [*] = ;                           var p: Pointer = gEffectLibrary.fList.GetPointer(ind);
      [*] = ;                           TLibEffects(p).GetOrCreate(projHnd);
      [*] = ;                        end;
      [*] = ;                     end;
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  if (bProcess) then
      [*] = ;                  begin
      [*] = ;                     var sx, sy, sz : Float;
      [*] = ;                     var tx, ty, tz : Float;
      [*] = ;                     _weapon_GetUnitShotStartPosition(myHnd, sx, sy, sz);
      [*] = ;                     _weapon_GetUnitShotEndPosition(trgHnd, tx, ty, tz);
      [*] = ;                     var bAllowShot : Boolean = _weapon_PredictShot(myHnd, weaponid, sx, sy, sz, tx, ty, tz);
      [*] = ;                     if (bAllowShot) then
      [*] = ;                     begin
      [*] = ;                        var trg : Integer;
      [*] = ;                        if (gWeapons[weaponid].buseownerdamage) and (_unit_IsBuilding(trgHnd)) then // arrow, firearrow, grenade
      [*] = ;                        trg := trgHnd;
      [*] = ;                        _unit_DoProjectile(myHnd, trg, False, sx, sy, sz, tx, ty, tz, weapind);
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     begin
      [*] = ;                        arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                        arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            ErrorLog('pobj=nil in OnAclAnimationReachedAttack');
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (arg_obj.orders[0].itype=gc_obj_order_type_attackpoint) then
      [*] = ;         begin
      [*] = ;            if (bProcess) then
      [*] = ;            begin
      [*] = ;               if (weaponid<>0) then
      [*] = ;               begin
      [*] = ;                  var sx, sy, sz : Float;
      [*] = ;                  _weapon_GetUnitShotStartPosition(myHnd, sx, sy, sz);
      [*] = ;                  var tx : Float = arg_obj.orders[0].info.dx;
      [*] = ;                  var tz : Float = arg_obj.orders[0].info.dy;
      [*] = ;                  var ty : Float;
      [*] = ;                  if (not RayCastWater(tx, tz, ty)) then // is water, then ty is water height
      [*] = ;                  ty := RayCastHeight(tx, tz);
      [*] = ;                  var bAllowShot : Boolean = _weapon_PredictShot(myHnd, weaponid, sx, sy, sz, tx, ty, tz);
      [*] = ;                  if (bAllowShot) then
      [*] = ;                  _unit_DoProjectile(myHnd, 0, False, sx, sy, sz, tx, ty, tz, weapind)
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;                     arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               ErrorLog('OnAclAnimationReachedAttack : trgHnd=0 and weaponid=0');
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            arg_obj.attackdelay := MaxFloat(arg_obj.attackdelay, 1);
      [*] = ;            arg_obj.attackmaxdelay := MaxFloat(arg_obj.attackmaxdelay, 1);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

