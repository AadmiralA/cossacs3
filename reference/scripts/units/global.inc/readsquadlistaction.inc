section.begin
   Name = ReadSquadListAction
   Code : struct.begin
      [*] = ;args arg_plr: TPlayerArgs;
      [*] = ;
      [*] = ;if not _net_IsOnline and not _net_IsReplay then
      [*] = ;_net_ErrorLog('_net_IsOnline=false');
      [*] = ;
      [*] = ;var plhnd: Integer = GetPlayerCurrentPlHandle;
      [*] = ;var plind: Integer = GetPlayerIndexByHandle(plhnd);
      [*] = ;
      [*] = ;gWriteSquadPtrList.Clear;
      [*] = ;_net_intlist.Clear();
      [*] = ;
      [*] = ;// read data from package
      [*] = ;var bFromServer: Boolean = RecordCustomReadBoolean;
      [*] = ;var action: Integer = RecordCustomReadInteger;
      [*] = ;var bstate: Boolean = RecordCustomReadBoolean;
      [*] = ;var form  : Integer = RecordCustomReadInteger;
      [*] = ;var sqcount : Integer = RecordCustomReadInteger;
      [*] = ;var i : Integer;
      [*] = ;for i:=sqcount-1 downto 0 do
      [*] = ;begin
      [*] = ;   var squid : Integer = RecordCustomReadInteger;
      [*] = ;   var psquad : Pointer = gPlayer[plind].squads.GetByUID(squid);
      [*] = ;   if (psquad<>nil) then
      [*] = ;   gWriteSquadPtrList.AddPointer(psquad)
      [*] = ;   else
      [*] = ;   _net_ErrorLog('ReadSquadListAction invalid squid('+IntToStr(squid)+') action = '+IntToStr(action));
      [*] = ;end;
      [*] = ;var gocount: Integer = RecordCustomReadInteger;
      [*] = ;for i:=0 to gocount-1 do begin
      [*] = ;   var uid: Integer = RecordCustomReadInteger;
      [*] = ;   var hnd: Integer = GetGameObjectHandleByUniqueId(uid);
      [*] = ;   if hnd<>0 then
      [*] = ;   _net_intlist.Add(hnd)
      [*] = ;   else _net_ErrorLog('readsquadlistaction invalid uid('+IntToStr(uid)+')');
      [*] = ;end;
      [*] = ;//Log(' ReadSquadListAction  sqcount='+IntToStr(sqcount)+' squid[0]='+IntToStr(squid0)+' action='+IntToStr(action)+' bstate='+BoolToStr(bstate)+' form='+IntToStr(form)+' listcount='+IntToStr(gocount));
      [*] = ;if _net_IsServer then
      [*] = ;begin
      [*] = ;   var i, j : Integer;
      [*] = ;   for i:=sqcount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var psquad : Pointer = gWriteSquadPtrList.GetPointer(i);
      [*] = ;      if (psquad<>nil) then
      [*] = ;      begin
      [*] = ;         var sqind : Integer = TSquad(psquad).findex;
      [*] = ;         case action of
      [*] = ;            gc_playersquad_action_fill : begin
      [*] = ;               _player_FillSquad(plhnd, sqind, bstate, False); // gIntListWriteSquadAction filled here
      [*] = ;               var bsel : Boolean = TSquad(pSquad).fSelected;
      [*] = ;               for j:=0 to gIntListWriteSquadAction.GetCount-1 do
      [*] = ;               begin
      [*] = ;                  var gohnd : Integer = gIntListWriteSquadAction.Get(j);
      [*] = ;                  _net_intlist.Add(gohnd);
      [*] = ;                  SetGameObjectPickedByHandle(goHnd, bsel);
      [*] = ;                  gGuiUpdateHighlights.Add(goHnd);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (action<>gc_playersquad_action_attack) then
      [*] = ;   begin
      [*] = ;      arg_plr.fpointer0 := gWriteSquadPtrList;
      [*] = ;      arg_plr.fordtyp := action;
      [*] = ;      arg_plr.fbstate := bstate;
      [*] = ;      arg_plr.fform := form;
      [*] = ;      arg_plr.fintlst := _net_intlist;
      [*] = ;      ExecuteState('WriteSquadListAction');
      [*] = ;   end;
      [*] = ;
      [*] = ;   for i:=sqcount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var psquad : Pointer = gWriteSquadPtrList.GetPointer(i);
      [*] = ;      if (psquad<>nil) then
      [*] = ;      begin
      [*] = ;         var sqind : Integer = TSquad(psquad).findex;
      [*] = ;         case action of
      [*] = ;            gc_playersquad_action_attack : begin
      [*] = ;               if (_net_intlist.GetCount>0) then
      [*] = ;               begin
      [*] = ;                  var trgHnd : Integer = _net_intlist.Get(0);
      [*] = ;                  _player_OrderUnitsToAttack(plInd, trgHnd, gEmptyList, gWriteSquadPtrList, bstate, False);
      [*] = ;               end
      [*] = ;               else
      [*] = ;               _net_ErrorLog('readsquadlistaction gc_playersquad_action_attack _net_intlist.GetCount=0');
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_stop : begin
      [*] = ;               var posx, posz, dirx, dirz : Float;
      [*] = ;               if (bstate) then
      [*] = ;               begin
      [*] = ;                  var movemode : Integer = _misc_GetSquadMoveMode(psquad);
      [*] = ;                  _squad_GetCentralPosAndDir(psquad, posx, posz, dirx, dirz);
      [*] = ;                  _player_SetSquadPosition(plHnd, sqind, posx, posz, dirx, dirz, False, True, movemode, False); // pay attention bLAN=True
      [*] = ;               end;
      [*] = ;               _player_StopSquad(plind, sqind);
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_form : begin
      [*] = ;               if (form>=0) then
      [*] = ;               _player_SetSquadFormation(plhnd, sqind, form, bstate, False)
      [*] = ;               else
      [*] = ;               _squad_SetCustomFormation(psquad, False);
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_disband : begin
      [*] = ;               var bsel : Boolean = TSquad(psquad).fSelected;
      [*] = ;               if (bsel) then
      [*] = ;               gIntegerList.Clear;
      [*] = ;               for j:=TSquad(psquad).GetCount-1 downto 0 do // do before disband
      [*] = ;               begin
      [*] = ;                  var gohnd : Integer = TSquad(psquad).Get(j);
      [*] = ;                  gGuiUpdateHighlights.Add(goHnd);
      [*] = ;                  if (bsel) then
      [*] = ;                  gIntegerList.Add(goHnd);
      [*] = ;               end;
      [*] = ;               _misc_DisbandSquad(plhnd, sqind, False);
      [*] = ;               if (bsel) then
      [*] = ;               _misc_AddUnitsToSelection(gIntegerList);
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_searchenemy : begin
      [*] = ;               TSquad(psquad).fSearchVictim := bstate;
      [*] = ;               for j:=TSquad(psquad).GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var gohnd : Integer = TSquad(psquad).Get(j);
      [*] = ;                  var pobj : Pointer = _unit_GetTObj(gohnd);
      [*] = ;                  if (pobj<>nil) then
      [*] = ;                  TObj(pobj).bsearchenemy := bstate;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_standground : begin
      [*] = ;               TSquad(psquad).fStandGround := bstate;
      [*] = ;               if (not bstate) then
      [*] = ;               begin
      [*] = ;                  TSquad(psquad).fHoldMode := False;
      [*] = ;                  TSquad(psquad).fHoldModeProgress := 0;
      [*] = ;               end;
      [*] = ;               for j:=TSquad(psquad).GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var gohnd : Integer = TSquad(psquad).Get(j);
      [*] = ;                  var pobj : Pointer = _unit_GetTObj(gohnd);
      [*] = ;                  if (pobj<>nil) then
      [*] = ;                  TObj(pobj).bstandground := bstate;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_fill : ; // handled before
      [*] = ;            else
      [*] = ;            _net_ErrorLog('ReadSquadListAction invalid action('+IntToStr(action)+')');
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end
      [*] = ;else
      [*] = ;if (not _net_IsReplay) or (bFromServer) then // fix double read when replay saved on client pc in online game
      [*] = ;begin // _net_IsClient or _net_IsReplay
      [*] = ;   var i, j : Integer;
      [*] = ;   for i:=sqcount-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var psquad : Pointer = gWriteSquadPtrList.GetPointer(i);
      [*] = ;      if (psquad<>nil) then
      [*] = ;      begin
      [*] = ;         var sqind : Integer = TSquad(psquad).findex;
      [*] = ;         case action of
      [*] = ;            gc_playersquad_action_stop : begin
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_form : begin
      [*] = ;               if (form>=0) then
      [*] = ;               _player_SetSquadFormation(plhnd, sqind, form, bstate, false)
      [*] = ;               else
      [*] = ;               _squad_SetCustomFormation(psquad, false);
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_fill : begin
      [*] = ;               var bsel : Boolean = TSquad(pSquad).fSelected;
      [*] = ;               for j:=0 to gocount-1 do
      [*] = ;               begin
      [*] = ;                  var gohnd : Integer = _net_intlist.Get(j);
      [*] = ;                  _squad_AddUnit(psquad, gohnd, True{false}); // allow InSquad units. AI merge two squads, so we need to ignore squad
      [*] = ;                  SetGameObjectPickedByHandle(goHnd, bsel);
      [*] = ;                  gGuiUpdateHighlights.Add(goHnd);
      [*] = ;               end;
      [*] = ;               _player_SetSquadFormation(plhnd, sqind, TSquad(psquad).fFormation, bstate, False);
      [*] = ;               {if bstate then
      [*] = ;               begin
      [*] = ;                  var posx, posz, dirx, dirz : Float;
      [*] = ;                  _squad_GetCentralPosAndDir(psquad, posx, posz, dirx, dirz);
      [*] = ;                  _player_SetSquadPosition(plhnd, sqind, posX, posZ, dirx, dirz, false, false, gc_obj_order_move_mode_default, False);
      [*] = ;               end;}
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_disband : begin
      [*] = ;               var bsel : Boolean = TSquad(psquad).fSelected;
      [*] = ;               if (bsel) then
      [*] = ;               gIntegerList.Clear;
      [*] = ;               for j:=TSquad(psquad).GetCount-1 downto 0 do // do before disband
      [*] = ;               begin
      [*] = ;                  var gohnd : Integer = TSquad(psquad).Get(j);
      [*] = ;                  gGuiUpdateHighlights.Add(goHnd);
      [*] = ;                  if (bsel) then
      [*] = ;                  gIntegerList.Add(goHnd);
      [*] = ;               end;
      [*] = ;               _misc_DisbandSquad(plhnd, sqind, false);
      [*] = ;               if (bsel) then
      [*] = ;               _misc_AddUnitsToSelection(gIntegerList);
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_searchenemy : begin
      [*] = ;               TSquad(psquad).fSearchVictim := bstate;
      [*] = ;               for j:=TSquad(psquad).GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var gohnd : Integer = TSquad(psquad).Get(j);
      [*] = ;                  var pobj : Pointer = _unit_GetTObj(gohnd);
      [*] = ;                  if (pobj<>nil) then
      [*] = ;                  TObj(pobj).bsearchenemy := bstate;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            gc_playersquad_action_standground : begin
      [*] = ;               TSquad(psquad).fStandGround := bstate;
      [*] = ;               if (not bstate) then
      [*] = ;               begin
      [*] = ;                  TSquad(psquad).fHoldMode := False;
      [*] = ;                  TSquad(psquad).fHoldModeProgress := 0;
      [*] = ;               end;
      [*] = ;               for j:=TSquad(psquad).GetCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var gohnd : Integer = TSquad(psquad).Get(j);
      [*] = ;                  var pobj : Pointer = _unit_GetTObj(gohnd);
      [*] = ;                  if (pobj<>nil) then
      [*] = ;                  TObj(pobj).bstandground := bstate;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            else
      [*] = ;            _net_ErrorLog('ReadSquadListAction invalid action('+IntToStr(action)+')');
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Progress');
   struct.end
section.end

