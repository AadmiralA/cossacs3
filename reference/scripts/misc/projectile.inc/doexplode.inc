section.begin
   Name = DoExplode
   Code : struct.begin
      [*] = ;args arg_proj : TProj;
      [*] = ;//_misc_ProfilerBegin('doexp');
      [*] = ;var pweapon : Pointer = gWeapons[arg_proj.weaponid];
      [*] = ;if (pweapon<>nil) then
      [*] = ;begin
      [*] = ;   var ownerHnd : Integer = GetGameObjectHandleByUniqueId(arg_proj.owner);
      [*] = ;   var trgHnd : Integer = GetGameObjectHandleByUniqueId(arg_proj.trg);
      [*] = ;
      [*] = ;   SetRandomKey(floor(arg_proj.uniqrnd*gc_MaxInt)); // sync multiplayer
      [*] = ;
      [*] = ;   var myHnd : Integer = GetGameObjectMyHandle;
      [*] = ;   var px : Float = GetGameObjectPositionXByHandle(myHnd);
      [*] = ;   var py : Float = GetGameObjectPositionYByHandle(myHnd);
      [*] = ;   var pz : Float = GetGameObjectPositionZByHandle(myHnd);
      [*] = ;
      [*] = ;   var waterheight : Float;
      [*] = ;   var bwater : Boolean = RayCastWater(px, pz, waterheight);
      [*] = ;
      [*] = ;   if (py>(waterheight+0.0025)) then
      [*] = ;   bwater := False;
      [*] = ;   if (bwater) then
      [*] = ;   begin
      [*] = ;      py := MaxFloat(py, waterheight+0.0025);
      [*] = ;      SetGameObjectPositionByHandle(myHnd, px, py, pz);
      [*] = ;   end;
      [*] = ;
      [*] = ;   var collisionTag : Integer = GetMapCollisionTagInRadius(px, pz, 1, True);
      [*] = ;   if (not TWeapon(pweapon).buseownerdamage) and (GetGameObjectMyCollisionDetection) and (trgHnd<>0) and ((_unit_IsBuilding(trgHnd)) or (_unit_IsWaterUnit(trgHnd))) then
      [*] = ;   begin
      [*] = ;      if (collisionTag<>gc_collisiontag_building) and (collisionTag<>gc_collisiontag_wall) then
      [*] = ;      begin
      [*] = ;         var bexists : Boolean = (GetGameObjectCountCollidedObjectsByHandle(myHnd)>0);
      [*] = ;         if (bexists) then
      [*] = ;         begin
      [*] = ;            var i : Integer;
      [*] = ;            for i:=0 to GetGameObjectMyCountCollidedObjects-1 do
      [*] = ;            begin
      [*] = ;               if (GetGameObjectMyCollidedGOHandle(i)=trgHnd) then
      [*] = ;               begin
      [*] = ;                  bexists := True;
      [*] = ;                  break;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         if (not bexists) then
      [*] = ;         begin
      [*] = ;            arg_proj.trg := 0;
      [*] = ;            trgHnd := 0;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   var explmedia : Integer = gc_obj_explmedia_default;
      [*] = ;   var propagation : Integer = gWeapons[arg_proj.weaponid].propagation;
      [*] = ;   var damage : Integer;
      [*] = ;   if (TWeapon(pweapon).buseownerdamage) then
      [*] = ;   damage := gPlayer[arg_proj.ownerpl].objbase[arg_proj.ownercid][arg_proj.ownerid].weapon[arg_proj.ownerweaponind].damage
      [*] = ;   else
      [*] = ;   damage := TWeapon(pweapon).damage;
      [*] = ;
      [*] = ;   var bskipdead : Boolean;
      [*] = ;   var bAllowTrgDamage : Boolean = True;
      [*] = ;   var pobj : Pointer = nil;
      [*] = ;   if (trgHnd<>0) then
      [*] = ;   begin
      [*] = ;      pobj := _unit_GetTObj(trgHnd);
      [*] = ;      if (pobj<>nil) then
      [*] = ;      begin
      [*] = ;         bskipdead := TObj(pobj).bdead;
      [*] = ;         if ((gObjProp[TObj(pobj).cid][TObj(pobj).id].bbuilding) and (not _weapon_CheckWeaponTargetable(arg_proj.weaponid))) then
      [*] = ;         bAllowTrgDamage := False;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if (damage>0) then
      [*] = ;   begin
      [*] = ;      // make round and target damage
      [*] = ;      var weaponkind : Integer = gObjProp[arg_proj.ownercid][arg_proj.ownerid].weapon[arg_proj.ownerweaponind].kind;
      [*] = ;      if (TWeapon(pweapon).radius>0) then
      [*] = ;      explmedia := _misc_DoRoundDamage(px, pz, TWeapon(pweapon).radius, damage, ownerHnd, trgHnd, weaponkind, False);
      [*] = ;      if (bAllowTrgDamage) and (trgHnd<>0) and (ownerHnd<>trgHnd) then
      [*] = ;      begin
      [*] = ;         if (pobj<>nil) and (not TObj(pobj).bdead) then
      [*] = ;         _misc_DoDamage(ownerHnd, trgHnd, damage, arg_proj.ownerweaponind, weaponkind);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   // calc explmedia
      [*] = ;   if (explmedia=gc_obj_explmedia_default) and (pobj<>nil) then
      [*] = ;   explmedia := gObjProp[TObj(pobj).cid][TObj(pobj).id].explmedia;
      [*] = ;   if (explmedia=gc_obj_explmedia_default) and (bwater) then
      [*] = ;   explmedia := gc_obj_explmedia_water
      [*] = ;   else
      [*] = ;   if (explmedia=gc_obj_explmedia_default) then
      [*] = ;   begin
      [*] = ;      if collisiontag>0 then
      [*] = ;      case collisionTag of
      [*] = ;         gc_collisiontag_building, gc_collisiontag_wall : begin
      [*] = ;            bskipdead := True;
      [*] = ;            explmedia := gc_obj_explmedia_building;
      [*] = ;         end;
      [*] = ;         gc_collisiontag_water : explmedia := gc_obj_explmedia_water;
      [*] = ;         gc_collisiontag_stretchland, gc_collisiontag_bridge : explmedia := gc_obj_explmedia_grass;
      [*] = ;         gc_collisiontag_terrain, gc_collisiontag_landborder, {yes, waterboarder here cause in engmis3 col tag 110 on rocks}gc_collisiontag_waterborder : explmedia := gc_obj_explmedia_grass;
      [*] = ;      end;
      [*] = ;      if (explmedia=gc_obj_explmedia_default) and (collisionTag=gc_collisiontag_none) then
      [*] = ;      explmedia := gc_obj_explmedia_grass;
      [*] = ;   end;
      [*] = ;   if (explmedia>gc_obj_explmedia_default) and (explmedia<gc_obj_explmedia_count) then
      [*] = ;   begin
      [*] = ;      if {(propagation=gc_weapon_propagation_angle) and}(TWeapon(pweapon).expdecname<>'') and ((explmedia=gc_obj_explmedia_grass) or (collisionTag=gc_collisiontag_terrain)) then
      [*] = ;      begin
      [*] = ;         // create decal
      [*] = ;         _weapon_CreateDecalExp(TWeapon(pweapon).expdecname, px, pz, 0, true, true, 4, 0, 20);
      [*] = ;      end;
      [*] = ;
      [*] = ;      var customexplcount : Integer = TWeapon(pweapon).customexplosions[explmedia].list.GetCount;
      [*] = ;      var childcount : Integer = TWeapon(pweapon).childs.list.GetCount;
      [*] = ;      var rheight, tmpheight : Float;
      [*] = ;      if (childcount>0) or (customexplcount>0) then
      [*] = ;      begin
      [*] = ;         const cMinHeight = 0.0025;
      [*] = ;         var theight : Float;
      [*] = ;         if (not RayCastWater(px, pz, theight)) then // is water, then theight is water height
      [*] = ;         theight := RayCastHeight(px, pz);
      [*] = ;         if ((py-theight)<cMinHeight) then
      [*] = ;         rheight := theight+cMinHeight
      [*] = ;         else
      [*] = ;         rheight := py;
      [*] = ;      end;
      [*] = ;
      [*] = ;      if (customexplcount>0) then
      [*] = ;      begin
      [*] = ;         var mincount : Integer = TWeapon(pweapon).customexplosions[explmedia].mincount;
      [*] = ;         var maxcount : Integer = TWeapon(pweapon).customexplosions[explmedia].maxcount;
      [*] = ;         var count : Integer = mincount+floor(RandomExt*(1+maxcount-mincount));
      [*] = ;         var i : Integer;
      [*] = ;         for i:=0 to count-1 do
      [*] = ;         begin
      [*] = ;            var rndind : Integer = floor(RandomExt*customexplcount);
      [*] = ;            var customexplweaponid : Integer = TWeapon(pweapon).customexplosions[explmedia].list.Get(rndind);
      [*] = ;            var pcustomexplweapon : Pointer = gWeapons[customexplweaponid];
      [*] = ;            if (pcustomexplweapon<>nil) then
      [*] = ;            begin
      [*] = ;               if (not bskipdead) and (trgHnd<>0) and (_weapon_CheckWeaponTargetable(customexplweaponid)) then
      [*] = ;               begin
      [*] = ;                  IntRegister0 := customexplweaponid;
      [*] = ;                  ExecuteState('DoExplodeCustom');
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  //if (bskipdead) and (gWeapons[customexplweaponid].propagation=gc_weapon_propagation_random1) then // commented to let wooden debris appear on dead building
      [*] = ;                  //continue;
      [*] = ;
      [*] = ;                  var tmpHnd : Integer;
      [*] = ;                  if (gWeapons[customexplweaponid].propagation<>gc_weapon_propagation_random1) then
      [*] = ;                  tmpHnd := trgHnd;
      [*] = ;                  if (gWeapons[customexplweaponid].propagation<>gc_weapon_propagation_slightup) then
      [*] = ;                  tmpheight := rheight
      [*] = ;                  else
      [*] = ;                  tmpheight := py;
      [*] = ;                  _weapon_CreateProjectileByWeaponID(ownerHnd, tmpHnd, False, px, tmpheight, pz, customexplweaponid, arg_proj.dx, arg_proj.dy, arg_proj.dz, arg_proj.ownercid, arg_proj.ownerid, arg_proj.ownerweaponind, RandomExt);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      // do childs
      [*] = ;      if (childcount>0) then
      [*] = ;      begin
      [*] = ;         var mincount : Integer = TWeapon(pweapon).childs.mincount;
      [*] = ;         var maxcount : Integer = TWeapon(pweapon).childs.maxcount;
      [*] = ;         var count : Integer = mincount+floor(RandomExt*(1+maxcount-mincount));
      [*] = ;         var i : Integer;
      [*] = ;         for i:=0 to count-1 do
      [*] = ;         begin
      [*] = ;            var rndind : Integer = floor(RandomExt*childcount);
      [*] = ;            var childweaponid : Integer = TWeapon(pweapon).childs.list.Get(rndind);
      [*] = ;            var pchildweapon : Pointer = gWeapons[childweaponid];
      [*] = ;            if (pchildweapon<>nil) then
      [*] = ;            begin
      [*] = ;               if (not bskipdead) and (trgHnd<>0) and (gWeapons[childweaponid].propagation<>gc_weapon_propagation_random1) and (gWeapons[childweaponid].propagation<>gc_weapon_propagation_slightup) then
      [*] = ;               begin
      [*] = ;                  IntRegister0 := childweaponid;
      [*] = ;                  ExecuteState('DoExplodeCustom');
      [*] = ;               end
      [*] = ;               else
      [*] = ;               begin
      [*] = ;                  if (gWeapons[childweaponid].propagation=gc_weapon_propagation_randomsimulate) then
      [*] = ;                  _misc_ProjSimulatePoint(gProjSimulatePoints.curind, childweaponid, arg_proj.owner, px, py, pz)
      [*] = ;                  else
      [*] = ;                  begin
      [*] = ;                     if (bskipdead) and (gWeapons[childweaponid].propagation=gc_weapon_propagation_random1) then // added to remove stone debris from mortar fire to already dead building.
      [*] = ;                     continue;
      [*] = ;
      [*] = ;                     var tmpHnd : Integer;
      [*] = ;                     if (gWeapons[childweaponid].propagation<>gc_weapon_propagation_random1) then
      [*] = ;                     tmpHnd := trgHnd;
      [*] = ;                     if (gWeapons[childweaponid].propagation<>gc_weapon_propagation_slightup) then
      [*] = ;                     tmpheight := rheight
      [*] = ;                     else
      [*] = ;                     tmpheight := py;
      [*] = ;                     _weapon_CreateProjectileByWeaponID(ownerHnd, tmpHnd, False, px, tmpheight, pz, childweaponid, arg_proj.dx, arg_proj.dy, arg_proj.dz, arg_proj.ownercid, arg_proj.ownerid, arg_proj.ownerweaponind, RandomExt);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   // sync weapon
      [*] = ;   var syncweaponcount : Integer = TWeapon(pweapon).syncweaponslist.GetCount;
      [*] = ;   if (TWeapon(pweapon).syncweaponslist.GetCount>0) then
      [*] = ;   begin
      [*] = ;      var i : Integer;
      [*] = ;      for i:=0 to syncweaponcount-1 do
      [*] = ;      begin
      [*] = ;         var syncweaponid : Integer = TWeapon(pweapon).syncweaponslist.Get(i);
      [*] = ;         var psyncweapon : Pointer = gWeapons[syncweaponid];
      [*] = ;         if (psyncweapon<>nil) then
      [*] = ;         _weapon_CreateProjectileByWeaponID(ownerHnd, trgHnd, (gWeapons[syncweaponid].buseownercustomobjpoints){False}, px, py, pz, syncweaponid, arg_proj.dx, arg_proj.dy, arg_proj.dz, arg_proj.ownercid, arg_proj.ownerid, arg_proj.ownerweaponind, RandomExt);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;//_misc_ProfilerEnd('doexp');
      [*] = ;
      [*] = ;SwitchTo('NothingEmpty');
   struct.end
section.end

