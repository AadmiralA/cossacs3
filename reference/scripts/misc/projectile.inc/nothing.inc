section.begin
   Name = Nothing
   Code : struct.begin
      [*] = ;args arg_proj : TProj;
      [*] = ;var trgHnd : Integer = GetGameObjectHandleByUniqueId(arg_proj.trg);
      [*] = ;if (trgHnd<>0) then
      [*] = ;begin
      [*] = ;   //_misc_ProfilerBegin('proj_n_0');
      [*] = ;   var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;   var bOk : Boolean;
      [*] = ;   var i : Integer;
      [*] = ;   for i:=0 to GetGameObjectMyCountCollidedObjects-1 do
      [*] = ;   begin
      [*] = ;      var tmpHnd : Integer = GetGameObjectMyCollidedGOHandle(i);
      [*] = ;      if (trgHnd=tmpHnd) then
      [*] = ;      begin
      [*] = ;         bOk := True;
      [*] = ;         break;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   //_misc_ProfilerEnd('proj_n_0');
      [*] = ;   var pobj : Pointer = _unit_GetTObj(trgHnd);
      [*] = ;   if (bOk) and (pobj<>nil) and ((gObjProp[TObj(pobj).cid][TObj(pobj).id].bbuilding) or (gObjProp[TObj(pobj).cid][TObj(pobj).id].explmedia=gc_obj_explmedia_ship)) then
      [*] = ;   begin
      [*] = ;      if (gObjProp[TObj(pobj).cid][TObj(pobj).id].bwall) and ((gPlayer[TObj(pobj).pl].enemyplmask and gPlayer[arg_proj.ownerpl].myplmask)=0) then
      [*] = ;      begin
      [*] = ;         //var ownerHnd : Integer = GetGameObjectHandleByUniqueId(arg_proj.owner);
      [*] = ;         if (gObjProp[arg_proj.ownercid][arg_proj.ownerid].usage=gc_obj_usage_tower) then
      [*] = ;         exit;
      [*] = ;      end;
      [*] = ;
      [*] = ;      //_misc_ProfilerBegin('proj_n_1');
      [*] = ;      //log('proj collide nothing');
      [*] = ;      var bHnd : Integer = GetBehaviourByClassName(myHnd, 'TXThrowBehaviour');
      [*] = ;      var dirsx, dirsy, dirsz : Float;
      [*] = ;      GetBehaviourAffineVectorProperty(bHnd, 'Direction', dirsx, dirsy, dirsz);
      [*] = ;      dirsy := 0;
      [*] = ;      VectorNormalize(dirsx, dirsy, dirsz);
      [*] = ;
      [*] = ;      var dx, dy, dz : Float;
      [*] = ;      var HSpeed : Float = GetBehaviourFloatProperty(bHnd, 'HSpeed');
      [*] = ;      var VSpeed : Float = GetBehaviourFloatProperty(bHnd, 'VSpeed');
      [*] = ;      dx := HSpeed*dirsx;
      [*] = ;      dy := VSpeed;
      [*] = ;      dz := HSpeed*dirsz;
      [*] = ;      VectorNormalize(dx, dy, dz);
      [*] = ;
      [*] = ;      var sx : Float = GetGameObjectMyPositionX-dx;
      [*] = ;      var sy : Float = GetGameObjectMyPositionY-dy;
      [*] = ;      var sz : Float = GetGameObjectMyPositionZ-dz;
      [*] = ;
      [*] = ;      var px, py, pz, nx, ny, nz : Float;
      [*] = ;      //_misc_ProfilerEnd('proj_n_1');
      [*] = ;      //_misc_ProfilerBegin('proj_n_2');
      [*] = ;      //var rayCast : Boolean = OctreeRayCastIntersectChildren(trgHnd, sx, sy, sz, dx, dy, dz, px, py, pz, nx, ny, nz); // no need to test childrens
      [*] = ;      var rayCast : Boolean = OctreeRayCastIntersect(trgHnd, sx, sy, sz, dx, dy, dz, px, py, pz, nx, ny, nz);
      [*] = ;      //_misc_ProfilerEnd('proj_n_2');
      [*] = ;      //_misc_LogFloat('dst', dst);
      [*] = ;      if (rayCast) then
      [*] = ;      begin
      [*] = ;         //_misc_ProfilerBegin('proj_n_3');
      [*] = ;         var dst : Float = VectorDistance(sx, sy, sz, px, py, pz);
      [*] = ;         if (dst<2) then
      [*] = ;         begin
      [*] = ;            {DebugDrawClean('a'+IntToStr(myHnd));
      [*] = ;            DebugDrawClean('b'+IntToStr(myHnd));
      [*] = ;            DebugDrawClean('c'+IntToStr(myHnd));
      [*] = ;            DebugDrawClean('d'+IntToStr(myHnd));
      [*] = ;            DebugDrawBox('a'+IntToStr(myHnd), px,py,pz, 0.1, 1, 0,0);
      [*] = ;            DebugDrawLine('b'+IntToStr(myHnd), px,py,pz, px-dx * 3, py-dy * 3, pz-dz * 3, 0, 0, 1);
      [*] = ;            DebugDrawLine('c'+IntToStr(myHnd), px,py,pz, px+nx * 3, py+ny * 3, pz+nz * 3, 0, 1,0);
      [*] = ;            VectorReflect(dx, dy, dz, nx, ny, nz);
      [*] = ;            DebugDrawLine('d'+IntToStr(myHnd), px,py,pz, px+dx * 3, py+dy * 3, pz+dz * 3, 1, 1,0);}
      [*] = ;            //_misc_ProfilerEnd('proj_n_3');
      [*] = ;            {var ownerHnd : Integer = GetGameObjectHandleByUniqueId(arg_proj.owner);
      [*] = ;            var projHnd : Integer;
      [*] = ;            var plhnd : Integer;
      [*] = ;            if ownerHnd<>0 then
      [*] = ;            plHnd := GetGameObjectPlayerHandleByHandle(ownerHnd)
      [*] = ;            else
      [*] = ;            plHnd := GetPlayerHandleByIndex(0);
      [*] = ;            var parg: Pointer = StateMachineGetArgDataByInd(GetPlayerStateMachineHandle(plHnd), gc_argplayer_plr);
      [*] = ;            if (parg<>nil) then
      [*] = ;            begin
      [*] = ;               TPlayerArgs(parg).fhandle:=ownerHnd;
      [*] = ;               TPlayerArgs(parg).ftarget:=trgHnd;
      [*] = ;               TPlayerArgs(parg).fbstate:=False;
      [*] = ;               TPlayerArgs(parg).fposx:=px;
      [*] = ;               TPlayerArgs(parg).fposy:=py;
      [*] = ;               TPlayerArgs(parg).fposz:=pz;
      [*] = ;               TPlayerArgs(parg).fweaponid:=arg_proj.weaponid;
      [*] = ;               TPlayerArgs(parg).ftrgx:=arg_proj.dx;
      [*] = ;               TPlayerArgs(parg).ftrgy:=arg_proj.dy;
      [*] = ;               TPlayerArgs(parg).ftrgz:=arg_proj.dz;
      [*] = ;               TPlayerArgs(parg).fcid:=arg_proj.ownercid;
      [*] = ;               TPlayerArgs(parg).fid:=arg_proj.ownerid;
      [*] = ;               TPlayerArgs(parg).find:=arg_proj.ownerweaponind;
      [*] = ;               TPlayerArgs(parg).frnd:=RandomExt;
      [*] = ;               PlayerExecuteStateByHandle(plHnd, 'WriteProj');
      [*] = ;               projHnd:=TPlayerArgs(parg).fresult;
      [*] = ;               log('!WRITEPROJ! Nothing on collide cpsid = '+gWeapons[arg_proj.weaponid].sid+' rnd='+IntToStr(GetRandomKey)+' gt='+FloatToStr(GetGameTime));
      [*] = ;
      [*] = ;               if (projHnd<>0) then
      [*] = ;               begin
      [*] = ;                  GameObjectExecuteStateByHandle(projHnd, 'OnStopEvent');
      [*] = ;                  GameObjectRequestToDestroyByHandle(projHnd);
      [*] = ;               end;
      [*] = ;            end
      [*] = ;            else
      [*] = ;            ErrorLog('_unit_DoProjectile parg=nil');
      [*] = ;            var projHnd : Integer = _weapon_CreateProjectileByWeaponID(ownerHnd, trgHnd, False, px, py, pz, arg_proj.weaponid, arg_proj.dx, arg_proj.dy arg_proj.dz, arg_proj.ownercid, arg_proj.ownerid, arg_proj.ownerweaponind, RandomExt);}
      [*] = ;            SetGameObjectPositionByHandle(myHnd, px, py, pz);
      [*] = ;            //BehaviourClear(myHnd);
      [*] = ;            BehaviourClearRequested(myHnd);
      [*] = ;            ExecuteState('OnStopEvent');
      [*] = ;         end;
      [*] = ;         //else
      [*] = ;         //_misc_ProfilerEnd('proj_n_3');
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   //_misc_ProfilerEnd('proj_n_0');
      [*] = ;end;
   struct.end
section.end

