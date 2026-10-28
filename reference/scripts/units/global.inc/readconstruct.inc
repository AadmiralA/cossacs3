section.begin
   Name = ReadConstruct
   Code : struct.begin
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;var plhnd : Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind : Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bFromServer : Boolean = RecordCustomReadBoolean;
      [*] = ;var cid: Integer = RecordCustomReadInteger;
      [*] = ;var sid: String = RecordCustomReadString;
      [*] = ;var posx: Float = RecordCustomReadFloat;
      [*] = ;var posz: Float = RecordCustomReadFloat;
      [*] = ;var clrord: Boolean = RecordCustomReadBoolean;
      [*] = ;var count: Integer = RecordCustomReadInteger;
      [*] = ;
      [*] = ;_net_intlist.Clear();
      [*] = ;
      [*] = ;var i: Integer;
      [*] = ;for i:=0 to count-1 do begin
      [*] = ;   var hnduid: Integer = RecordCustomReadInteger;
      [*] = ;   var handle: Integer = GetGameObjectHandleByUniqueId(hnduid);
      [*] = ;   if handle<>0 then
      [*] = ;   _net_intlist.Add(handle)
      [*] = ;   else _net_ErrorLog('readconstruct invalid uid('+IntToStr(hnduid)+')');
      [*] = ;end;
      [*] = ;
      [*] = ;// check on can place
      [*] = ;var canplace, allowreq : Boolean = True;
      [*] = ;if (_net_IsServer) then // got request from client
      [*] = ;begin
      [*] = ;   allowreq := (_player_CheckAccessControlRequirements(plHnd, cid, sid)=gc_result_checkaccesscontrolreq_ok);
      [*] = ;   if (allowreq) then
      [*] = ;   begin
      [*] = ;      var miscPlHnd : Integer = GetPlayerHandleByIndex(gc_playerind_misc);
      [*] = ;      var goHnd : Integer = _player_CreateConstructionDummyBySID(miscPlHnd, cid, sid, posx, posz);
      [*] = ;      if (goHnd<>0) then
      [*] = ;      begin
      [*] = ;         var id : Integer = _unit_ConvertObjSIDToID(cid, sid);
      [*] = ;         var usage : Integer = gObjProp[cid][id].usage;
      [*] = ;         if (usage=gc_obj_usage_mine) then
      [*] = ;         canplace := _misc_CanPlaceBuilding(goHnd, plind, usage, posx, posz, 0, 0.26, 0, -1, True)
      [*] = ;         else
      [*] = ;         canplace := _misc_CanPlaceBuilding(goHnd, plind, usage, posx, posz, gc_path_mincoldist, 0.26, 0.5, gc_path_minemincoldist, True);
      [*] = ;         GameObjectDestroyByHandle(goHnd);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;begin
      [*] = ;   if (canplace) and (allowreq) then
      [*] = ;   _player_ConstructBuildingList(plhnd, cid, sid, posx, posz, _net_intlist, clrord, _net_IsServer)
      [*] = ;   else
      [*] = ;   if (not allowreq)then
      [*] = ;   ErrorLog('Client plind='+IntToStr(plind)+' tried to build building sid='+sid+' that doesnt pass AccessControlRequirements')
      [*] = ;   else
      [*] = ;   ErrorLog('Client plind='+IntToStr(plind)+' tried to build building sid='+sid+' in invalid place');
      [*] = ;end;
      [*] = ;
      [*] = ;if _net_debug_log_construct then
      [*] = ;_net_Log('readconstruct(count='+IntToStr(count)+';cid='+IntToStr(cid)+';sid='+sid+')');
      [*] = ;
      [*] = ;if _net_debug_log_size_reader then _net_Log_SizeReader;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

