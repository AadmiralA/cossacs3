section.begin
   Name = OnAclAnimationReachedConstruct
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;if (not arg_obj.bdead) then
      [*] = ;begin
      [*] = ;   var bProcess : Boolean = not (_net_IsClient or _net_IsReplay);
      [*] = ;   if (not bProcess) or (arg_obj.orders[0].itype=gc_obj_order_type_build) or (arg_obj.orders[0].itype=gc_obj_order_type_repair) then
      [*] = ;   begin
      [*] = ;      // изменения прогресса и хитпоинтов постройки при строительстве, на каждый удар молотком
      [*] = ;      var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;      //var trgHnd : Integer = arg_obj.orders[0].info.trg;
      [*] = ;      var trgHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;      //if (arg_obj.orders[0].info.trg<>0) and (trgHnd<>arg_obj.orders[0].info.trg) then
      [*] = ;      //ErrorLog('GEC, pay attention trgHnd = '+IntToStr(trgHnd)+' arg_obj.orders[0].info.trg = '+IntToStr(arg_obj.orders[0].info.trg));
      [*] = ;      if (trgHnd<>0) then
      [*] = ;      begin
      [*] = ;         var pobj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;         if (pobj<>nil) then
      [*] = ;         begin
      [*] = ;            var pobjbase : Pointer = gPlayer[TObj(pobj).pl].objbase[TObj(pobj).cid][TObj(pobj).id]; if (gc_debug_pointernil) then _init_DebugPointerNil(pobjbase);
      [*] = ;            if (not TObj(pobj).bbuilt) then
      [*] = ;            begin
      [*] = ;               var delta : Float;
      [*] = ;               if (TObjBase(pobjbase).buildtime<>0) then
      [*] = ;               delta := (gc_buildtime_progressperhit/TObjBase(pobjbase).buildtime)
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  delta := 1;
      [*] = ;                  ErrorLog('DoBuildProgress : buildtime=0');
      [*] = ;               end;
      [*] = ;               TObj(pobj).buildprogress := TObj(pobj).buildprogress+delta;
      [*] = ;               var deltahp : Integer = round(TObjBase(pobjbase).maxhp*delta);
      [*] = ;               if ((TObj(pobj).hp+deltahp)>TObjBase(pobjbase).maxhp) then
      [*] = ;               deltahp := TObjBase(pobjbase).maxhp-TObj(pobj).hp;
      [*] = ;               TObj(pobj).hp := TObj(pobj).hp+deltahp;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            if (arg_obj.orders[0].itype=gc_obj_order_type_repair) then
      [*] = ;            TObj(pobj).hp := Min(TObj(pobj).hp+gc_gameplay_repairhp, TObjBase(pobjbase).maxhp);
      [*] = ;            if (gSoundManager.bprocess) then
      [*] = ;            _unit_RequestPlaySound(myHnd, gc_snd_ind_workconstruct);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         ErrorLog('pobj=nil in OnAclAnimationReachedConstruct');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

