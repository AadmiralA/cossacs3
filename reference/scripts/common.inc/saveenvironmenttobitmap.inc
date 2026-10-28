section.begin
   Name = SaveEnvironmentToBitmap
   Code : struct.begin
      [*] = ;const cForestTypePineFirSpruce = 0;
      [*] = ;const cForestTypeMixed = 1;
      [*] = ;const cForestTypeLeaf = 2;
      [*] = ;var forestType : Integer = IntRegister0;
      [*] = ;
      [*] = ;procedure PaintBitmap(const bitmap : Integer; x, y : Integer; r, g, b, a : Float; radius : Integer; bSet, bAdd, bRemove, bRound : Boolean);
      [*] = ;begin
      [*] = ;   var rad : Integer = radius-1;
      [*] = ;   var fr, fg, fb, fa : Float;
      [*] = ;   var i, j : Integer;
      [*] = ;   for i:=-rad to rad do
      [*] = ;   for j:=-rad to rad do
      [*] = ;   begin
      [*] = ;      var newx : Integer = x+i;
      [*] = ;      var newy : Integer = y+j;
      [*] = ;      if (bRound) and (radius>1) then
      [*] = ;      begin
      [*] = ;         var bSkip : Boolean;
      [*] = ;         case radius of
      [*] = ;            2 : bSkip := ((Abs(i)+Abs(j))=rad*2);
      [*] = ;            3 : bSkip := ((Abs(i)+Abs(j))>=radius);
      [*] = ;            4 : bSkip := ((Abs(i)+Abs(j))>=radius);
      [*] = ;         end;
      [*] = ;         if (bSkip) then
      [*] = ;         continue;
      [*] = ;      end;
      [*] = ;      GetBitmapPixel(bitmap, newx, newy, fr, fg, fb, fa);
      [*] = ;      if (bSet) then
      [*] = ;      SetBitmapPixel(bitmap, newx, newy, r, g, b, a)
      [*] = ;      else
      [*] = ;      if (bAdd) then
      [*] = ;      SetBitmapPixel(bitmap, newx, newy, _misc_MinFloat(fr+r, 1), _misc_MinFloat(fg+g, 1), _misc_MinFloat(fb+b, 1), _misc_MinFloat(fa+a, 1))
      [*] = ;      else
      [*] = ;      if (bRemove) then
      [*] = ;      SetBitmapPixel(bitmap, newx, newy, _misc_MaxFloat(fr-r, 0), _misc_MaxFloat(fg-g, 0), _misc_MaxFloat(fb-b, 0), _misc_MaxFloat(fa-a, 0));
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;var width : Integer = GetMapWidth;
      [*] = ;var height : Integer = GetMapHeight;
      [*] = ;
      [*] = ;var bitmap : Integer = CreateBitmap;
      [*] = ;SetBitmapSize(bitmap, width, height);
      [*] = ;SetBitmapFormat(bitmap, 24);
      [*] = ;var i, j : Integer;
      [*] = ;for i:=0 to width-1 do
      [*] = ;for j:=0 to height-1 do
      [*] = ;SetBitmapPixel(bitmap, i, j, 0, 0, 0, 0);
      [*] = ;
      [*] = ;if (GetCountOfPlayers>gc_playerind_env) then
      [*] = ;begin
      [*] = ;   var plhnd : Integer = GetPlayerHandleByIndex(gc_playerind_env);
      [*] = ;   var i : Integer;
      [*] = ;   for i:=GetPlayerGameObjectsCountByHandle(plhnd)-1 downto 0 do
      [*] = ;   begin
      [*] = ;      var gohnd : Integer = GetGameObjectHandleByIndex(i, plhnd);
      [*] = ;      if (_misc_GetBaseObjID(gohnd)=gc_baseid_res) then
      [*] = ;      begin
      [*] = ;         var pres : Pointer = _res_GetTRes(gohnd);
      [*] = ;         if (pres<>nil) and (TRes(pres).itype=gc_resource_type_wood) then
      [*] = ;         begin
      [*] = ;            var x : Integer = round(GetGameObjectPositionXByHandle(gohnd))+(GetMapWidth div 2);
      [*] = ;            var y : Integer = round(GetGameObjectPositionZByHandle(gohnd))+(GetMapHeight div 2);
      [*] = ;            var fr, fg, fb, fa : Float;
      [*] = ;            GetBitmapPixel(bitmap, x, y, fr, fg, fb, fa);
      [*] = ;            var r : Integer = floor(fr*255);
      [*] = ;            var g : Integer = floor(fg*255);
      [*] = ;            var b : Integer = floor(fb*255);
      [*] = ;            var a : Integer = floor(fa*255);
      [*] = ;
      [*] = ;            if r=0 then r := 128;
      [*] = ;            {g := g+1;
      [*] = ;            fr := r/255;
      [*] = ;            fg := g/255;
      [*] = ;            fb := b/255;}
      [*] = ;            SetBitmapPixel(bitmap, x, y, 128/255, fg, fb, 0);
      [*] = ;            PaintBitmap(bitmap, x, y, 0, 1/255, 0, 0, 4, False, True, False, True);
      [*] = ;            //SetBitmapPixel(bitmap, x, y, fr, fg, fb, 0);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;IntRegister2 := bitmap; // will be destroyed in LoadEnvieonmentFromBitmap
      [*] = ;//SaveBitmap(bitmap, '.\data\gen\bitmap\ext\tmp_env.bmp');
      [*] = ;//FreeBitmap(bitmap);
   struct.end
section.end

