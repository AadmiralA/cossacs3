section.begin
   Name = OnAclAnimationReachedWork
   Code : struct.begin
      [*] = ;args arg_obj : TObj;
      [*] = ;
      [*] = ;if (not arg_obj.bdead) then
      [*] = ;begin
      [*] = ;   var bProcess : Boolean = not (_net_IsClient or _net_IsReplay);
      [*] = ;   if (not bProcess) or (arg_obj.orders[0].itype=gc_obj_order_type_gainres) then
      [*] = ;   begin
      [*] = ;      // добавляет крестьянину единцу ресурса (стейт срабатывает на каждый удар топора), и уменьшает hitpoint'ы ресурса.
      [*] = ;      //var trgHnd : Integer = arg_obj.orders[0].info.trg;
      [*] = ;      var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;      var trgHnd : Integer = GetGameObjectSTOHandleByHandle(myHnd);
      [*] = ;      if (trgHnd<>0) and (_misc_GetBaseObjID(trgHnd)=gc_baseid_res) then
      [*] = ;      begin
      [*] = ;         var pres : Pointer = _res_GetTRes(trgHnd);
      [*] = ;         if (pres<>nil) then
      [*] = ;         begin
      [*] = ;            case TRes(pres).itype of
      [*] = ;               gc_resource_type_food : begin
      [*] = ;                  if (bProcess) then
      [*] = ;                  begin
      [*] = ;                     var resdec : Integer = Max(1, floor(100/(1+(gPlayer[arg_obj.pl].fieldlife/100))));
      [*] = ;                     var newhp : Integer = TRes(pres).hp-resdec;
      [*] = ;                     TRes(pres).hp := newhp;
      [*] = ;                  end;
      [*] = ;                  //_unit_RequestPlaySound(myHnd, gc_snd_ind_workfood);
      [*] = ;               end;
      [*] = ;               gc_resource_type_wood : begin
      [*] = ;                  if (bProcess) then
      [*] = ;                  begin
      [*] = ;                     TRes(pres).hp := TRes(pres).hp-1;
      [*] = ;                     if (TRes(pres).hp=0) then
      [*] = ;                     _unit_SetTagStates(trgHnd, gc_statetag_essential_death);
      [*] = ;                  end;
      [*] = ;                  if (gSoundManager.bprocess) then
      [*] = ;                  _unit_RequestPlaySound(myHnd, gc_snd_ind_workwood);
      [*] = ;               end;
      [*] = ;               gc_resource_type_stone : begin
      [*] = ;                  if (bProcess) then
      [*] = ;                  TRes(pres).hp := TRes(pres).hp-1;
      [*] = ;                  if (gSoundManager.bprocess) then
      [*] = ;                  _unit_RequestPlaySound(myHnd, gc_snd_ind_workstone);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            if (bProcess) then
      [*] = ;            begin
      [*] = ;               arg_obj.restype := TRes(pres).itype;
      [*] = ;               arg_obj.resamount := arg_obj.resamount+1;
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         ErrorLog('pres=nil in OnAclAnimationReachedWork');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SwitchTo('Nothing');
   struct.end
section.end

