section.begin
   Name = Nothing
   UsedFileName = 
   Code : struct.begin
      [*] = ;args arg_obj: TObj;
      [*] = ;
      [*] = ;if gMovie.blockunits then begin
      [*] = ;   _unit_SetTagStates(GetGameObjectMyHandle, gc_statetag_move_idle or gc_statetag_action_none);
      [*] = ;   arg_obj.orders[0].bexecute := False;
      [*] = ;   exit;
      [*] = ;end;
      [*] = ;
      [*] = ;if not arg_obj.bbuilt then exit;
      [*] = ;
      [*] = ;var gametime : Float = GetGameTime;
      [*] = ;var gametimewithrnd : Float = gametime+arg_obj.uniqrnd;
      [*] = ;var deltatime : Float = gametime-arg_obj.lastprogresstime;
      [*] = ;var progresstick : Integer = arg_obj.progresstick;
      [*] = ;var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;
      [*] = ;//_misc_ProfilerBegin('unit.nothing');
      [*] = ;
      [*] = ;var pobjprop : Pointer = gObjProp[arg_obj.cid][arg_obj.id];
      [*] = ;var statetag : Integer = GetGameObjectMyStatesTag;
      [*] = ;
      [*] = ;if (not arg_obj.bdead) then
      [*] = ;begin
      [*] = ;   var bplayable : Boolean = GetGameObjectMyPlayableObject;
      [*] = ;   if (bplayable) or (arg_obj.sndwalkproc) then // comment that line, or units will produce walk sound after they came to mine or transport forever
      [*] = ;   begin
      [*] = ;      if (arg_obj.sndwalktype=0) then
      [*] = ;      begin
      [*] = ;         case TObjProp(pobjprop).usage of
      [*] = ;            gc_obj_usage_peasant : begin
      [*] = ;               if ((statetag and gc_statetag_resource)=gc_statetag_resource_stone) then
      [*] = ;               arg_obj.sndwalktype := 8
      [*] = ;               else
      [*] = ;               arg_obj.sndwalktype := 9
      [*] = ;            end;
      [*] = ;            gc_obj_usage_fasthorse : arg_obj.sndwalktype := 5;
      [*] = ;            gc_obj_usage_hardhorse, gc_obj_usage_horseshooter : arg_obj.sndwalktype := 6;
      [*] = ;            gc_obj_usage_cannon, gc_obj_usage_mortar, gc_obj_usage_mcannon : arg_obj.sndwalktype := 3;
      [*] = ;            gc_obj_usage_supermortar : arg_obj.sndwalktype := 4;
      [*] = ;            else
      [*] = ;            begin
      [*] = ;               if (_unit_IsWaterUnit(myHnd)) then
      [*] = ;               begin
      [*] = ;                  // to do walk ships
      [*] = ;               end
      [*] = ;               else
      [*] = ;               if (gPlayer[arg_obj.pl].objbase[arg_obj.cid][arg_obj.id].protection[gc_obj_weapon_kind_cannister]>0) then
      [*] = ;               arg_obj.sndwalktype := 2
      [*] = ;               else
      [*] = ;               arg_obj.sndwalktype := 1;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      if (gSoundManager.bprocess) and (statetag and gc_statetag_move_walk<>0) then
      [*] = ;      begin
      [*] = ;         case arg_obj.sndwalktype of
      [*] = ;            1 : gSoundManager.AddObjIfInFrustum(myHnd, gSoundManager.counter.walkinfcloth, gSoundManager.counter.walkdstinfcloth);
      [*] = ;            2 : gSoundManager.AddObjIfInFrustum(myHnd, gSoundManager.counter.walkinfmetal, gSoundManager.counter.walkdstinfmetal);
      [*] = ;            3 : gSoundManager.AddObjIfInFrustum(myHnd, gSoundManager.counter.walkartwheels, gSoundManager.counter.walkdstartwheels);
      [*] = ;            4 : gSoundManager.AddObjIfInFrustum(myHnd, gSoundManager.counter.walkartmortar, gSoundManager.counter.walkdstartmortar);
      [*] = ;            5 : gSoundManager.AddObjIfInFrustum(myHnd, gSoundManager.counter.walkcavfast, gSoundManager.counter.walkdstcavfast);
      [*] = ;            6 : gSoundManager.AddObjIfInFrustum(myHnd, gSoundManager.counter.walkcavslow, gSoundManager.counter.walkdstcavslow);
      [*] = ;            7 : gSoundManager.AddObjIfInFrustum(myHnd, gSoundManager.counter.walkinfcloth, gSoundManager.counter.walkdstinfcloth);
      [*] = ;            8 : gSoundManager.AddObjIfInFrustum(myHnd, gSoundManager.counter.walkpeastone, gSoundManager.counter.walkdstpeastone);
      [*] = ;            9 : gSoundManager.AddObjIfInFrustum(myHnd, gSoundManager.counter.walkpea, gSoundManager.counter.walkdstpea);
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (arg_obj.sndwalkproc) then
      [*] = ;      begin
      [*] = ;         case arg_obj.sndwalktype of
      [*] = ;            1 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkinfcloth, gSoundManager.counter.walkdstinfcloth);
      [*] = ;            2 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkinfmetal, gSoundManager.counter.walkdstinfmetal);
      [*] = ;            3 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkartwheels, gSoundManager.counter.walkdstartwheels);
      [*] = ;            4 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkartmortar, gSoundManager.counter.walkdstartmortar);
      [*] = ;            5 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkcavfast, gSoundManager.counter.walkdstcavfast);
      [*] = ;            6 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkcavslow, gSoundManager.counter.walkdstcavslow);
      [*] = ;            7 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkinfcloth, gSoundManager.counter.walkdstinfcloth);
      [*] = ;            8 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkpeastone, gSoundManager.counter.walkdstpeastone);
      [*] = ;            9 : gSoundManager.RemoveObj(myHnd, gSoundManager.counter.walkpea, gSoundManager.counter.walkdstpea);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      if (arg_obj.attackdelay>0) then
      [*] = ;      begin
      [*] = ;         arg_obj.attackdelay := arg_obj.attackdelay-deltatime;
      [*] = ;         if (arg_obj.attackdelay<0) then
      [*] = ;         arg_obj.attackdelay := 0;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_IsClient or _net_IsReplay then
      [*] = ;begin
      [*] = ;   if (not arg_obj.bdead) then
      [*] = ;   begin
      [*] = ;      var bplayable : Boolean = GetGameObjectMyPlayableObject;
      [*] = ;      if (bplayable) then
      [*] = ;      begin
      [*] = ;         if gametime>arg_obj.lasttimescangrid then
      [*] = ;         begin
      [*] = ;            arg_obj.lasttimescangrid := gametimewithrnd{arg_obj.lasttimescangrid}+gc_unit_TimeScanGrid;
      [*] = ;            _unit_ScanGridProgress(myHnd);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      arg_obj.lastprogresstime := gametime;
      [*] = ;      arg_obj.progresstick := progresstick+1;
      [*] = ;   end;
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   if (not arg_obj.bdead) then
      [*] = ;   begin
      [*] = ;      if (arg_obj.id>0) and (arg_obj.hp <= 0) then
      [*] = ;      begin
      [*] = ;         ExecuteState('OnDeath');
      [*] = ;         _unit_SetTagStates(myHnd, gc_statetag_essential_death);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         var plHnd : Integer = GetGameObjectPlayerHandleByHandle(myHnd);
      [*] = ;         var plInd : Integer = arg_obj.pl;
      [*] = ;         var bplayable : Boolean = GetGameObjectMyPlayableObject;
      [*] = ;
      [*] = ;         if gametime>arg_obj.lasttimetopology then
      [*] = ;         begin
      [*] = ;            arg_obj.lasttimetopology := gametimewithrnd+gc_unit_TimeTopology;
      [*] = ;            _unit_TopologyProgress(myHnd);
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (bplayable) then
      [*] = ;         begin
      [*] = ;            if gametime>arg_obj.lasttimescangrid then
      [*] = ;            begin
      [*] = ;               arg_obj.lasttimescangrid := gametimewithrnd+gc_unit_TimeScanGrid;
      [*] = ;               _unit_ScanGridProgress(myHnd);
      [*] = ;            end;
      [*] = ;            if gametime>arg_obj.lasttimeidlegrid then
      [*] = ;            begin
      [*] = ;               arg_obj.lasttimeidlegrid := gametimewithrnd+gc_unit_TimeIdleGrid;
      [*] = ;               _unit_IdleGridProgress(myHnd);
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (statetag and gc_statetag_essential_birth<>0) and (statetag and gc_statetag_visual_none<>0) then
      [*] = ;         if ((arg_obj.orders[0].itype<gc_obj_order_type_leavetransport) and (arg_obj.orders[0].itype>gc_obj_order_type_leavebuilding)) or (arg_obj.standtime>1) then
      [*] = ;         begin
      [*] = ;            _unit_SetTagStates(myHnd, gc_statetag_essential_none);
      [*] = ;            ErrorLog('unit:nothing: Possible hang unit was fixed to essential_none');
      [*] = ;            exit;
      [*] = ;         end;
      [*] = ;
      [*] = ;         //_misc_ProfilerBegin('Nothing_SV');
      [*] = ;         var bIdleAction : Boolean = ((statetag and gc_statetag_action_none)<>0) and ((statetag and gc_statetag_execute_none)<>0);
      [*] = ;         var bIdleMove : Boolean = ((statetag and gc_statetag_move_walk)=0);
      [*] = ;         if (bIdleAction) and (bIdleMove) then
      [*] = ;         begin
      [*] = ;            if (arg_obj.standtime>0.002) or ((statetag and gc_statetag_move_idle)<>0) then
      [*] = ;            arg_obj.standtime := arg_obj.standtime+deltatime
      [*] = ;            else
      [*] = ;            arg_obj.standtime := 0.001;
      [*] = ;         end;
      [*] = ;
      [*] = ;         if (arg_obj.exitDelay>0) then
      [*] = ;         arg_obj.exitDelay := _misc_MaxFloat(arg_obj.exitDelay-deltatime, 0);
      [*] = ;
      [*] = ;         var ordertype : Integer = arg_obj.orders[0].itype;
      [*] = ;         var searchdeltatime : Float = gametime-arg_obj.lastsearchenemy;
      [*] = ;         if (searchdeltatime<-0.45) and (ordertype<=gc_obj_order_type_move) then
      [*] = ;         searchdeltatime := 0;
      [*] = ;         if (searchdeltatime>=0) and (bplayable) then
      [*] = ;         begin
      [*] = ;            var searchdeltastep : Float = arg_obj.uniqrnd*0.15+0.3;
      [*] = ;            arg_obj.lastsearchenemy := gametime+searchdeltastep;
      [*] = ;            var bai : Boolean = gPlayer[plInd].bai and (not gPlayer[plInd].aidata.bhumanai);
      [*] = ;            var usage : Integer = TObjProp(pobjprop).usage;
      [*] = ;            var bart : Boolean = (TObjProp(pobjprop).bArtillery) and (((usage=gc_obj_usage_mortar) or (usage=gc_obj_usage_supermortar) or (usage=gc_obj_usage_cannon)));
      [*] = ;            if (not gbool_editor_peacemode) and (bplayable) and (arg_obj.bsearchenemy)
      [*] = ;            and _misc_IsCorrectScanCellOwner(plInd, arg_obj.scanGridX, arg_obj.scanGridY) then
      [*] = ;            begin
      [*] = ;               var orderinfoprogress : Float = arg_obj.orders[0].info.progress;
      [*] = ;               var bOrderNone, bOrderMove, bOrderAttack, bOrderGuard, bOrderLeave : Boolean;
      [*] = ;               case ordertype of
      [*] = ;                  gc_obj_order_type_none : bOrderNone := True;
      [*] = ;                  gc_obj_order_type_move : bOrderMove := True;
      [*] = ;                  gc_obj_order_type_attackobj : bOrderAttack := True;
      [*] = ;                  gc_obj_order_type_guard : bOrderGuard := True;
      [*] = ;                  gc_obj_order_type_leavebuilding : bOrderLeave := True;
      [*] = ;               end;
      [*] = ;               var bOrderAllowSearch : Boolean;
      [*] = ;               var bSmartSearch : Boolean = bOrderMove and gProfile.bsearchenemyinfront and (orderinfoprogress=gc_obj_order_move_mode_default) and (TObjProp(pobjprop).media<>gc_obj_media_water) and (not gbool_lan_isonlinecached);
      [*] = ;               if not bai then
      [*] = ;               begin
      [*] = ;                  if bOrderNone then
      [*] = ;                  bOrderAllowSearch := True
      [*] = ;                  else
      [*] = ;                  if bOrderAttack and ((orderinfoprogress=gc_obj_order_attack_mode_default) or (gProfile.bsearchenemyinfront and (arg_obj.squad>=0))) then
      [*] = ;                  bOrderAllowSearch := True
      [*] = ;                  else
      [*] = ;                  if bOrderMove and (orderinfoprogress=gc_obj_order_move_mode_attack) then
      [*] = ;                  bOrderAllowSearch := True
      [*] = ;                  else
      [*] = ;                  if bOrderLeave or bOrderGuard then
      [*] = ;                  bOrderAllowSearch := True;
      [*] = ;                  {case ordertype of
      [*] = ;                     gc_obj_order_type_none : bOrderAllowSearch := True;
      [*] = ;                     gc_obj_order_type_move : bOrderAllowSearch := (orderinfoprogress=gc_obj_order_move_mode_attack);
      [*] = ;                     gc_obj_order_type_attackobj : bOrderAllowSearch := (orderinfoprogress=gc_obj_order_attack_mode_default) or ((gProfile.bsearchenemyinfront) and (arg_obj.squad>=0));
      [*] = ;                     gc_obj_order_type_guard : bOrderAllowSearch := True;
      [*] = ;                     gc_obj_order_type_leavebuilding : bOrderAllowSearch := True;
      [*] = ;                  end;}
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  if bOrderNone then
      [*] = ;                  bOrderAllowSearch := True
      [*] = ;                  else
      [*] = ;                  if bOrderAttack and (orderinfoprogress=gc_obj_order_attack_mode_default) then
      [*] = ;                  bOrderAllowSearch := True
      [*] = ;                  else
      [*] = ;                  if bOrderMove and (orderinfoprogress=gc_obj_order_move_mode_attack) then
      [*] = ;                  bOrderAllowSearch := True
      [*] = ;                  else
      [*] = ;                  if (arg_obj.squad<0) or bart then
      [*] = ;                  bOrderAllowSearch := True
      [*] = ;                  else
      [*] = ;                  if TSquad(gPlayer[plInd].squads.Get(arg_obj.squad)).fArmy<0 then
      [*] = ;                  bOrderAllowSearch := True;
      [*] = ;                  {case ordertype of
      [*] = ;                     gc_obj_order_type_none : bOrderAllowSearch := True;
      [*] = ;                     gc_obj_order_type_move : bOrderAllowSearch := (orderinfoprogress=gc_obj_order_move_mode_attack);
      [*] = ;                     gc_obj_order_type_attackobj : bOrderAllowSearch := (orderinfoprogress=gc_obj_order_attack_mode_default);
      [*] = ;                     //gc_obj_order_type_guard : bOrderAllowSearch := True;
      [*] = ;                  end}
      [*] = ;               end;
      [*] = ;
      [*] = ;               var bScenarioSmartArt : Boolean;
      [*] = ;               if (gScenario.bactive) and (not gMap.bbattle) then
      [*] = ;               begin
      [*] = ;                  bScenarioSmartArt := (plInd<>0) and (TObjProp(pobjprop).media<>gc_obj_media_water);
      [*] = ;                  if bart and bScenarioSmartArt and (bOrderNone or bOrderAttack or (ordertype=gc_obj_order_type_attackpoint)) then
      [*] = ;                  bOrderAllowSearch := True;
      [*] = ;               end;
      [*] = ;               //bOrderAllowSearch := bart and bai;
      [*] = ;
      [*] = ;               if bOrderAllowSearch then
      [*] = ;               begin
      [*] = ;                  //function DoRand(var res : Boolean) : Boolean;
      [*] = ;                  //begin
      [*] = ;                     //   //res := random>(1-0.12*(searchdeltastep+searchdeltatime)/0.35);
      [*] = ;                     //   res := false;
      [*] = ;                     //   Result := res;
      [*] = ;                  //end;
      [*] = ;                  //var rndres : Boolean;
      [*] = ;                  var orderinfotrg : Integer = arg_obj.orders[0].info.trg;
      [*] = ;                  var stoHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;                  if bOrderAttack and (stoHnd=0) then
      [*] = ;                  stoHnd := orderinfotrg;
      [*] = ;                  var bNotRunAway : Boolean = not (bOrderMove and (orderinfoprogress=gc_obj_order_move_mode_runaway));
      [*] = ;                  if bIdleAction and bNotRunAway and (((stoHnd=0) or ((orderinfotrg=stoHnd) and (bOrderGuard or (bai and bart))))) then
      [*] = ;                  //or ((not bIdleMove or ((usage=gc_obj_usage_shooter) {and bNotRunAway})) and DoRand(rndres)) then
      [*] = ;                  begin
      [*] = ;                     //if rndres then
      [*] = ;                     //arg_obj.lastsearchenemy := arg_obj.lastsearchenemy+5+arg_obj.uniqrnd*3; // make similar check next time after some long delay
      [*] = ;                     if (TObjProp(pobjprop).media=gc_obj_media_water) then
      [*] = ;                     begin
      [*] = ;                        var bAttackOrder : Boolean = (bOrderAttack or (ordertype=gc_obj_order_type_attackpoint));
      [*] = ;                        if (arg_obj.attackdelay=0) or (bScenarioSmartArt) then
      [*] = ;                        begin
      [*] = ;                           if (not bAttackOrder) then
      [*] = ;                           begin
      [*] = ;                              var posx, posz : Float;
      [*] = ;                              var trgHnd : Integer;
      [*] = ;                              if (_unit_SearchEnemyLongRangeArtillery(myHnd, GetGameObjectMyPositionX, GetGameObjectMyPositionZ, true, trgHnd, posx, posz, (bai or bScenarioSmartArt))) then
      [*] = ;                              begin
      [*] = ;                                 if (bAttackOrder) then
      [*] = ;                                 _unit_RemoveOrder(myHnd, 0);
      [*] = ;
      [*] = ;                                 var pOrder : Pointer;
      [*] = ;                                 if trgHnd<>0 then
      [*] = ;                                 pOrder := _unit_OrderAttack(myHnd, trgHnd, True, False, False)
      [*] = ;                                 else
      [*] = ;                                 pOrder := _unit_OrderAttackPoint(myHnd, posx, posz, True, False);
      [*] = ;
      [*] = ;                                 TOrder(pOrder).static := bai;
      [*] = ;                                 arg_obj.inBattle := true;
      [*] = ;
      [*] = ;                                 if trgHnd <> 0 then
      [*] = ;                                 begin
      [*] = ;                                    if _unit_GetUsage(trgHnd) = gc_obj_usage_tower then
      [*] = ;                                    arg_obj.artDistFactor := 1
      [*] = ;                                    else
      [*] = ;                                    arg_obj.artDistFactor := 0.5;
      [*] = ;
      [*] = ;                                    arg_obj.artX := GetGameObjectPositionXByHandle(trgHnd);
      [*] = ;                                    arg_obj.artY := GetGameObjectPositionZByHandle(trgHnd);
      [*] = ;                                    arg_obj.artHnd := trgHnd;
      [*] = ;                                 end
      [*] = ;                                 else
      [*] = ;                                 begin
      [*] = ;                                    arg_obj.artX := posX;
      [*] = ;                                    arg_obj.artY := posZ;
      [*] = ;                                    arg_obj.artHnd := 0;
      [*] = ;                                    arg_obj.artDistFactor := 0.5;
      [*] = ;                                 end;
      [*] = ;                              end
      [*] = ;                              else
      [*] = ;                              begin
      [*] = ;                                 if arg_obj.inBattle then
      [*] = ;                                 _unit_ResetArtParams(myHnd);
      [*] = ;
      [*] = ;                                 if (bAttackOrder) then
      [*] = ;                                 _unit_RemoveOrder(myHnd, 0);
      [*] = ;                              end;
      [*] = ;                           end
      [*] = ;                           else
      [*] = ;                           begin
      [*] = ;                              var trgHnd : Integer;
      [*] = ;                              var posx, posz : Float;
      [*] = ;                              if (bScenarioSmartArt) and (arg_obj.attackdelay>0) and (not _unit_SearchEnemyLongRangeArtillery(myHnd, GetGameObjectMyPositionX, GetGameObjectMyPositionZ, true, trgHnd, posx, posz, True)) then
      [*] = ;                              begin
      [*] = ;                                 if (bAttackOrder) then
      [*] = ;                                 _unit_RemoveOrder(myHnd, 0);
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        begin
      [*] = ;                           if (bAttackOrder) then
      [*] = ;                           begin
      [*] = ;                              _unit_RemoveOrder(myHnd, 0);
      [*] = ;                              _unit_SetSTO(myHnd, 0);
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                        {if (ordertype<>gc_obj_order_type_attackobj) then
      [*] = ;                        begin
      [*] = ;                           var trgHnd : Integer = _unit_DoSearchVictim(myHnd, false);
      [*] = ;                           if (trgHnd<>0) then
      [*] = ;                           begin
      [*] = ;                              var pOrder : Pointer = _unit_OrderAttack(myHnd, trgHnd, True, False, False);
      [*] = ;                              TOrder(pOrder).static := true;
      [*] = ;                              arg_obj.inBattle := true;
      [*] = ;                           end
      [*] = ;                           else
      [*] = ;                           begin
      [*] = ;                              arg_obj.inBattle := false;
      [*] = ;                           end;
      [*] = ;                           //_unit_DoSearchVictim(myHnd, false);
      [*] = ;                        end;}
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     if bart and (bai or bScenarioSmartArt) then
      [*] = ;                     begin
      [*] = ;                        var bAttackOrder : Boolean = bOrderAttack or (ordertype=gc_obj_order_type_attackpoint);
      [*] = ;                        if (arg_obj.attackdelay=0) or bScenarioSmartArt then
      [*] = ;                        begin
      [*] = ;                           if not bAttackOrder then
      [*] = ;                           begin
      [*] = ;                              var posx, posz : Float;
      [*] = ;                              var trgHnd : Integer;
      [*] = ;                              if (_unit_SearchEnemyLongRangeArtillery(myHnd, GetGameObjectMyPositionX, GetGameObjectMyPositionZ, true, trgHnd, posx, posz, True)) then
      [*] = ;                              begin
      [*] = ;                                 if (bAttackOrder) then
      [*] = ;                                 _unit_RemoveOrder(myHnd, 0);
      [*] = ;
      [*] = ;                                 var pOrder : Pointer;
      [*] = ;                                 if trgHnd<>0 then
      [*] = ;                                 pOrder := _unit_OrderAttack(myHnd, trgHnd, True, False, False)
      [*] = ;                                 else
      [*] = ;                                 pOrder := _unit_OrderAttackPoint(myHnd, posx, posz, True, False);
      [*] = ;
      [*] = ;                                 TOrder(pOrder).static := true;
      [*] = ;                                 arg_obj.inBattle := true;
      [*] = ;
      [*] = ;                                 if trgHnd <> 0 then
      [*] = ;                                 begin
      [*] = ;                                    if _unit_GetUsage(trgHnd) = gc_obj_usage_tower then
      [*] = ;                                    arg_obj.artDistFactor := 1
      [*] = ;                                    else
      [*] = ;                                    arg_obj.artDistFactor := 0.5;
      [*] = ;
      [*] = ;                                    arg_obj.artX := GetGameObjectPositionXByHandle(trgHnd);
      [*] = ;                                    arg_obj.artY := GetGameObjectPositionZByHandle(trgHnd);
      [*] = ;                                    arg_obj.artHnd := trgHnd;
      [*] = ;                                 end
      [*] = ;                                 else
      [*] = ;                                 begin
      [*] = ;                                    arg_obj.artX := posX;
      [*] = ;                                    arg_obj.artY := posZ;
      [*] = ;                                    arg_obj.artHnd := 0;
      [*] = ;                                    arg_obj.artDistFactor := 0.5;
      [*] = ;                                 end;
      [*] = ;                              end
      [*] = ;                              else
      [*] = ;                              begin
      [*] = ;                                 if arg_obj.inBattle then
      [*] = ;                                 _unit_ResetArtParams(myHnd);
      [*] = ;
      [*] = ;                                 if (bAttackOrder) then
      [*] = ;                                 _unit_RemoveOrder(myHnd, 0);
      [*] = ;                              end;
      [*] = ;                           end
      [*] = ;                           else
      [*] = ;                           begin
      [*] = ;                              var trgHnd : Integer;
      [*] = ;                              var posx, posz : Float;
      [*] = ;                              if bScenarioSmartArt and (arg_obj.attackdelay>0) then
      [*] = ;                              begin
      [*] = ;                                 var bTargetExists : Boolean = _unit_SearchEnemyLongRangeArtillery(myHnd, GetGameObjectMyPositionX, GetGameObjectMyPositionZ, true, trgHnd, posx, posz, True);
      [*] = ;                                 if (not bTargetExists) then
      [*] = ;                                 _unit_RemoveOrder(myHnd, 0)
      [*] = ;                                 else
      [*] = ;                                 begin
      [*] = ;                                    _unit_RemoveOrder(myHnd, 0);
      [*] = ;                                    var pOrder : Pointer = _unit_OrderAttackPoint(myHnd, posx, posz, True, False);
      [*] = ;                                    TOrder(pOrder).static := true;
      [*] = ;                                 end;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end
      [*] = ;                        else
      [*] = ;                        if (bAttackOrder) then
      [*] = ;                        begin
      [*] = ;                           _unit_RemoveOrder(myHnd, 0);
      [*] = ;                           _unit_SetSTO(myHnd, 0);
      [*] = ;                        end;
      [*] = ;                     end
      [*] = ;                     else
      [*] = ;                     begin
      [*] = ;                        var pobj : Pointer = arg_obj;
      [*] = ;                        _misc_ForceSearchEnemyUseParams(myHnd, pobj, pobjprop, plInd, bai, bOrderAttack);
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               if bSmartSearch and (not bai) then
      [*] = ;               begin
      [*] = ;                  const cProgressSmartSearch = 3;
      [*] = ;                  if (progresstick mod cProgressSmartSearch=0) and (_unit_DoSearchVictim(myHnd, false)<>0) then
      [*] = ;                  gPlayer[plInd].searchenemycur := gPlayer[plInd].searchenemycur+gc_progress_searchenemyweight;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         //_misc_ProfilerEnd('Nothing_SV');
      [*] = ;
      [*] = ;         // прогресс вероятности смерти юнита, в случае голода
      [*] = ;         if gProfile.bFamine and (gPlayer[plInd].bfamine) and (bplayable) then
      [*] = ;         begin
      [*] = ;            if (gInterface.gamemode=gc_gamemode_game) or (gInterface.gamemode=gc_gamemode_spectator) then
      [*] = ;            begin
      [*] = ;               if (not TObjProp(pobjprop).bbuilding) and (not TObjProp(pobjprop).bnohungry) then
      [*] = ;               begin
      [*] = ;                  if (arg_obj.insideofuid<>0) and (gPlayer[plInd].difficulty>0) then // won't be here, cause unit insideofuid is unplayable
      [*] = ;                  begin
      [*] = ;                     // случай, если юнит сидит в шахте
      [*] = ;                     {var absorberHnd : Integer = GetGameObjectHandleByUniqueId(arg_obj.insideofuid);
      [*] = ;                     if (absorberHnd<>0) then
      [*] = ;                     begin
      [*] = ;                        var pobj2 : Pointer = _unit_GetTObj(absorberHnd);
      [*] = ;                        if (pobj2<>nil) then
      [*] = ;                        begin
      [*] = ;                           if (gObjProp[TObj(pobj2).cid][TObj(pobj2).id].usage=gc_obj_usage_mine) then
      [*] = ;                           begin
      [*] = ;                              var pObjInside : Pointer = _misc_GetObjectArgData(absorberHnd, gc_argunit_inside);
      [*] = ;                              var insidecount2 : Integer;
      [*] = ;                              if pObjInside <> nil then
      [*] = ;                              insidecount2 := TIntegerList(pObjInside).GetCount;
      [*] = ;
      [*] = ;                              if (insidecount2>0) and (_misc_RandomInt<(250/insidecount2)) then
      [*] = ;                              begin
      [*] = ;                                 _unit_SetTagStates(myHnd, gc_statetag_essential_death);
      [*] = ;                                 arg_obj.insideofuid := 0;
      [*] = ;                              end;
      [*] = ;                           end;
      [*] = ;                        end;
      [*] = ;                     end;}
      [*] = ;                  end
      [*] = ;                  else
      [*] = ;                  if (((gPlayer[plInd].difficulty>1) or (gbool_lan_isonlinecached)) and (_misc_RandomInt<50)) or ((gPlayer[plInd].difficulty=0) and (_misc_RandomInt<5)) or ((gPlayer[plInd].difficulty=1) and (_misc_RandomInt<12)) then
      [*] = ;                  _unit_SetTagStates(myHnd, gc_statetag_essential_death);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         if (gPlayer[plInd].brebellion) and (TObjProp(pobjprop).bmercenary) and (plInd<>gc_player_mercenaryind) and (bplayable) then
      [*] = ;         begin
      [*] = ;            if (gInterface.gamemode=gc_gamemode_game) or (gInterface.gamemode=gc_gamemode_spectator) then
      [*] = ;            begin
      [*] = ;               // прогресс бунта юнита, в случае если у игрока не осталось золота для платы
      [*] = ;               if ((gPlayer[plInd].difficulty>1) and (_misc_RandomInt<6000)) or ((gPlayer[plInd].difficulty=0) and (_misc_RandomInt<100)) or ((gPlayer[plInd].difficulty=1) and (_misc_RandomInt<200)) then
      [*] = ;               begin
      [*] = ;                  var plMercHnd : Integer = GetPlayerHandleByIndex(gc_player_mercenaryind);
      [*] = ;                  _misc_ChangePlayer(myHnd, plMercHnd, False, False, True);
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            if (plHnd=GetPlayerHandleInterfaceIO) then
      [*] = ;            begin
      [*] = ;               // Show log if player InterfaceIO
      [*] = ;               var s1 : String = 'Rebel, but in editor mode your units wont die';
      [*] = ;               var s2 : String;
      [*] = ;               _misc_ShowLogMessage(gc_logmessagesid_none, s1, s2, False);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         if (TObjProp(pobjprop).bcapture) and (bplayable) then
      [*] = ;         begin
      [*] = ;            var bart : Boolean = TObjProp(pobjprop).bArtillery;
      [*] = ;            if gametime>arg_obj.lasttimecheckcapture then
      [*] = ;            begin
      [*] = ;               if not bart then
      [*] = ;               arg_obj.lasttimecheckcapture := gametimewithrnd{arg_obj.lasttimecheckcapture}+gc_unit_TimeCheckCapture
      [*] = ;               else
      [*] = ;               arg_obj.lasttimecheckcapture := gametimewithrnd{arg_obj.lasttimecheckcapture}+gc_unit_TimeCheckCaptureArt;
      [*] = ;               if ((gMap.settings.additional.capture=gc_mapsettings_capture_default) or (bart)
      [*] = ;               or ((gMap.settings.additional.capture=gc_mapsettings_capture_onlyartillery) and (bart))) then // optimized
      [*] = ;               _misc_CheckCapture(myHnd)
      [*] = ;            end;
      [*] = ;            {case gMap.settings.additional.capture of
      [*] = ;               gc_mapsettings_capture_default : _misc_CheckCapture(myHnd);
      [*] = ;               gc_mapsettings_capture_nopeasants : begin
      [*] = ;                  if (TObjProp(pobjprop).usage<>gc_obj_usage_peasant) then
      [*] = ;                  _misc_CheckCapture(myHnd);
      [*] = ;               end;
      [*] = ;               gc_mapsettings_capture_nocenterspeasants : begin
      [*] = ;                  if (TObjProp(pobjprop).usage<>gc_obj_usage_peasant) and (TObjProp(pobjprop).usage<>gc_obj_usage_center) then
      [*] = ;                  _misc_CheckCapture(myHnd);
      [*] = ;               end;
      [*] = ;               gc_mapsettings_capture_onlyartillery : begin
      [*] = ;                  if (not TObjProp(pobjprop).bArtillery) then
      [*] = ;                  _misc_CheckCapture(myHnd);
      [*] = ;               end;
      [*] = ;            end;}
      [*] = ;         end;
      [*] = ;
      [*] = ;         const cProgressTestReach = 3;
      [*] = ;         if ((statetag and gc_statetag_move_walk)<>0) and (progresstick mod cProgressTestReach=0) and TObjProp(pobjprop).media<>gc_obj_media_water then
      [*] = ;         begin
      [*] = ;            var tpCount : Integer = GetGameObjectTrackPointCountByHandle(myHnd);
      [*] = ;            if (tpCount>0) then
      [*] = ;            begin
      [*] = ;               var bTest : Boolean;
      [*] = ;               var updSTOHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd); // stoHnd is invalid after stoHnd := orderinfotrg; string
      [*] = ;               if (updSTOHnd<>0) then
      [*] = ;               begin
      [*] = ;                  var pobjtrg : Pointer = _misc_GetObjectArgData(updSTOHnd, gc_argunit_obj);
      [*] = ;                  if (pobjtrg<>nil) and (TBaseObj(pobjtrg).baseid=gc_baseid_obj) and (not gObjProp[TObj(pobjtrg).cid][TObj(pobjtrg).id].bbuilding) then
      [*] = ;                  bTest := gObjProp[TObj(pobjtrg).cid][TObj(pobjtrg).id].usage<>gc_obj_usage_transport;
      [*] = ;               end
      [*] = ;               else
      [*] = ;               bTest := True;
      [*] = ;
      [*] = ;               if (bTest) then
      [*] = ;               begin
      [*] = ;                  var myRegion : Integer = _misc_GetRegionByPos(GetGameObjectMyPositionX, GetGameObjectMyPositionZ);
      [*] = ;                  var x, y, z : Float;
      [*] = ;                  GetGameObjectMyTrackPointCoordsByIndex(tpCount-1, x, y, z);
      [*] = ;                  var endRegion : Integer = _misc_GetRegionByPos(x, z);
      [*] = ;
      [*] = ;                  if (myRegion >= 0) and (endRegion >= 0) and (myRegion <> endRegion) then
      [*] = ;                  _unit_Stop(myHnd);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         var bfindbestpos : Boolean;
      [*] = ;         if gametime>arg_obj.lasttimebestposition then
      [*] = ;         begin
      [*] = ;            // ship move to water
      [*] = ;            if{bIdleAction and bIdleMove and }(GetGameObjectMyTrackPointMovementModeInt=0) and TObjProp(pobjprop).media=gc_obj_media_water then begin
      [*] = ;               var px: float=GetGameObjectMyPositionX;
      [*] = ;               var pz: float=GetGameObjectMyPositionZ;
      [*] = ;               var collisiontag: integer=GetMapCollisionTag(px, pz, false);
      [*] = ;               if collisionTag<>gc_collisiontag_water then begin
      [*] = ;                  //log('Ship: try move to water');
      [*] = ;                  if _unit_GetRandomWaterPosition(myHnd, 5, px, pz) or _unit_GetRandomWaterPosition(myHnd, 15, px, pz) then begin
      [*] = ;                     //log('Ship: random water position was founded');
      [*] = ;                     _unit_OrderMove(myHnd, px, pz, 0, 0, gc_obj_order_move_mode_runaway{gc_obj_order_move_mode_default}, true);
      [*] = ;                  end;// else ErrorLog('Ship: random water position was not founded');
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            arg_obj.lasttimebestposition := gametimewithrnd{arg_obj.lasttimebestposition}+gc_unit_TimeFindBestPosition;
      [*] = ;            bfindbestpos := True;
      [*] = ;         end;
      [*] = ;         _misc_DoProgressOrders(myHnd, bfindbestpos);
      [*] = ;
      [*] = ;         arg_obj.lastprogresstime := gametime;
      [*] = ;         arg_obj.progresstick := progresstick+1;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;//_misc_ProfilerEnd('unit.nothing');
   struct.end
section.end

