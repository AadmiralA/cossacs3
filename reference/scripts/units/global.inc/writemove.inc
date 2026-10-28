section.begin
   Name = WriteMove
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;var posx: Float = arg_plr.fposx;       // FloatRegister0
      [*] = ;var posz: Float = arg_plr.fposz;       // FloatRegister1
      [*] = ;var visposx: Float = arg_plr.fvisposx;
      [*] = ;var visposz: Float = arg_plr.fvisposz;
      [*] = ;var dirx: Float = arg_plr.fdirx;       // FloatRegister2
      [*] = ;var diry: Float;
      [*] = ;var dirz: Float = arg_plr.fdirz;       // FloatRegister3
      [*] = ;var addord: Boolean = arg_plr.faddord; // BoolRegister0
      [*] = ;var dofirst: Boolean = arg_plr.fdofirst;
      [*] = ;var rebuild: Boolean = arg_plr.frebuild;
      [*] = ;var center : Boolean = arg_plr.fCenter;
      [*] = ;var mode: Integer = arg_plr.fmode;     // IntRegister0
      [*] = ;var grhnd: Integer = arg_plr.fgroup;   // GetPlayerGrHandleByHandle(plhnd, gc_group_selected)
      [*] = ;var squad: Integer = arg_plr.fsquad;
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;var hdirx: Float = dirx;
      [*] = ;var hdiry: Float = diry;
      [*] = ;var hdirz: Float = dirz;
      [*] = ;VectorRotateY(hdirx, hdiry, hdirz, -90);
      [*] = ;
      [*] = ;var minx: Float = GroupGetMinXByHandle(grhnd);
      [*] = ;var miny: Float = GroupGetMinYByHandle(grhnd);
      [*] = ;
      [*] = ;if rebuild then
      [*] = ;GroupGameObjectsGridRebuildExtByHandle(grhnd, posx, posz, dirx, dirz);
      [*] = ;var squaduid : Integer;
      [*] = ;if squad >= 0 then
      [*] = ;begin
      [*] = ;   var pSquad : Pointer = _misc_GetTSquad(plInd, squad);
      [*] = ;   if (pSquad<>nil) then
      [*] = ;   begin
      [*] = ;      squaduid := TSquad(psquad).fuid;
      [*] = ;      TSquad(psquad).fAttackMode := (mode = gc_obj_order_move_mode_attack);
      [*] = ;      TSquad(psquad).fHoldMode := False;
      [*] = ;      TSquad(psquad).fHoldModeProgress := 0;
      [*] = ;      TSquad(psquad).fStandGround := False;
      [*] = ;
      [*] = ;      _squad_RebuildGrid(pSquad, posX, posZ, dirX, dirZ);
      [*] = ;
      [*] = ;      _group_AddUnitsFromSquad(grHnd, pSquad);
      [*] = ;      _group_Initialize(grHnd, pSquad);
      [*] = ;
      [*] = ;      TSquad(pSquad).fDirX := dirX;
      [*] = ;      TSquad(pSquad).fDirZ := dirZ;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   ErrorLog('WriteMove:psquad=nil');
      [*] = ;end;
      [*] = ;
      [*] = ;var cols : Integer = GroupGameObjectsGridGetColumnsByHandle(grHnd);
      [*] = ;var rows : Integer = GroupGameObjectsGridGetRowsByHandle(grHnd);
      [*] = ;
      [*] = ;var netclient: Boolean = (_net_IsClient) and (cols>0) and (rows>0) and (GetGroupCountGameObjectsByHandle(grhnd)>0);
      [*] = ;
      [*] = ;// create package
      [*] = ;if netclient then begin
      [*] = ;   // gc_obj_order_type_movetoxy, gc_obj_order_type_attackxy
      [*] = ;   if not RecordCustomBegin('ReadMove') then
      [*] = ;   _net_ErrorLog('RecordCustomBegin(''ReadMove'')');
      [*] = ;   // write order data to package
      [*] = ;   RecordCustomWriteFloat(dirx);
      [*] = ;   RecordCustomWriteFloat(dirz);
      [*] = ;   RecordCustomWriteBoolean(addord);
      [*] = ;   RecordCustomWriteBoolean(dofirst);
      [*] = ;   RecordCustomWriteInteger(mode);
      [*] = ;   RecordCustomWriteInteger(GetGroupCountGameObjectsByHandle(grhnd));
      [*] = ;end;
      [*] = ;
      [*] = ;var checkcount : Integer;
      [*] = ;var i, j: Integer;
      [*] = ;for j:=rows-1 downto 0 do
      [*] = ;for i:=cols-1 downto 0 do
      [*] = ;begin
      [*] = ;   var gohnd : Integer = GetGroupGameObjectHandleByGridColRow(grhnd, i, j);
      [*] = ;   if gohnd <> 0 then
      [*] = ;   begin
      [*] = ;      var x, y : Float;
      [*] = ;      if not center then
      [*] = ;      begin
      [*] = ;         x := posx + (i-cols/2+0.5) * minx * hdirx - (j+0.5) * miny * dirx;
      [*] = ;         y := posz + (i-cols/2+0.5) * minx * hdirz - (j+0.5) * miny * dirz
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         x := posx + (i-cols/2+0.5) * minx * hdirx - (j-rows/2+0.5) * miny * dirx;
      [*] = ;         y := posz + (i-cols/2+0.5) * minx * hdirz - (j-rows/2+0.5) * miny * dirz;
      [*] = ;      end;
      [*] = ;
      [*] = ;      var pobj: Pointer = _unit_GetTObj(gohnd);
      [*] = ;      if (pobj<>nil) then
      [*] = ;      begin
      [*] = ;         var unitradius: Float = gObjProp[TObj(pobj).cid][TObj(pobj).id].radius;
      [*] = ;         var dispradius: Float = unitradius*1.1;
      [*] = ;         var bsquad: Boolean; // TODO : check if this is formation.
      [*] = ;         if bsquad then
      [*] = ;         begin
      [*] = ;            if (unitradius=gc_obj_radius_horse) then
      [*] = ;            unitradius := gc_obj_radius_formation_horse
      [*] = ;            else
      [*] = ;            unitradius := gc_obj_radius_formation_default;
      [*] = ;            dispradius := unitradius*0.35;
      [*] = ;         end;
      [*] = ;         var dispx: Float;
      [*] = ;         dispx:=(0.5-random)*dispradius;
      [*] = ;         var dispy, dispz: Float;
      [*] = ;         VectorRotateY(dispx, dispy, dispz, random*360);
      [*] = ;         x:=x+dispx;
      [*] = ;         y:=y+dispz;
      [*] = ;
      [*] = ;         //SetGameObjectStateTargetPositionByHandle(gohnd, x, 0, y);
      [*] = ;
      [*] = ;         TObj(pobj).bstandground:=False;
      [*] = ;
      [*] = ;         if not addord then
      [*] = ;         _unit_ClearOrders(gohnd);
      [*] = ;
      [*] = ;         var pOrder : Pointer = _unit_OrderMove(gohnd, x, y, dirx, dirz, mode, dofirst);
      [*] = ;         if (pOrder<>nil) then
      [*] = ;         TOrder(pOrder).info.amount := gPathTag;
      [*] = ;      end;
      [*] = ;
      [*] = ;      checkcount := checkcount+1;
      [*] = ;      if netclient then begin
      [*] = ;         // write uids and coords to package
      [*] = ;         RecordCustomWriteInteger(GetGameObjectUniqueIdByHandle(gohnd));
      [*] = ;         RecordCustomWriteFloat(x);
      [*] = ;         RecordCustomWriteFloat(y);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;if (checkcount<>GetGroupCountGameObjectsByHandle(grhnd)) then
      [*] = ;ErrorLog('WriteMove : checkcount<>GetGroupCountGameObjectsByHandle(grhnd)');
      [*] = ;
      [*] = ;// sending squaduid and player to sync standground
      [*] = ;RecordCustomWriteWord(squaduid);
      [*] = ;if (squaduid>0) then
      [*] = ;RecordCustomWriteByte(plInd);
      [*] = ;
      [*] = ;if gPathTag < 255 then
      [*] = ;gPathTag := gPathTag+1
      [*] = ;else
      [*] = ;gPathTag := 1;
      [*] = ;
      [*] = ;if grHnd <> 0 then
      [*] = ;GroupSetDirectPosAndDirByHandle(grHnd, visPosX, 0, visPosZ, dirX, 0, dirZ);
      [*] = ;
      [*] = ;// finish package
      [*] = ;if netclient then begin
      [*] = ;   if _net_debug_log_size_writer then _net_Log_SizeWriter;
      [*] = ;   RecordCustomEnd;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

