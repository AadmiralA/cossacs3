section.begin
   Name = PostProcess
   Code : struct.begin
      [*] = ;const cRNone = 0;
      [*] = ;const cRPlateau = 1;
      [*] = ;const cRHill = 2;
      [*] = ;const cRRavine = 3;
      [*] = ;const cGNone = 0;
      [*] = ;const cGForest = 1;
      [*] = ;const cGGenForest = 2;
      [*] = ;const cGStone = 3;
      [*] = ;const cGForestStone = 4;
      [*] = ;const cBNone = 0;
      [*] = ;const cBWater = 1;
      [*] = ;
      [*] = ;const cRColorPlateau = 255;
      [*] = ;const cRColorHill = 96;
      [*] = ;const cRColorRavine = 48;
      [*] = ;const cRColorSmooth = 128 div 2;
      [*] = ;const cRColorPlateauSmooth = ((cRColorPlateau div 2)+cRColorSmooth);
      [*] = ;const cRColorHillSmooth = ((cRColorHill div 2)+cRColorSmooth);
      [*] = ;const cRColorRavineSmooth = ((cRColorRavine div 2)+cRColorSmooth);
      [*] = ;const cGColorForest = 255;
      [*] = ;const cGColorGenForest = 64;
      [*] = ;const cGColorStone = 128;
      [*] = ;const cGColorForestStone = 192;
      [*] = ;const cBColorWater = 255;
      [*] = ;
      [*] = ;// source channel, target channel, width, height, radius
      [*] = ;var bitmaptrg : Integer = CreateBitmap;
      [*] = ;var width : Integer = GetMapWidth;
      [*] = ;var height : Integer = GetMapHeight;
      [*] = ;SetBitmapSize(bitmaptrg, width, height);
      [*] = ;
      [*] = ;var mask : array [0..640-1] of array [0..640-1] of TColor;
      [*] = ;var maskTmp : array [0..640-1] of array [0..640-1] of TColor;
      [*] = ;var x, y : Integer;
      [*] = ;for x:=0 to width-1 do
      [*] = ;for y:=0 to height-1 do
      [*] = ;begin
      [*] = ;   var ir, ig, ib : Integer;
      [*] = ;   case gMapMask[x,y].terrain of
      [*] = ;      cRPlateau, cRHill, cRRavine : ir := cRColorPlateau; // all colors raise to max
      [*] = ;   end;
      [*] = ;   if (gMapMask[x,y].water=cBWater) then
      [*] = ;   ib := cBColorWater;
      [*] = ;
      [*] = ;   mask[x, y].r := (ir/255);
      [*] = ;   mask[x, y].g := 0;
      [*] = ;   mask[x, y].b := (ib/255);
      [*] = ;end;
      [*] = ;
      [*] = ;function IsPlateau(x, y : Integer; min : Float) : Boolean;
      [*] = ;begin
      [*] = ;   Result := (mask[ClampInt(x, 0, width-1), ClampInt(y, 0, height-1)].r>=min);
      [*] = ;end;
      [*] = ;
      [*] = ;function IsWater(x, y : Integer; min : Float) : Boolean;
      [*] = ;begin
      [*] = ;   Result := (mask[ClampInt(x, 0, width-1), ClampInt(y, 0, height-1)].b>=min);
      [*] = ;end;
      [*] = ;
      [*] = ;const bRandomNoise = True;
      [*] = ;// Key params
      [*] = ;const cMinDropPower = 1;
      [*] = ;const cMaxDropPower = 3;
      [*] = ;const cNoiseProb = 0.5;
      [*] = ;// Additional params, not so powerful
      [*] = ;const cMinNoise = 1;
      [*] = ;const cMaxNoise = 1;
      [*] = ;const cTryNoiseNearestProb = 0.6;
      [*] = ;const cTryNoiseNearestProbRecursive = 0.99;
      [*] = ;const cMaxDeep = 10;
      [*] = ;//const cDeepExpFactor = 0.15;
      [*] = ;const cIsPlateauMin = 0.1;
      [*] = ;const cIsPlateauMax = 0.95;
      [*] = ;const cMinPower = 0.1;
      [*] = ;const cPowerMinBreak = 0.1;
      [*] = ;const cPowerMaxBreak = 0.9;
      [*] = ;// water
      [*] = ;const cIsWaterMin = 0.1;
      [*] = ;const cIsWaterMax = 0.95;
      [*] = ;
      [*] = ;function IsBorderBlackMax(x, y : Integer) : Boolean;
      [*] = ;begin
      [*] = ;   Result := False;
      [*] = ;   if (not IsPlateau(x, y, cIsPlateauMin)) then
      [*] = ;   //if (not IsPlateau(x, y, cIsPlateauMin)) and (not IsWater(x, y, cIsWaterMin)) then
      [*] = ;   begin
      [*] = ;      if (IsPlateau(x-1, y, cIsPlateauMax)) or (IsPlateau(x, y-1, cIsPlateauMax)) or (IsPlateau(x+1, y, cIsPlateauMax)) or (IsPlateau(x, y+1, cIsPlateauMax)) then
      [*] = ;      Result := True;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;function IsBorderBlackWaterMax(x, y : Integer) : Boolean;
      [*] = ;begin
      [*] = ;   Result := False;
      [*] = ;   //if (not IsWater(x, y, cIsWaterMin)) then
      [*] = ;   if (not IsWater(x, y, cIsWaterMin)) and (not IsPlateau(x, y, cIsPlateauMin)) then
      [*] = ;   begin
      [*] = ;      if (IsWater(x-1, y, cIsWaterMax)) or (IsWater(x, y-1, cIsWaterMax)) or (IsWater(x+1, y, cIsWaterMax)) or (IsWater(x, y+1, cIsWaterMax)) then
      [*] = ;      Result := True;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;function IsBorderBlackWater(x, y : Integer) : Boolean;
      [*] = ;begin
      [*] = ;   Result := False;
      [*] = ;   if (not IsWater(x, y, cIsWaterMax)) then
      [*] = ;   begin
      [*] = ;      if (IsWater(x-1, y, cIsWaterMin)) or (IsWater(x, y-1, cIsWaterMin)) or (IsWater(x+1, y, cIsWaterMin)) or (IsWater(x, y+1, cIsWaterMin)) then
      [*] = ;      Result := True;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;function IsBorderBlack(x, y : Integer) : Boolean;
      [*] = ;begin
      [*] = ;   Result := False;
      [*] = ;   if (not IsPlateau(x, y, cIsPlateauMax)) then
      [*] = ;   begin
      [*] = ;      if (IsPlateau(x-1, y, cIsPlateauMin)) or (IsPlateau(x, y-1, cIsPlateauMin)) or (IsPlateau(x+1, y, cIsPlateauMin)) or (IsPlateau(x, y+1, cIsPlateauMin)) then
      [*] = ;      Result := True;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;procedure TryNoise(x, y, deep : Integer; power : Float; bWater : Boolean; ttype : Integer);
      [*] = ;begin
      [*] = ;   if (power<cMinPower) then
      [*] = ;   exit;
      [*] = ;
      [*] = ;   var rnd : Float;
      [*] = ;   if (bRandomNoise) then
      [*] = ;   rnd := cMinNoise+RandomExt*(cMaxNoise-cMinNoise)
      [*] = ;   else
      [*] = ;   rnd := cMaxNoise;
      [*] = ;   rnd := rnd*power;
      [*] = ;
      [*] = ;   if (not bWater) then
      [*] = ;   begin
      [*] = ;      mask[x, y].r := _misc_MaxFloat(mask[x, y].r, rnd);
      [*] = ;      gMapMask[x,y].terrain := ttype;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      mask[x, y].b := _misc_MaxFloat(mask[x, y].b, rnd);
      [*] = ;      gMapMask[x,y].water := ttype;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;procedure TryNoiseRound(x, y, deep : Integer; power : Float; bWater : Boolean; ttype : Integer);
      [*] = ;begin
      [*] = ;   if (power<cMinPower) or (deep>=cMaxDeep) then
      [*] = ;   exit;
      [*] = ;   var i : Integer;
      [*] = ;   for i:=0 to 7 do
      [*] = ;   begin
      [*] = ;      var rndpower : Float = power*(cPowerMinBreak+RandomExt*(cPowerMaxBreak-cPowerMinBreak));
      [*] = ;      var modpower : Float = 1;
      [*] = ;      var nx : Integer;
      [*] = ;      var ny : Integer;
      [*] = ;      case i of
      [*] = ;         0 : begin
      [*] = ;            nx := x-1;
      [*] = ;            ny := y;
      [*] = ;         end;
      [*] = ;         1 : begin
      [*] = ;            nx := x+1;
      [*] = ;            ny := y;
      [*] = ;         end;
      [*] = ;         2 : begin
      [*] = ;            nx := x;
      [*] = ;            ny := y-1;
      [*] = ;         end;
      [*] = ;         3 : begin
      [*] = ;            nx := x;
      [*] = ;            ny := y+1;
      [*] = ;         end;
      [*] = ;
      [*] = ;         4 : begin
      [*] = ;            nx := x-1;
      [*] = ;            ny := y-1;
      [*] = ;            modpower := 0.5;
      [*] = ;         end;
      [*] = ;         5 : begin
      [*] = ;            nx := x+1;
      [*] = ;            ny := y-1;
      [*] = ;            modpower := 0.5;
      [*] = ;         end;
      [*] = ;         6 : begin
      [*] = ;            nx := x-1;
      [*] = ;            ny := y+1;
      [*] = ;            modpower := 0.5;
      [*] = ;         end;
      [*] = ;         7 : begin
      [*] = ;            nx := x+1;
      [*] = ;            ny := y+1;
      [*] = ;            modpower := 0.5;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      if (not bWater) then
      [*] = ;      begin
      [*] = ;         if (IsBorderBlack(nx, ny)) then
      [*] = ;         begin
      [*] = ;            if (not IsPlateau(nx, ny, 0.1)) then
      [*] = ;            begin
      [*] = ;               TryNoise(nx, ny, deep+1, rndpower*modpower, bWater, ttype);
      [*] = ;               if (RandomExt<cTryNoiseNearestProbRecursive) then
      [*] = ;               TryNoiseRound(nx, ny, deep+1, rndpower*modpower*(cPowerMinBreak+RandomExt*(cPowerMaxBreak-cPowerMinBreak)), bWater, ttype);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         if (IsBorderBlackWater(nx, ny)) then
      [*] = ;         begin
      [*] = ;            if (not IsWater(nx, ny, 0.1)) then
      [*] = ;            begin
      [*] = ;               TryNoise(nx, ny, deep+1, rndpower*modpower, bWater, ttype);
      [*] = ;               if (RandomExt<cTryNoiseNearestProbRecursive) then
      [*] = ;               TryNoiseRound(nx, ny, deep+1, rndpower*modpower*(cPowerMinBreak+RandomExt*(cPowerMaxBreak-cPowerMinBreak)), bWater, ttype);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;{procedure TryNoiseNearest(x, y, deep : Integer; power : Float);
      [*] = ;begin
      [*] = ;   if (power<cMinPower) then
      [*] = ;   exit;
      [*] = ;   if (deep>=cMaxDeep) then
      [*] = ;   exit;
      [*] = ;   var rndInd : Integer = floor(RandomExt*4);
      [*] = ;   var nx, ny : Integer;
      [*] = ;   case rndInd of
      [*] = ;      0 : begin
      [*] = ;         nx := x-1;
      [*] = ;         ny := y;
      [*] = ;      end;
      [*] = ;      1 : begin
      [*] = ;         nx := x+1;
      [*] = ;         ny := y;
      [*] = ;      end;
      [*] = ;      2 : begin
      [*] = ;         nx := x;
      [*] = ;         ny := y-1;
      [*] = ;      end;
      [*] = ;      3 : begin
      [*] = ;         nx := x;
      [*] = ;         ny := y+1;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   //if IsBorderBlack(nx, ny) then
      [*] = ;   //TryNoise(nx, ny, deep+1, power);
      [*] = ;   //if IsBorderBlack(nx, ny) then
      [*] = ;   TryNoiseRound(nx, ny, deep+1, power);
      [*] = ;   if (RandomExt<cTryNoiseNearestProbRecursive) then
      [*] = ;   TryNoiseNearest(nx, ny, deep+1, power*0.75);
      [*] = ;end;}
      [*] = ;function FindBorderTerrainType(x, y : Integer) : Integer;
      [*] = ;begin
      [*] = ;   Result := cRNone;
      [*] = ;   var i, j : Integer;
      [*] = ;   for j:=-1 to 1 do
      [*] = ;   for i:=-1 to 1 do
      [*] = ;   begin
      [*] = ;      if ((i=0) or (j=0)) then
      [*] = ;      begin
      [*] = ;         var ttype : Integer = gMapMask[ClampInt(x+i, 0, width-1), ClampInt(y+j, 0, height-1)].terrain;
      [*] = ;         if (ttype<>cRNone) then
      [*] = ;         begin
      [*] = ;            Result := ttype;
      [*] = ;            exit;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;function FindNearestTerrainType(x, y, rad : Integer) : Integer;
      [*] = ;begin
      [*] = ;   Result := cRNone;
      [*] = ;   var minttype : Integer;
      [*] = ;   var mindist : Float = 1000000;
      [*] = ;   var i, j : Integer;
      [*] = ;   for j:=-rad to rad do
      [*] = ;   for i:=-rad to rad do
      [*] = ;   begin
      [*] = ;      if ((i=0) or (j=0)) then
      [*] = ;      begin
      [*] = ;         var ix : Integer = ClampInt(x+i, 0, width-1);
      [*] = ;         var iy : Integer = ClampInt(y+j, 0, height-1);
      [*] = ;         var ttype : Integer = gMapMask[ix, iy].terrain;
      [*] = ;         if (ttype<>cRNone) then
      [*] = ;         begin
      [*] = ;            var dist : Float = VectorDistance(x, 0, y, ix, 0, iy);
      [*] = ;            if (dist<mindist) then
      [*] = ;            begin
      [*] = ;               minttype := ttype;
      [*] = ;               mindist := dist;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   Result := minttype;
      [*] = ;end;
      [*] = ;
      [*] = ;procedure PreprocessNoise();
      [*] = ;begin
      [*] = ;   for y:=1 to width-2 do
      [*] = ;   for x:=1 to height-2 do
      [*] = ;   begin
      [*] = ;      if (IsBorderBlackMax(x, y)) then
      [*] = ;      begin
      [*] = ;         if (RandomExt<cNoiseProb) then
      [*] = ;         begin
      [*] = ;            var deep : Integer;
      [*] = ;            var power : Float = cMinDropPower+RandomExt*(cMaxDropPower-cMinDropPower);
      [*] = ;            var ttype : Integer = FindBorderTerrainType(x,y);
      [*] = ;            if (ttype>cRNone) then
      [*] = ;            begin
      [*] = ;               TryNoise(x, y, deep, power, False, ttype);
      [*] = ;               if (RandomExt<cTryNoiseNearestProb) then
      [*] = ;               TryNoiseRound(x, y, deep, power, False, ttype);
      [*] = ;            end
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;procedure PreprocessNoiseWater();
      [*] = ;begin
      [*] = ;   for y:=1 to width-2 do
      [*] = ;   for x:=1 to height-2 do
      [*] = ;   begin
      [*] = ;      if (IsBorderBlackWaterMax(x, y)) then
      [*] = ;      begin
      [*] = ;         if (RandomExt<cNoiseProb) then
      [*] = ;         begin
      [*] = ;            var deep : Integer;
      [*] = ;            var power : Float = cMinDropPower+RandomExt*(cMaxDropPower-cMinDropPower);
      [*] = ;            TryNoise(x, y, deep, power, True, cBWater);
      [*] = ;            if (RandomExt<cTryNoiseNearestProb) then
      [*] = ;            TryNoiseRound(x, y, deep, power, True, cBWater);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;procedure BitmapGaussBlur(bitmaptrg : Integer; const w, h : Integer; const r : Float; bFinal : Boolean);
      [*] = ;begin
      [*] = ;   var cDoubleSQRR : Float = 2*r*r;
      [*] = ;   var rs : Integer = Round(r*2.57+0.5);
      [*] = ;   if (rs<1) then
      [*] = ;   ErrorLog('BitmapGaussBlur rs<1');
      [*] = ;   var ix, iy : Integer;
      [*] = ;   var i, j : Integer;
      [*] = ;   for i:=0 to h-1 do
      [*] = ;   for j:=0 to w-1 do
      [*] = ;   begin
      [*] = ;      var valR, wsumR : Float;
      [*] = ;      var valG, wsumG : Float;
      [*] = ;      var valB, wsumB : Float;
      [*] = ;      for iy:=(i-rs) to (i+rs+1-1) do
      [*] = ;      for ix:=(j-rs) to (j+rs+1-1) do
      [*] = ;      begin
      [*] = ;         var x : Integer = min(w-1, max(0, ix));
      [*] = ;         var y : Integer = min(h-1, max(0, iy));
      [*] = ;         var dsq : Float = (ix-j)*(ix-j)+(iy-i)*(iy-i);
      [*] = ;         var wght : Float = Exp( -dsq / (cDoubleSQRR) ) / (PI*cDoubleSQRR);
      [*] = ;
      [*] = ;         var fr, fg, fb : Float;
      [*] = ;         fr := mask[x, y].r;
      [*] = ;         fg := mask[x, y].g;
      [*] = ;         fb := mask[x, y].b;
      [*] = ;
      [*] = ;         valR := valR+fr*wght;
      [*] = ;         wsumR := wsumR+wght;
      [*] = ;         valG := valG+fg*wght;
      [*] = ;         wsumG := wsumG+wght;
      [*] = ;         valB := valB+fb*wght;
      [*] = ;         wsumB := wsumB+wght;
      [*] = ;      end;
      [*] = ;      var resR : Float = Round(255*valR/wsumR);
      [*] = ;      var resG : Float = Round(255*valG/wsumG);
      [*] = ;      var resB : Float = Round(255*valB/wsumB);
      [*] = ;      resR := resR/255;
      [*] = ;      resG := resG/255;
      [*] = ;      resB := resB/255;
      [*] = ;      resR := Clamp(resR, 0, 1);
      [*] = ;      resG := Clamp(resG, 0, 1);
      [*] = ;      resB := Clamp(resB, 0, 1);
      [*] = ;      maskTmp[j, i].r := resR;
      [*] = ;      maskTmp[j, i].g := resG;
      [*] = ;      maskTmp[j, i].b := resB;
      [*] = ;   end;
      [*] = ;
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   for i:=0 to h-1 do
      [*] = ;   for j:=0 to w-1 do
      [*] = ;   begin
      [*] = ;      var fr, fg, fb : Float;
      [*] = ;      fr := maskTmp[j, i].r;
      [*] = ;      fb := maskTmp[j, i].b;
      [*] = ;      var ttype : Integer;
      [*] = ;      if (gMapMask[j, i].terrain=cRNone) then
      [*] = ;      ttype := FindNearestTerrainType(j, i, rs)
      [*] = ;      else
      [*] = ;      ttype := gMapMask[j, i].terrain;
      [*] = ;
      [*] = ;      {case ttype of
      [*] = ;         cRPlateau : fr := maskTmp[j, i].r*(cRColorPlateau/255);
      [*] = ;         cRHill : fr := maskTmp[j, i].r*(cRColorHill/255);
      [*] = ;         cRRavine : fr := maskTmp[j, i].r*(cRColorRavine/255);
      [*] = ;         else
      [*] = ;         fr := 0;
      [*] = ;      end;}
      [*] = ;
      [*] = ;      case gMapMask[j, i].water of
      [*] = ;         cBWater : fb := maskTmp[j, i].b*(cBColorWater/255);
      [*] = ;      end;
      [*] = ;      SetBitmapPixel(bitmaptrg, j, i, fr, fg, fb, 1);
      [*] = ;
      [*] = ;      if (bFinal) then
      [*] = ;      maskTmp[j, i].a := ttype;
      [*] = ;   end;
      [*] = ;   if (bFinal) then
      [*] = ;   for i:=0 to h-1 do
      [*] = ;   for j:=0 to w-1 do
      [*] = ;   gMapMask[j, i].terrain := Round(maskTmp[j, i].a);
      [*] = ;end;
      [*] = ;
      [*] = ;ApplicationProcessMessages;
      [*] = ;PreprocessNoise;
      [*] = ;ApplicationProcessMessages;
      [*] = ;PreprocessNoiseWater;
      [*] = ;ApplicationProcessMessages;
      [*] = ;BitmapGaussBlur(bitmaptrg, width, height, 0.00001, False); // 1
      [*] = ;SaveBitmap(bitmaptrg, '.\data\gen\bitmap\ext\bitmaptrgnosmooth.bmp');
      [*] = ;
      [*] = ;ApplicationProcessMessages;
      [*] = ;BitmapGaussBlur(bitmaptrg, width, height, 0.925, True); // 1
      [*] = ;SaveBitmap(bitmaptrg, '.\data\gen\bitmap\ext\bitmaptrg.bmp');
      [*] = ;ApplicationProcessMessages;
      [*] = ;
      [*] = ;var bitmap : Integer = CreateBitmap;
      [*] = ;SetBitmapFormat(bitmap, 24);
      [*] = ;SetBitmapSize(bitmap, width, height);
      [*] = ;for y:=0 to height-1 do
      [*] = ;for x:=0 to width-1 do
      [*] = ;begin
      [*] = ;   var fr, fg, fb : Integer;
      [*] = ;   if (gMapMask[x, y].terrain>cRNone) then
      [*] = ;   fr := 1;
      [*] = ;   if (gMapMask[x, y].forest>cGNone) then
      [*] = ;   fg := 1;
      [*] = ;   if (gMapMask[x, y].water>cBNone) then
      [*] = ;   fb := 1;
      [*] = ;   SetBitmapPixel(bitmap, x, y, fr, fg, fb, 1);
      [*] = ;end;
      [*] = ;SaveBitmap(bitmap, '.\data\gen\bitmap\ext\bitmaprow.bmp');
      [*] = ;FreeBitmap(bitmap);
      [*] = ;
      [*] = ;FreeBitmap(bitmaptrg);
   struct.end
section.end

