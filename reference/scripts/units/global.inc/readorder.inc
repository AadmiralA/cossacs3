section.begin
   Name = ReadOrder
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;//var hnduid: Integer = RecordCustomReadInteger;
      [*] = ;var ordtyp: Integer = RecordCustomReadInteger;
      [*] = ;var taruid: Integer = RecordCustomReadInteger;
      [*] = ;var clrord: Boolean = RecordCustomReadBoolean;
      [*] = ;var locktrg: Boolean = RecordCustomReadBoolean;
      [*] = ;var number: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;var posx, posz: Float;
      [*] = ;if (ordtyp=gc_obj_order_type_patrol) or (ordtyp=gc_obj_order_type_attackpoint) then begin
      [*] = ;   posx:=RecordCustomReadFloat;
      [*] = ;   posz:=RecordCustomReadFloat;
      [*] = ;end;
      [*] = ;
      [*] = ;//var handle: Integer = GetGameObjectHandleByUniqueId(hnduid);
      [*] = ;var target: Integer = GetGameObjectHandleByUniqueId(taruid);
      [*] = ;var tarx: Float = GetGameObjectPositionXByHandle(target);
      [*] = ;var tarz: Float = GetGameObjectPositionZByHandle(target);
      [*] = ;
      [*] = ;var unknown: Boolean;
      [*] = ;var i: Integer;
      [*] = ;var myPlInd : Integer = GetPlayerIndexByHandle(GetPlayerCurrentPlHandle);
      [*] = ;
      [*] = ;if(ordtyp=gc_obj_order_type_build) or (ordtyp=gc_obj_order_type_repair) or (ordtyp=gc_obj_order_type_gotomine) then begin
      [*] = ;   gTmpIntList.Clear;
      [*] = ;   for i:=0 to number-1 do begin
      [*] = ;      var uid: Integer = RecordCustomReadInteger;
      [*] = ;      var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;      gTmpIntList.Add(hnd);
      [*] = ;   end;
      [*] = ;   if ordtyp=gc_obj_order_type_gotomine then
      [*] = ;   begin
      [*] = ;      gPtrList.Clear;
      [*] = ;      _player_OrderUnitsToGoInside(myPlInd, gTmpIntList, gPtrList, target, clrord, false);
      [*] = ;   end
      [*] = ;   else _player_OrderUnitsToBuild(gTmpIntList, target, clrord, (ordtyp=gc_obj_order_type_repair), false);
      [*] = ;   end else begin
      [*] = ;   var bNewLockTrg : Boolean = (locktrg) or (_net_IsServer);
      [*] = ;   for i:=0 to number-1 do begin
      [*] = ;      var hnduid: Integer = RecordCustomReadInteger;
      [*] = ;      var handle: Integer = GetGameObjectHandleByUniqueId(hnduid);
      [*] = ;      if handle<>0 then begin
      [*] = ;         case ordtyp of
      [*] = ;            gc_obj_order_type_gainres: begin
      [*] = ;               if clrord then _unit_ClearOrders(handle);
      [*] = ;               _unit_OrderGainResource(handle, target, tarx, tarz);
      [*] = ;            end;
      [*] = ;            gc_obj_order_type_gotomine, gc_obj_order_type_gototransport: begin
      [*] = ;               if clrord then _unit_ClearOrders(handle);
      [*] = ;               _unit_OrderGoInside(handle, target);
      [*] = ;            end;
      [*] = ;            gc_obj_order_type_attackobj: begin
      [*] = ;               // TODO: Squad logic
      [*] = ;               var pobj: Pointer = _unit_GetTObj(handle);
      [*] = ;               if pobj<>nil then begin
      [*] = ;                  TObj(pobj).bstandground:=False;
      [*] = ;                  TObj(pobj).bsearchenemy:=True;
      [*] = ;                  if (gObjProp[TObj(pobj).cid][TObj(pobj).id].bbuilding) then
      [*] = ;                  _unit_OrderAttack(handle, target, True, False, bNewLockTrg)
      [*] = ;                  else
      [*] = ;                  _unit_OrderAttack(handle, target, False, clrord, bNewLockTrg);
      [*] = ;               end else _net_ErrorLog('order('+IntToStr(ordtyp)+') invalid pointer pobj=nil for uid('+IntToStr(hnduid)+')');
      [*] = ;            end;
      [*] = ;            gc_obj_order_type_guard: begin
      [*] = ;               var pobj: Pointer = _unit_GetTObj(handle);
      [*] = ;               if pobj<>nil then begin
      [*] = ;                  var pobjprop: Pointer = gObjProp[TObj(pobj).cid][TObj(pobj).id];
      [*] = ;                  if pobjprop<>nil then begin
      [*] = ;                     if(TObj(pobj).bbuilt) and (not TObjProp(pobjprop).bbuilding) and (not TObjProp(pobjprop).bcapture) then begin
      [*] = ;                        TObj(pobj).bstandground:=False;
      [*] = ;                        if clrord then _unit_ClearOrders(handle);
      [*] = ;                        _unit_OrderGuard(handle, target);
      [*] = ;                     end; // else _net_ErrorLog('order('+IntToStr(ordtyp)+') too much dummy traffic');
      [*] = ;                  end else _net_ErrorLog('order('+IntToStr(ordtyp)+') invalid pointer pobjbase=nil for uid('+IntToStr(hnduid)+')');
      [*] = ;               end else _net_ErrorLog('order('+IntToStr(ordtyp)+') invalid pointer pobj=nil for uid('+IntToStr(hnduid)+')');
      [*] = ;            end;
      [*] = ;            gc_obj_order_type_patrol: begin
      [*] = ;               //posx, posz
      [*] = ;               var pobj: Pointer = _unit_GetTObj(handle);
      [*] = ;               if pobj<>nil then begin
      [*] = ;                  TObj(pobj).bstandground:=False;
      [*] = ;                  TObj(pobj).bsearchenemy:=True;
      [*] = ;                  if clrord then _unit_ClearOrders(handle);
      [*] = ;                  _unit_OrderPatrol(handle, GetGameObjectPositionXByHandle(handle), GetGameObjectPositionZByHandle(handle), posx, posz);
      [*] = ;               end else _net_ErrorLog('order('+IntToStr(ordtyp)+') invalid pointer pobj=nil for uid('+IntToStr(hnduid)+')');
      [*] = ;            end;
      [*] = ;            gc_obj_order_type_attackpoint: begin
      [*] = ;               var pobj: Pointer = _unit_GetTObj(handle);
      [*] = ;               if pobj<>nil then begin
      [*] = ;                  TObj(pobj).bstandground:=False;
      [*] = ;                  TObj(pobj).bsearchenemy:=True;
      [*] = ;                  if clrord then _unit_ClearOrders(handle);
      [*] = ;                  _unit_OrderAttackPoint(handle, posx, posz, False, False);
      [*] = ;               end else _net_ErrorLog('order('+IntToStr(ordtyp)+') invalid pointer pobj=nil for uid('+IntToStr(hnduid)+')');
      [*] = ;            end;
      [*] = ;            else unknown := true;
      [*] = ;         end;
      [*] = ;      end else _net_ErrorLog('order('+IntToStr(ordtyp)+') invalid uid('+IntToStr(hnduid)+')');
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if unknown then
      [*] = ;_net_ErrorLog('order('+IntToStr(ordtyp)+') is unknown type');
      [*] = ;
      [*] = ;if _net_debug_log_order then
      [*] = ;_net_Log('readorder(number='+IntToStr(number)+';ordtyp='+IntToStr(ordtyp)+';taruid='+IntToStr(taruid)+';clrord='+BoolToStr(clrord)+')');
      [*] = ;// _net_Log('readorder(hnduid='+IntToStr(hnduid)+';ordtyp='+IntToStr(ordtyp)+';taruid='+IntToStr(taruid)+';clrord='+BoolToStr(clrord)+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

