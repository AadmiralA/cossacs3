section.begin
   Name = ReadNew
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;   _net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bFromServer: Boolean = RecordCustomReadBoolean;
      [*] = ;var race: String = RecordCustomReadString;
      [*] = ;var base: String = RecordCustomReadString;
      [*] = ;var posx: Float = RecordCustomReadFloat;
      [*] = ;var posz: Float = RecordCustomReadFloat;
      [*] = ;var cid: Integer = RecordCustomReadInteger; // if cid<=0 then this is countryid, else this is uid for building that produce unit, and by building uid we know it's cid
      [*] = ;var uid: Integer = RecordCustomReadInteger;
      [*] = ;var num: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;if _net_IsServer then begin
      [*] = ;   arg_plr.fracename:=race;
      [*] = ;   arg_plr.fbasename:=base;
      [*] = ;   arg_plr.fposx:=posx;
      [*] = ;   arg_plr.fposz:=posz;
      [*] = ;   arg_plr.fcid:=cid;
      [*] = ;   ExecuteState('WriteNew');
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;begin // _net_IsClient or _net_IsReplay
      [*] = ;   var gohnd: Integer;
      [*] = ;   var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;   if (cid<=0) then
      [*] = ;   gint_unit_parentcid:=-cid // required in gameobject initial state to setup country id
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      var buildhnd : Integer = GetGameObjectHandleByUniqueId(cid);
      [*] = ;      if (buildhnd<>0) then
      [*] = ;      begin
      [*] = ;         var pobj : Pointer = _unit_GetTObj(buildhnd);
      [*] = ;         if (pobj<>nil) then
      [*] = ;         begin
      [*] = ;            gint_unit_parentcid := TObj(pobj).cid;
      [*] = ;            // decrease produce counter
      [*] = ;            if (TObj(pobj).orders[0].itype=gc_obj_order_type_produce) then
      [*] = ;            begin
      [*] = ;               if (TObj(pobj).orders[0].info.amount<>gc_obj_order_produce_infinite) then
      [*] = ;               TObj(pobj).orders[0].info.amount := TObj(pobj).orders[0].info.amount-1;
      [*] = ;               TObj(pobj).orders[0].info.progress := 0;
      [*] = ;
      [*] = ;               if (TObj(pobj).orders[0].info.amount=0) then
      [*] = ;               _unit_RemoveOrder(buildhnd, 0)
      [*] = ;               else
      [*] = ;               if (TObj(pobj).orders[0].info.amount=gc_obj_order_produce_infinite) then
      [*] = ;               begin
      [*] = ;                  // если заказано "бесконечное производство", то смотрим, нет ли других заказов, и переключаемся на следующий. А этот приказ кладем в конец списка приказов.
      [*] = ;                  var lastInd : Integer;
      [*] = ;                  var i : Integer;
      [*] = ;                  for i:=0 to gc_obj_MaxOrderCount-1 do
      [*] = ;                  begin
      [*] = ;                     if (TObj(pobj).orders[i].itype=gc_obj_order_type_none) then
      [*] = ;                     begin
      [*] = ;                        lastInd := i;
      [*] = ;                        break;
      [*] = ;                     end;
      [*] = ;                  end;
      [*] = ;                  if (lastInd>0) then
      [*] = ;                  begin
      [*] = ;                     TObj(pobj).orders[lastInd] := TObj(pobj).orders[0];
      [*] = ;                     _unit_RemoveOrder(buildhnd, 0);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   gohnd:=CreatePlayerGameObjectHandleByHandle(plhnd, race, base, posx, 0, posz);
      [*] = ;   var pobj: Pointer = _unit_GetTObj(gohnd);
      [*] = ;   if pobj<>nil then begin
      [*] = ;      if gObjProp[TObj(pobj).cid][TObj(pobj).id].bbuilding then begin
      [*] = ;         //function _player_ConstructBuilding(plHnd, cid : Integer; sid : String; px, pz : Float) : Integer;
      [*] = ;         TObj(pobj).bbuilt:=false;
      [*] = ;         TObj(pobj).buildprogress:=0;
      [*] = ;         TObj(pobj).hp:=gPlayer[TObj(pobj).pl].objbase[TObj(pobj).cid][TObj(pobj).id].maxhp;
      [*] = ;         _unit_ControlBuildProgress(gohnd);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if (gObjProp[TObj(pobj).cid][TObj(pobj).id].explmedia=gc_obj_explmedia_ship) then
      [*] = ;      GameObjectRollByHandle(gohnd, gc_obj_shipcontructrollangle);
      [*] = ;   end else _net_ErrorLog('invalid pointer: pobj=nil');
      [*] = ;   var gouid: Integer = GetGameObjectUniqueIdByHandle(gohnd);
      [*] = ;   if _net_debug_log_new then
      [*] = ;      _net_Log('readnew(uid='+IntToStr(gouid)+';num='+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+';name=['+race+';'+base+'];coords=['+FloatToStr(posx)+';'+FloatToStr(posz)+'])');
      [*] = ;   if uid<>gouid then
      [*] = ;      _net_ErrorLog('created invalid uid: coming('+IntToStr(uid)+') != current('+IntToStr(gouid)+')');
      [*] = ;   //if num<>GetPlayerGameObjectsCountByHandle(plhnd) then
      [*] = ;   //   _net_ErrorLog('number of units is different: coming('+IntToStr(num)+') != current('+IntToStr(GetPlayerGameObjectsCountByHandle(plhnd))+')');
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

