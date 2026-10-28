section.begin
   Name = LoadTextureTerrainFromBitmap
   Code : struct.begin
      [*] = ;const cLowHeight = 0.1;
      [*] = ;
      [*] = ;var bitmapLow : Integer = CreateBitmap;
      [*] = ;var bitmapHigh : Integer = CreateBitmap;
      [*] = ;LoadBitmap(bitmapLow, '.\data\gen\bitmap\ext\tile_lowground.bmp');
      [*] = ;LoadBitmap(bitmapHigh, '.\data\gen\bitmap\ext\tile_highground.bmp');
      [*] = ;
      [*] = ;var width, height : Integer;
      [*] = ;GetBitmapSize(bitmapLow, width, height);
      [*] = ;
      [*] = ;var lx, ty : Float;
      [*] = ;_misc_GetMapLeftTopPos(lx, ty);
      [*] = ;
      [*] = ;var x, y : Integer;
      [*] = ;for y:=0 to Min(GetMapHeight, height) do
      [*] = ;for x:=0 to Min(GetMapWidth, width) do
      [*] = ;begin
      [*] = ;   var realx : Integer = floor(lx+x);
      [*] = ;   var realy : Integer = floor(ty+y);
      [*] = ;
      [*] = ;   var bitmap : Integer;
      [*] = ;   var h : Float = GetHeightData(realx, realy);
      [*] = ;   if (h<cLowHeight) then
      [*] = ;   bitmap := bitmapLow
      [*] = ;   else
      [*] = ;   bitmap := bitmapHigh;
      [*] = ;
      [*] = ;   var fr, fg, fb, fa : Float;
      [*] = ;   GetBitmapPixel(bitmap, Min(x, width-1), Min(y, height-1), fr, fg, fb, fa);
      [*] = ;   var r : Integer = floor(fr*255);
      [*] = ;   var g : Integer = floor(fg*255);
      [*] = ;   var b : Integer = floor(fb*255);
      [*] = ;   var tex : Integer = r div 2;
      [*] = ;   if (GetTileIndex(x, y)=0) then
      [*] = ;   begin
      [*] = ;      var tile : String = MapGetTileBlockByTileIndex(tex);
      [*] = ;      if (tile<>'') then
      [*] = ;      SetTileIndex(x, y, tex);
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;FreeBitmap(bitmapLow);
      [*] = ;FreeBitmap(bitmapHigh);
   struct.end
section.end

