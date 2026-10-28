section.begin
   Name = GenerateMap
   Code : struct.begin
      [*] = ;const cPlateauHeight = 2.964;
      [*] = ;const cHillHeight = cPlateauHeight/2.25;
      [*] = ;const cRavineHeight = -2.964;
      [*] = ;const cWaterHeight = -1.1;
      [*] = ;
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
      [*] = ;const cProfiler = False;
      [*] = ;const cResetTerrainHeight = True;
      [*] = ;const cPostProcessBitmapGaussian = True;
      [*] = ;const cLoadHeightDataFromTexture = True;
      [*] = ;const cSmoothTerrainUsingBitmap = True;
      [*] = ;const cSmoothAllMapHeight = True;
      [*] = ;const cPlaceWater = True;
      [*] = ;const cPlaceForest = True;
      [*] = ;const cTextureTerrain = True;
      [*] = ;const cSmoothTile = True;
      [*] = ;const cLoadTextureFromBitmap = True;
      [*] = ;const cPlaceFreeForests = True;
      [*] = ;const cSeedWheat = True;
      [*] = ;const cBuildTopology = True;
      [*] = ;const cUpdateMinimap = True;
      [*] = ;const cPostStandPatterns = True;
      [*] = ;// const cDropWaterPatterns (modify water height/facture);
      [*] = ;const cForestTypePineFirSpruce = 0;
      [*] = ;const cForestTypeMixed = 1;
      [*] = ;const cForestTypeLeaf = 2;
      [*] = ;var forestType : Integer = cForestTypePineFirSpruce;//cForestTypePineFirSpruce;
      [*] = ;
      [*] = ;var randkey0, randkey1 : Integer;
      [*] = ;var inputbitmap : String = '.\data\gen\bitmap\bitmapsrc.bmp';
      [*] = ;var inputrandkey : String = '10';
      [*] = ;var folderpath, missionname : String;
      [*] = ;
      [*] = ;var bRandomMap : Boolean = BoolRegister0;
      [*] = ;var bTGA : Boolean;
      [*] = ;
      [*] = ;var srcbitmappath : String;
      [*] = ;
      [*] = ;var mapwidth : Integer = GetMapWidth;
      [*] = ;var mapheight : Integer = GetMapHeight;
      [*] = ;if (not bRandomMap) then
      [*] = ;begin
      [*] = ;   EditorFormClear('GenerateMapFromBitmap');
      [*] = ;   AddEditorFormInput('mapsize', 'Map Size (320-640)', IntToStr(GetMapWidth));
      [*] = ;   if (EditorFormExecute) then
      [*] = ;   begin
      [*] = ;      var genmapsize : Integer = StrToInt(GetEditorFormValue('mapsize'));
      [*] = ;      if (mapwidth<>genmapsize) or (mapheight<>genmapsize) then
      [*] = ;      begin
      [*] = ;         NewMap(genmapsize, genmapsize, 0);
      [*] = ;         exit;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   exit;
      [*] = ;end;
      [*] = ;
      [*] = ;if (not bRandomMap) then
      [*] = ;begin
      [*] = ;   randkey0 := 0;
      [*] = ;   randkey1 := 10;
      [*] = ;   missionname := 'TMPmis#';
      [*] = ;   if (gstring_editor_lastgenbitmapname<>'') then
      [*] = ;   inputbitmap := gstring_editor_lastgenbitmapname;
      [*] = ;   if (gstring_editor_lastgenmapname<>'') then
      [*] = ;   missionname := gstring_editor_lastgenmapname;
      [*] = ;
      [*] = ;   EditorFormClear('GenerateMapFromBitmap');
      [*] = ;   AddEditorFormInput('inputbitmap', 'Bitmap filename', inputbitmap);
      [*] = ;   AddEditorFormInput('randkey1', 'Random Key', inputrandkey);
      [*] = ;   AddEditorFormInput('missionname', 'Mission Name', missionname);
      [*] = ;   if (EditorFormExecute) then
      [*] = ;   begin
      [*] = ;      inputbitmap := GetEditorFormValue('inputbitmap');
      [*] = ;      inputrandkey := GetEditorFormValue('randkey1');
      [*] = ;      randkey1 := StrToInt(inputrandkey);
      [*] = ;      folderpath := '.\data\gen\bitmap\';
      [*] = ;      missionname := GetEditorFormValue('missionname');
      [*] = ;
      [*] = ;      var ext : String = ExtractFileExt(inputbitmap);
      [*] = ;      if (ext='') then
      [*] = ;      inputbitmap := inputbitmap+'.bmp';
      [*] = ;
      [*] = ;      srcbitmappath := inputbitmap;
      [*] = ;      if (not IsFileExists(srcbitmappath)) then
      [*] = ;      begin
      [*] = ;         srcbitmappath := folderpath+inputbitmap;
      [*] = ;         if (not IsFileExists(srcbitmappath)) then
      [*] = ;         begin
      [*] = ;            srcbitmappath := folderpath+'\'+inputbitmap;
      [*] = ;            if (not IsFileExists(srcbitmappath)) then
      [*] = ;            begin
      [*] = ;               var slog : String = 'file '+srcbitmappath+' not exists';
      [*] = ;               srcbitmappath := '.\data\gen\bitmap\'+inputbitmap+'.bmp';
      [*] = ;               log(srcbitmappath);
      [*] = ;               if (not IsFileExists(srcbitmappath)) then
      [*] = ;               begin
      [*] = ;                  ErrorLog(slog);
      [*] = ;                  exit;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      gstring_editor_lastgenbitmapname := inputbitmap;
      [*] = ;      gstring_editor_lastgenmapname := missionname;
      [*] = ;
      [*] = ;      gMap.name := missionname;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   exit;
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   bTGA := True;
      [*] = ;   randkey0 := gMap.settings.gen.randkey0;
      [*] = ;   randkey1 := gMap.settings.gen.randkey1;
      [*] = ;   var pRecordManager : Integer = ParserSelectRecordManager;
      [*] = ;   var bGenRecordManager : Boolean = (ParserGetCountByHandle(pRecordManager)>0);
      [*] = ;   if (bGenRecordManager) then
      [*] = ;   begin
      [*] = ;      var pGen : Integer = ParserSelectByHandleByKey(pRecordManager, 'Generator');
      [*] = ;      if (pGen<>0) then
      [*] = ;      begin
      [*] = ;         folderpath := ParserGetValueByKeyByHandle(pGen, 'maskpath');
      [*] = ;         inputbitmap := ParserGetValueByKeyByHandle(pGen, 'maskname');
      [*] = ;      end
      [*] = ;      else
      [*] = ;      bGenRecordManager := False;
      [*] = ;   end;
      [*] = ;   if (not bGenRecordManager) then
      [*] = ;   begin
      [*] = ;      var plcount : Integer;
      [*] = ;      var i : Integer;
      [*] = ;      for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;      if (gMap.players[i].bexists) and (gMap.players[i].cid<>gc_spectator_countryid) then
      [*] = ;      plcount := plcount+1;
      [*] = ;
      [*] = ;      if (plcount>=gc_MaxPlayerCount) then
      [*] = ;      begin
      [*] = ;         ErrorLog('8th player not yet supported');
      [*] = ;         plcount := gc_MaxPlayerCount-1;
      [*] = ;      end;
      [*] = ;
      [*] = ;      folderpath := StringRegister0;
      [*] = ;
      [*] = ;      var pFileList : Integer = _parser_ParserTemporary(True);
      [*] = ;      ParserGetFilesInRoot(folderpath+'\*.tga');
      [*] = ;      var count : Integer = ParserGetCountByHandle(pFileList);
      [*] = ;      if (count=0) then
      [*] = ;      exit;
      [*] = ;      ParserClearByHandle(pFileList);
      [*] = ;      for i:=plcount to gc_MaxPlayerCount-1 do
      [*] = ;      begin
      [*] = ;         ParserGetFilesInRoot(folderpath+'\'+IntToStr(i)+'pl*.tga');
      [*] = ;         count := ParserGetCountByHandle(pFileList);
      [*] = ;         if (count>0) then
      [*] = ;         break
      [*] = ;         else
      [*] = ;         ParserClearByHandle(pFileList);
      [*] = ;      end;
      [*] = ;      if (count=0) then
      [*] = ;      exit;
      [*] = ;      var rndind : Integer = floor(RandomExt*count);
      [*] = ;      inputbitmap := ParserGetValueByIndexByHandle(pFileList, rndind);
      [*] = ;      log(inputbitmap+' randkey='+IntToStr(gMap.settings.gen.randkey1));
      [*] = ;
      [*] = ;      var pRMGenerator : Integer = _misc_SelectRecordManagerGeneratorParser(True);
      [*] = ;      if (pRMGenerator<>0) then
      [*] = ;      begin
      [*] = ;         ParserSetValueByKeyByHandle(pRMGenerator, 'maskpath', folderpath);
      [*] = ;         ParserSetValueByKeyByHandle(pRMGenerator, 'maskname', inputbitmap);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   srcbitmappath := folderpath+'\'+inputbitmap;
      [*] = ;   if (not IsFileExists(srcbitmappath)) then
      [*] = ;   begin
      [*] = ;      ErrorLog('not exist '+srcbitmappath);
      [*] = ;      exit;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (mapwidth<>mapheight) then
      [*] = ;begin
      [*] = ;   ErrorLog('mapwidth<>mapheight');
      [*] = ;   //exit;
      [*] = ;end;
      [*] = ;
      [*] = ;SetRandomExtKey64(randkey0, randkey1);
      [*] = ;
      [*] = ;procedure _misc_SmoothTerrainCustom(x, y, times : Integer; power, lerp, minh, maxh : Float);
      [*] = ;begin
      [*] = ;   if (power=0) then
      [*] = ;   exit;
      [*] = ;   var lt, rt, lb, rb, h : Float;
      [*] = ;   var z1, z2 : Float;
      [*] = ;   var minx, miny : Float;
      [*] = ;   _misc_GetMapLeftTopPos(minx, miny);
      [*] = ;   var i : Integer;
      [*] = ;   for i:=0 to times-1 do
      [*] = ;   if (x>minx) and (x<(mapwidth div 2)) and (y>miny) and (y<(mapwidth div 2)) then
      [*] = ;   begin
      [*] = ;      lt := GetHeightData(x-1, y-1);
      [*] = ;      rt := GetHeightData(x+1, y-1);
      [*] = ;      lb := GetHeightData(x-1, y+1);
      [*] = ;      rb := GetHeightData(x+1, y+1);
      [*] = ;      h := GetHeightData(x, y);
      [*] = ;      if (h<>lt) or (h<>rt) or (h<>lb) or (h<>rb) then
      [*] = ;      if h>=minh and h<=maxh then
      [*] = ;      begin
      [*] = ;         z1 := (lt+rt)/2;
      [*] = ;         z2 := (lb+rb)/2;
      [*] = ;         var res : Float = h+((z1+z2)/2-h)/power;
      [*] = ;         res := Lerp(h, res, lerp);
      [*] = ;         SetHeightData(x, y, res);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;var envplHnd : Integer;
      [*] = ;if (GetCountOfPlayers>gc_playerind_env) then
      [*] = ;envplHnd := GetPlayerHandleByIndex(gc_playerind_env)
      [*] = ;else
      [*] = ;exit;
      [*] = ;
      [*] = ;var lx, ty : Float;
      [*] = ;_misc_GetMapLeftTopPos(lx, ty);
      [*] = ;
      [*] = ;var x, y : Integer;
      [*] = ;if (cResetTerrainHeight) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Reset terrain height');
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   if (bRandomMap) then
      [*] = ;   ResetPatternMap(False)
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      PlateauTerrain(0, 0, False, ((mapheight div 2)-1), True);
      [*] = ;      for y:=0 to mapheight do
      [*] = ;      for x:=0 to mapwidth do
      [*] = ;      SetTileIndex(x, y, 0);
      [*] = ;
      [*] = ;      ClearPlayerGameObjectsByHandle(envplHnd);
      [*] = ;      LoadPatterns(True, True);
      [*] = ;      for y:=0 to gc_terraindata_mapheight-1 do
      [*] = ;      for x:=0 to gc_terraindata_mapwidth-1 do
      [*] = ;      begin
      [*] = ;         var realx : Integer = floor(lx+x);
      [*] = ;         var realy : Integer = floor(ty+y);
      [*] = ;         MapDrawCollision(realx+0.5, realy+0.5, 0, 2, True); // clear
      [*] = ;      end;
      [*] = ;      ResetPatternMap(False);
      [*] = ;   end;
      [*] = ;
      [*] = ;   _misc_ProfilerEnd('Reset terrain height');
      [*] = ;end;
      [*] = ;
      [*] = ;var srcbitmap : Integer;
      [*] = ;if (bTGA) then
      [*] = ;srcbitmap := CreateBitmapTGA
      [*] = ;else
      [*] = ;srcbitmap := CreateBitmap;
      [*] = ;LoadBitmap(srcbitmap, srcbitmappath);
      [*] = ;var srcwidth, srcheight : Integer;
      [*] = ;GetBitmapSize(srcbitmap, srcwidth, srcheight);
      [*] = ;ApplicationProcessMessages;
      [*] = ;
      [*] = ;if (srcwidth<>mapwidth) or (srcheight<>mapheight) then
      [*] = ;begin
      [*] = ;   //ErrorLog('texture width height not equal to map width height');
      [*] = ;   //FreeBitmap(srcbitmap);
      [*] = ;   //exit;
      [*] = ;end;
      [*] = ;
      [*] = ;_misc_ProfilerBegin('Analyse bitmap');
      [*] = ;var spInd : Integer;
      [*] = ;for spInd:=0 to gc_MaxPlayerCount-1 do
      [*] = ;begin
      [*] = ;   gMap.players[spInd].startx := -10000;
      [*] = ;   gMap.players[spInd].starty := -10000;
      [*] = ;end;
      [*] = ;spInd := 0;
      [*] = ;
      [*] = ;for y:=0 to mapheight-1 do
      [*] = ;for x:=0 to mapwidth-1 do
      [*] = ;gPatternMask[x,y] := False;
      [*] = ;
      [*] = ;if (srcwidth>mapwidth) then // if map lesser size then bmp (less 256)
      [*] = ;begin
      [*] = ;   var bitmapx, bitmapy : Integer;
      [*] = ;   for bitmapy:=0 to srcheight-1 do
      [*] = ;   for bitmapx:=0 to srcwidth-1 do
      [*] = ;   begin
      [*] = ;      var fr, fg, fb, fa : Float;
      [*] = ;      var x : Integer = floor((bitmapx/srcwidth)*mapwidth);
      [*] = ;      var y : Integer = floor((bitmapy/srcheight)*mapheight);
      [*] = ;      GetBitmapPixel(srcbitmap, bitmapx, bitmapy, fr, fg, fb, fa);
      [*] = ;      if (fr=1) and (fg=1) and (fb=1) then // starting position
      [*] = ;      begin
      [*] = ;         if (spInd<gc_MaxPlayerCount) then
      [*] = ;         begin
      [*] = ;            if (spInd=0) or (gMap.players[spInd-1].startx<>bitmapx) or (gMap.players[spInd-1].starty<>bitmapy) then
      [*] = ;            begin
      [*] = ;               gMap.players[spInd].startx := bitmapx;
      [*] = ;               gMap.players[spInd].starty := bitmapy;
      [*] = ;               spInd := spInd+1;
      [*] = ;            end;
      [*] = ;         end
      [*] = ;         else
      [*] = ;         ErrorLog('spInd>gc_MaxPlayerCount');
      [*] = ;         fr := 0;
      [*] = ;         fg := 0;
      [*] = ;         fb := 0;
      [*] = ;      end;
      [*] = ;      var ir : Integer = floor(fr*255);
      [*] = ;      var ig : Integer = floor(fg*255);
      [*] = ;      var ib : Integer = floor(fb*255);
      [*] = ;      gMapMask[x,y].r := fr;
      [*] = ;      gMapMask[x,y].g := fg;
      [*] = ;      gMapMask[x,y].b := fb;
      [*] = ;      if (fa>0) then
      [*] = ;      gMapMask[x,y].broad := True
      [*] = ;      else
      [*] = ;      gMapMask[x,y].broad := False;
      [*] = ;      case ir of
      [*] = ;         cRColorPlateau, cRColorPlateauSmooth : begin
      [*] = ;            gMapMask[x,y].bsmooth := (ir=cRColorPlateauSmooth);
      [*] = ;            gMapMask[x,y].terrain := cRPlateau;
      [*] = ;         end;
      [*] = ;         cRColorHill, cRColorHillSmooth : begin
      [*] = ;            gMapMask[x,y].bsmooth := (ir=cRColorHillSmooth);
      [*] = ;            gMapMask[x,y].terrain := cRHill;
      [*] = ;         end;
      [*] = ;         cRColorRavine, cRColorRavineSmooth : begin
      [*] = ;            gMapMask[x,y].bsmooth := (ir=cRColorRavineSmooth);
      [*] = ;            gMapMask[x,y].terrain := cRRavine;
      [*] = ;         end;
      [*] = ;         else
      [*] = ;         begin
      [*] = ;            gMapMask[x,y].bsmooth := (ir=cRColorSmooth);
      [*] = ;            gMapMask[x,y].terrain := cRNone;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      case ig of
      [*] = ;         cGColorForest : gMapMask[x,y].forest := cGForest;
      [*] = ;         (cGColorForest div 2) : begin
      [*] = ;            case gMapMask[x,y].terrain of
      [*] = ;               cRPlateau, cRHill, cRRavine : gMapMask[x,y].forest := cGForest;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         cGColorGenForest : gMapMask[x,y].forest := cGGenForest;
      [*] = ;         cGColorStone : gMapMask[x,y].forest := cGStone;
      [*] = ;         cGColorForestStone : gMapMask[x,y].forest := cGForestStone;
      [*] = ;         else
      [*] = ;         gMapMask[x,y].forest := cGNone;
      [*] = ;      end;
      [*] = ;      case ib of
      [*] = ;         cBColorWater : gMapMask[x,y].water := cBWater;
      [*] = ;         else
      [*] = ;         gMapMask[x,y].water := cBNone;
      [*] = ;      end;
      [*] = ;      if ((ir=0) and (ig=0) and (ib=0))
      [*] = ;      or ((ir=255) and (ig=255) and (ib=255)) then
      [*] = ;      gPatternMask[x,y] := False
      [*] = ;      else
      [*] = ;      gPatternMask[x,y] := True;
      [*] = ;   end;
      [*] = ;end
      [*] = ;else
      [*] = ;for y:=0 to mapheight-1 do
      [*] = ;for x:=0 to mapwidth-1 do
      [*] = ;begin
      [*] = ;   var fr, fg, fb, fa : Float;
      [*] = ;   var bitmapx, bitmapy : Integer;
      [*] = ;   if (mapwidth=srcwidth) and (mapheight=srcheight) then
      [*] = ;   begin
      [*] = ;      bitmapx := x;
      [*] = ;      bitmapy := y;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      bitmapx := floor((x/mapwidth)*srcwidth);
      [*] = ;      bitmapy := floor((y/mapheight)*srcheight);
      [*] = ;   end;
      [*] = ;   GetBitmapPixel(srcbitmap, bitmapx, bitmapy, fr, fg, fb, fa);
      [*] = ;   if (fr=1) and (fg=1) and (fb=1) then // starting position
      [*] = ;   begin
      [*] = ;      if (spInd<gc_MaxPlayerCount) then
      [*] = ;      begin
      [*] = ;         if (spInd=0) or (gMap.players[spInd-1].startx<>bitmapx) or (gMap.players[spInd-1].starty<>bitmapy) then
      [*] = ;         begin
      [*] = ;            // check if this is not dublicate, as it can be for huge map size, causes of scaling coordinates.
      [*] = ;            var bdubl : Boolean;
      [*] = ;            var i : Integer;
      [*] = ;            for i:=0 to spInd-1 do
      [*] = ;            begin
      [*] = ;               if (gMap.players[i].startx=bitmapx) and (gMap.players[i].starty=bitmapy) then
      [*] = ;               begin
      [*] = ;                  bdubl := True;
      [*] = ;                  break;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;            if (not bdubl) then
      [*] = ;            begin
      [*] = ;               gMap.players[spInd].startx := bitmapx;
      [*] = ;               gMap.players[spInd].starty := bitmapy;
      [*] = ;               spInd := spInd+1;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      ErrorLog('spInd>gc_MaxPlayerCount');
      [*] = ;      fr := 0;
      [*] = ;      fg := 0;
      [*] = ;      fb := 0;
      [*] = ;   end;
      [*] = ;   var ir : Integer = floor(fr*255);
      [*] = ;   var ig : Integer = floor(fg*255);
      [*] = ;   var ib : Integer = floor(fb*255);
      [*] = ;   gMapMask[x,y].r := fr;
      [*] = ;   gMapMask[x,y].g := fg;
      [*] = ;   gMapMask[x,y].b := fb;
      [*] = ;   if (fa>0) then
      [*] = ;   gMapMask[x,y].broad := True
      [*] = ;   else
      [*] = ;   gMapMask[x,y].broad := False;
      [*] = ;   case ir of
      [*] = ;      cRColorPlateau, cRColorPlateauSmooth : begin
      [*] = ;         gMapMask[x,y].bsmooth := (ir=cRColorPlateauSmooth);
      [*] = ;         gMapMask[x,y].terrain := cRPlateau;
      [*] = ;      end;
      [*] = ;      cRColorHill, cRColorHillSmooth : begin
      [*] = ;         gMapMask[x,y].bsmooth := (ir=cRColorHillSmooth);
      [*] = ;         gMapMask[x,y].terrain := cRHill;
      [*] = ;      end;
      [*] = ;      cRColorRavine, cRColorRavineSmooth : begin
      [*] = ;         gMapMask[x,y].bsmooth := (ir=cRColorRavineSmooth);
      [*] = ;         gMapMask[x,y].terrain := cRRavine;
      [*] = ;      end;
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         gMapMask[x,y].bsmooth := (ir=cRColorSmooth);
      [*] = ;         gMapMask[x,y].terrain := cRNone;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   case ig of
      [*] = ;      cGColorForest : gMapMask[x,y].forest := cGForest;
      [*] = ;      (cGColorForest div 2) : begin
      [*] = ;         case gMapMask[x,y].terrain of
      [*] = ;            cRPlateau, cRHill, cRRavine : gMapMask[x,y].forest := cGForest;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      cGColorGenForest : gMapMask[x,y].forest := cGGenForest;
      [*] = ;      cGColorStone : gMapMask[x,y].forest := cGStone;
      [*] = ;      cGColorForestStone : gMapMask[x,y].forest := cGForestStone;
      [*] = ;      else
      [*] = ;      gMapMask[x,y].forest := cGNone;
      [*] = ;   end;
      [*] = ;   case ib of
      [*] = ;      cBColorWater : gMapMask[x,y].water := cBWater;
      [*] = ;      else
      [*] = ;      gMapMask[x,y].water := cBNone;
      [*] = ;   end;
      [*] = ;   if ((ir=0) and (ig=0) and (ib=0))
      [*] = ;   or ((ir=255) and (ig=255) and (ib=255)) then
      [*] = ;   gPatternMask[x,y] := False
      [*] = ;   else
      [*] = ;   gPatternMask[x,y] := True;
      [*] = ;end;
      [*] = ;var i : Integer;
      [*] = ;for i:=0 to spInd-1 do
      [*] = ;begin
      [*] = ;   gMap.players[i].startx := -(GetMapWidth div 2)+(gMap.players[i].startx/srcwidth)*mapwidth;
      [*] = ;   gMap.players[i].starty := -(GetMapHeight div 2)+(gMap.players[i].starty/srcheight)*mapheight;
      [*] = ;end;
      [*] = ;_misc_ProfilerEnd('Analyse bitmap');
      [*] = ;
      [*] = ;var trgbitmap : Integer;
      [*] = ;var trgwidth, trgheight : Integer;
      [*] = ;if (not bRandomMap) then
      [*] = ;begin
      [*] = ;   if (cPostProcessBitmapGaussian) then
      [*] = ;   begin
      [*] = ;      _misc_ProfilerBegin('Postprocess bitmap gaussian');
      [*] = ;      ApplicationProcessMessages;
      [*] = ;      SetRandomExtKey64(randkey0, randkey1);
      [*] = ;      ExecuteState('PostProcess');
      [*] = ;      ApplicationProcessMessages;
      [*] = ;      _misc_ProfilerEnd('Postprocess bitmap gaussian');
      [*] = ;   end;
      [*] = ;
      [*] = ;   trgbitmap := CreateBitmap;
      [*] = ;   LoadBitmap(trgbitmap, '.\data\gen\bitmap\ext\bitmaptrg.bmp');
      [*] = ;   GetBitmapSize(trgbitmap, trgwidth, trgheight);
      [*] = ;   if (trgwidth=0) or (trgheight=0) then
      [*] = ;   begin
      [*] = ;      ErrorLog('trgbitmap not valid');
      [*] = ;      FreeBitmap(trgbitmap);
      [*] = ;      exit;
      [*] = ;   end;
      [*] = ;end
      [*] = ;else
      [*] = ;begin
      [*] = ;   trgbitmap := srcbitmap;
      [*] = ;   trgwidth := srcwidth;
      [*] = ;   trgheight := srcheight;
      [*] = ;end;
      [*] = ;
      [*] = ;ApplicationProcessMessages;
      [*] = ;for y:=0 to mapheight-1 do
      [*] = ;for x:=0 to mapwidth-1 do
      [*] = ;begin
      [*] = ;   var fr, fg, fb, fa : Float;
      [*] = ;   var bitmapx : Integer = floor((x/mapwidth)*trgwidth);
      [*] = ;   var bitmapy : Integer = floor((y/mapheight)*trgheight);
      [*] = ;   GetBitmapPixel(trgbitmap, bitmapx, bitmapy, fr, fg, fb, fa);
      [*] = ;   gMapMask[x,y].r := fr;
      [*] = ;   //gMapMask[x,y].g := fg;
      [*] = ;   gMapMask[x,y].b := fb;
      [*] = ;end;
      [*] = ;
      [*] = ;if (trgwidth>gc_terraindata_mapwidth) or (trgheight>gc_terraindata_mapheight) then
      [*] = ;ErrorLog('(trgwidth>gc_terraindata_mapwidth) or (trgheight>gc_terraindata_mapheight)');
      [*] = ;
      [*] = ;if (cLoadHeightDataFromTexture) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Load height data from texture');
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   for y:=0 to mapheight do
      [*] = ;   for x:=0 to mapwidth do
      [*] = ;   begin
      [*] = ;      var realx : Integer = floor(lx+x);
      [*] = ;      var realy : Integer = floor(ty+y);
      [*] = ;      var ix : Integer = ClampInt(x, 0, mapwidth-1);
      [*] = ;      var iy : Integer = ClampInt(y, 0, mapheight-1);
      [*] = ;      var fr : Float = gMapMask[ix,iy].r;
      [*] = ;      var fb : Float = gMapMask[ix,iy].b;
      [*] = ;      if (bRandomMap) and (fr=1) and (fb=1) then // starting point
      [*] = ;      continue;
      [*] = ;      var hwater : Float = cWaterHeight*(fb*(1+fr*3.5));
      [*] = ;      var hterrain : Float;
      [*] = ;      case gMapMask[ix,iy].terrain of
      [*] = ;         cRPlateau : hterrain := cPlateauHeight*fr;
      [*] = ;         cRHill : hterrain := cHillHeight*fr;//*(255/cRColorHill); - required if color range of height is 0-32 0-96 in trgbitmap
      [*] = ;         cRRavine : hterrain := cRavineHeight*fr;//*(255/cRColorRavine);
      [*] = ;      end;
      [*] = ;      SetHeightData(realx, realy, hwater+hterrain)
      [*] = ;   end;
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   _misc_ProfilerEnd('Load height data from texture');
      [*] = ;end;
      [*] = ;
      [*] = ;if (cSmoothTerrainUsingBitmap) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('SmoothTerrain');
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   for y:=0 to mapheight-1 do
      [*] = ;   for x:=0 to mapwidth-1 do
      [*] = ;   begin
      [*] = ;      if (gMapMask[x,y].bsmooth) then
      [*] = ;      begin
      [*] = ;         var realx : Integer = floor(lx+x);
      [*] = ;         var realy : Integer = floor(ty+y);
      [*] = ;         SmoothTerrain(realx, -realy, False, 3);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   _misc_ProfilerEnd('SmoothTerrain');
      [*] = ;end;
      [*] = ;
      [*] = ;if (cSmoothAllMapHeight) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Smooth all map height');
      [*] = ;   for y:=0 to mapheight-1 do
      [*] = ;   for x:=0 to mapwidth-1 do
      [*] = ;   begin
      [*] = ;      var realx : Integer = floor(lx+x);
      [*] = ;      var realy : Integer = floor(ty+y);
      [*] = ;      _misc_SmoothTerrainCustom(realx, realy, 1, 4, 0.5, 0.01, 100);
      [*] = ;      //_misc_SmoothTerrainCustom(realx, realy, 1, 4, 0.5, -100, 0.01);
      [*] = ;   end;
      [*] = ;   ApplicationProcessMessages;
      [*] = ;
      [*] = ;   var i : Integer;
      [*] = ;   for i:=0 to 2 do
      [*] = ;   begin
      [*] = ;      for y:=0 to mapheight do
      [*] = ;      for x:=0 to mapwidth do
      [*] = ;      begin
      [*] = ;         var realx : Integer = floor(lx+x);
      [*] = ;         var realy : Integer = floor(ty+y);
      [*] = ;         var ix : Integer = ClampInt(x, 0, mapwidth-1);
      [*] = ;         var iy : Integer = ClampInt(y, 0, mapheight-1);
      [*] = ;         var bWater : Boolean = (gMapMask[ix,iy].water=cBWater);
      [*] = ;         if (not bWater) then
      [*] = ;         begin
      [*] = ;            var nx, ny : Integer;
      [*] = ;            const rad = 2;
      [*] = ;            for [MAIN]ny:=ClampInt(y-rad, 0, mapwidth-1) to ClampInt(y+rad, 0, mapwidth-1) do
      [*] = ;            for nx:=ClampInt(x-rad, 0, mapwidth-1) to ClampInt(x+rad, 0, mapwidth-1) do
      [*] = ;            begin
      [*] = ;               if (gMapMask[nx,ny].water=cBWater) then
      [*] = ;               begin
      [*] = ;                  bWater := True;
      [*] = ;                  break(MAIN);
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;         if (bWater) then
      [*] = ;         _misc_SmoothTerrainCustom(realx, realy, 1, 0.35, 0.5, -100, -0.01);
      [*] = ;      end;
      [*] = ;      ApplicationProcessMessages;
      [*] = ;   end;
      [*] = ;   TerrainUpdate(True, False);
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   _misc_ProfilerEnd('Smooth all map height');
      [*] = ;end;
      [*] = ;
      [*] = ;if (cPlaceWater) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Placing water');
      [*] = ;   const cWaterFieldSize = 2;
      [*] = ;   const cWaterMapArrSize = (640 div cWaterFieldSize);
      [*] = ;   var bWaterMap : array [0..cWaterMapArrSize] of array [0..cWaterMapArrSize] of Boolean;
      [*] = ;   var wmx, wmy : Integer;
      [*] = ;   for wmy:=0 to cWaterMapArrSize-1 do
      [*] = ;   for wmx:=0 to cWaterMapArrSize-1 do
      [*] = ;   begin
      [*] = ;      for [CELL]y:=wmy*cWaterFieldSize to Min(((wmy+1)*cWaterFieldSize), mapheight-1) do
      [*] = ;      for x:=wmx*cWaterFieldSize to Min(((wmx+1)*cWaterFieldSize), mapwidth-1) do
      [*] = ;      begin
      [*] = ;         var realx : Integer = floor(lx+x);
      [*] = ;         var realy : Integer = floor(ty+y);
      [*] = ;         if (x<0) or (x>=gc_terraindata_mapwidth) or (y<0) or (y>=gc_terraindata_mapheight) then
      [*] = ;         begin
      [*] = ;            ErrorLog('(x<0) or (x>=gc_terraindata_mapwidth) or (y<0) or (y>=gc_terraindata_mapheight)');
      [*] = ;            continue;
      [*] = ;         end;
      [*] = ;         if (gMapMask[x,y].b>0) and ((gMapMask[x,y].r<1) or (gMapMask[x,y].b<1)) then
      [*] = ;         begin
      [*] = ;            if (wmx<0) or (wmx>cWaterMapArrSize) or (wmy<0) or (wmy>cWaterMapArrSize) then
      [*] = ;            begin
      [*] = ;               ErrorLog('(wmx<0) or (wmx>cWaterMapArrSize) or (wmy<0) or (wmy>cWaterMapArrSize)');
      [*] = ;               continue;
      [*] = ;            end;
      [*] = ;            bWaterMap[wmx, wmy] := True;
      [*] = ;            break(CELL);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   ApplicationProcessMessages;
      [*] = ;
      [*] = ;   {for wmy:=0 to cWaterMapArrSize do
      [*] = ;   for wmx:=0 to cWaterMapArrSize do
      [*] = ;   begin
      [*] = ;      //if (not bWaterMap[wmx, wmy]) and ((bWaterMap[wmx+1, wmy]) or (bWaterMap[wmx, wmy+1]) or (bWaterMap[wmx+1, wmy+1])) then
      [*] = ;      //bWaterMap[wmx, wmy] := True;
      [*] = ;   end;}
      [*] = ;
      [*] = ;   WaterFieldsClear;
      [*] = ;   const cOffsetY = -0.5;
      [*] = ;   var mapsizemod : Float = (cWaterMapArrSize/((640/mapheight)*2))*cWaterFieldSize; // var mapsizemod : Float = (cWaterMapArrSize div ((640 div mapheight)*2))*cWaterFieldSize;
      [*] = ;   for wmy:=0 to cWaterMapArrSize-1 do
      [*] = ;   for wmx:=0 to cWaterMapArrSize-1 do
      [*] = ;   begin
      [*] = ;      if (wmx<0) or (wmx>=cWaterMapArrSize) or (wmy<0) or (wmy>=cWaterMapArrSize) then
      [*] = ;      begin
      [*] = ;         ErrorLog('(wmx<0) or (wmx>cWaterMapArrSize) or (wmy<0) or (wmy>cWaterMapArrSize)');
      [*] = ;         continue;
      [*] = ;      end;
      [*] = ;      if (bWaterMap[wmx, wmy]) then
      [*] = ;      begin
      [*] = ;         var tx : Float = wmx*cWaterFieldSize-mapsizemod;
      [*] = ;         var tz : Float = wmy*cWaterFieldSize-mapsizemod;
      [*] = ;         var bx : Float = (wmx+1)*cWaterFieldSize-mapsizemod;
      [*] = ;         var bz : Float = (wmy+1)*cWaterFieldSize-mapsizemod;
      [*] = ;         WaterFieldAdd('x'+IntToStr(wmx)+'y'+IntToStr(wmy), tx, tz, bx, bz, cOffsetY);
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   _misc_ProfilerEnd('Placing water');
      [*] = ;end;
      [*] = ;
      [*] = ;if (cPostStandPatterns) and (not bRandomMap) then
      [*] = ;begin
      [*] = ;   var pPostPatterns : Integer = _parser_ParserTemporary(True);
      [*] = ;   ParserLoadFromFileByHandle(pPostPatterns, '.\data\gen\bitmap\postpatterns.cfg');
      [*] = ;   var pMissions : Integer = ParserSelectByHandleByKey(pPostPatterns, 'Missions');
      [*] = ;   var tmpPatternMask : array [0..640-1] of array [0..640-1] of Boolean;
      [*] = ;
      [*] = ;   var i, j : Integer;
      [*] = ;   for i:=0 to ParserGetCountByHandle(pMissions)-1 do
      [*] = ;   begin
      [*] = ;      var pMission : Integer = ParserSelectByHandleByIndex(pMissions, i);
      [*] = ;      var name : String = ParserGetValueByKeyByHandle(pMission, 'MissionName');
      [*] = ;      if (name=missionname) then
      [*] = ;      begin
      [*] = ;         var pPatterns : Integer = ParserSelectByHandleByKey(pMission, 'Patterns');
      [*] = ;         for j:=0 to ParserGetCountByHandle(pPatterns)-1 do
      [*] = ;         begin
      [*] = ;            var pPattern : Integer = ParserSelectByHandleByIndex(pPatterns, j);
      [*] = ;            var patternName : String = ParserGetValueByKeyByHandle(pPattern, 'PatternName');
      [*] = ;            var x : Float = ParserGetFloatValueByKeyByHandle(pPattern, 'X');
      [*] = ;            var y : Float = ParserGetFloatValueByKeyByHandle(pPattern, 'Y');
      [*] = ;            _misc_StandPatternPlaceMask(patternName, x, y, 0, tmpPatternMask);
      [*] = ;            if StrExists(patternName, 'mng') or StrExists(patternName, 'mni') or StrExists(patternName, 'mnc') then
      [*] = ;            begin
      [*] = ;               const r = 5;
      [*] = ;               var mapW : Integer = GetMapWidth;
      [*] = ;               var mapH : Integer = GetMapHeight;
      [*] = ;               var k, l : Integer;
      [*] = ;               for k:=-round(r) to floor(r*2)-1 do
      [*] = ;               for l:=-round(r) to floor(r*2)-1 do
      [*] = ;               begin
      [*] = ;                  if (VectorDistance(x, 0, y, x+l, 0, y+k)<r) then
      [*] = ;                  begin
      [*] = ;                     var indx : Integer = floor(x+(mapW div 2)+l);
      [*] = ;                     var indy : Integer = floor(y+2+(mapH div 2)+k); // +2 is visual offset
      [*] = ;                     if (indx>=0) and (indx<gc_terraindata_mapwidth) and (indy>=0) and (indy<gc_terraindata_mapheight) then
      [*] = ;                     tmpPatternMask[indx, indy] := True;
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   for y:=0 to mapheight-1 do
      [*] = ;   for x:=0 to mapwidth-1 do
      [*] = ;   begin
      [*] = ;      if (tmpPatternMask[x,y]) then
      [*] = ;      begin
      [*] = ;         for i:=-1 to 1 do
      [*] = ;         for j:=-1 to 2 do
      [*] = ;         if (gMapMask[x,y].forest=cGForest) then
      [*] = ;         begin
      [*] = ;            if ((x+i)>=0) and ((x+i)<mapwidth) and ((y+j)>=0) and ((y+j)<mapheight) then
      [*] = ;            gMapMask[x+i,y+j].forest := cGNone;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (cTextureTerrain) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Texture terrain');
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   SetRandomExtKey64(randkey0, randkey1);
      [*] = ;   IntRegister0 := forestType;
      [*] = ;   ExecuteState('TextureMap');
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   _misc_ProfilerEnd('Texture terrain');
      [*] = ;end;
      [*] = ;
      [*] = ;if (cSmoothTile) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Smooth tile');
      [*] = ;   //MapGeneratorSmoothTiles;
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   ExecuteState('SmoothTiles');
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   _misc_ProfilerEnd('Smooth tile');
      [*] = ;end;
      [*] = ;
      [*] = ;if (cLoadTextureFromBitmap) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Load texture from bitmap');
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   ExecuteState('LoadTextureTerrainFromBitmap');
      [*] = ;   ApplicationProcessMessages;
      [*] = ;   _misc_ProfilerEnd('Load texture from bitmap');
      [*] = ;end;
      [*] = ;
      [*] = ;var lastenvcount : Integer = GetPlayerGameObjectsCountByHandle(envPlHnd);
      [*] = ;if (cPlaceFreeForests) and (not bRandomMap) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Place free forests');
      [*] = ;
      [*] = ;   var envplHnd : Integer = GetPlayerHandleByIndex(gc_playerind_env);
      [*] = ;   LoadPatterns(True, True);
      [*] = ;   //for y:=0 to gc_terraindata_mapheight-1 do
      [*] = ;   //for x:=0 to gc_terraindata_mapwidth-1 do
      [*] = ;   //gPatternMask[x, y] := False;
      [*] = ;   //ResetPatternMap(False); // no need to reset
      [*] = ;
      [*] = ;   //var area : Integer = mapwidth*mapheight;
      [*] = ;   for y:=0 to mapheight-1 do
      [*] = ;   for x:=0 to mapwidth-1 do
      [*] = ;   begin
      [*] = ;      var realx : Integer = floor(lx+x);
      [*] = ;      var realy : Integer = floor(ty+y);
      [*] = ;      if ((not bRandomMap) and (gMapMask[x,y].forest<>cGGenForest)) or (gMapMask[x,y].bsmooth) or (gMapMask[x,y].water<>cBNone) or (gMapMask[x,y].broad) {or (gMapMask[x,y].terrain<>cRNone)}or (GetMapCollisionTag(realx, realy, True)>0) then
      [*] = ;      begin
      [*] = ;         //area := area-1;
      [*] = ;         gPatternMask[x, y] := True;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   //var areamod : Float = (area/(mapwidth*mapheight));}
      [*] = ;
      [*] = ;   var pGenerator : Integer = _misc_GetOrCreateGeneratorParser(True);
      [*] = ;   var probsmall, probmid, problarge, probhuge : Float;
      [*] = ;   var pln_small, pln_mid, pln_large, pln_huge : Float;
      [*] = ;   _misc_GetFreePatternMaskModifier(probsmall, probmid, problarge, probhuge);
      [*] = ;   const plain = 0.001;
      [*] = ;   pln_small := plain*probsmall/2;
      [*] = ;   pln_mid := plain*probmid*2;
      [*] = ;   pln_large := plain*problarge;
      [*] = ;   pln_huge := plain*probhuge/4;
      [*] = ;   _misc_SetupPatternsByType(pGenerator, envplHnd, 'plain_huge', pln_huge);
      [*] = ;   _misc_SetupPatternsByType(pGenerator, envplHnd, 'plain_big', pln_large);
      [*] = ;   _misc_SetupPatternsByType(pGenerator, envplHnd, 'plain_medium', pln_mid);
      [*] = ;   _misc_SetupPatternsByType(pGenerator, envplHnd, 'plain_small', pln_small);
      [*] = ;
      [*] = ;   //_misc_SetupPatternsByType(pGenerator, envplHnd, 'plain', 0.0002*areamod);
      [*] = ;   //_misc_SetupPatternsByType(pGenerator, envplHnd, 'plain_small', 0.0015*areamod);
      [*] = ;
      [*] = ;   {var frs_big : Float = 0.0006;
      [*] = ;   var frs_mid : Float = 0.0004;
      [*] = ;   var frs_small : Float = 0.000275;
      [*] = ;   var dcr : Float = 0.0005;
      [*] = ;   var stn1 : Float = 0.00017;
      [*] = ;   var stn2 : Float = 0.00011;
      [*] = ;
      [*] = ;   var probsmall, probmid, problarge : Float;
      [*] = ;   _misc_GetFreePatternMaskModifier(probsmall, probmid, problarge);
      [*] = ;   frs_big := frs_big*probsmall*4;
      [*] = ;   frs_mid := frs_mid*probmid*4;
      [*] = ;   frs_small := frs_small*problarge*4;
      [*] = ;   stn1 := stn1*probsmall;
      [*] = ;   stn2 := stn2*probsmall;
      [*] = ;
      [*] = ;   case forestType of
      [*] = ;      0 : begin
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_pinefir_big', frs_big/4);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_spruce_big', frs_big/4);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_pine_big', frs_big/4);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_pine_big_2', frs_big/4);
      [*] = ;
      [*] = ;         //SetupPatternsByType('stones', stn1);
      [*] = ;
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_pinefir_medium', frs_mid/3);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_spruce_medium', frs_mid/3);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_pine_medium', frs_mid/3);
      [*] = ;
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'stones', stn2);
      [*] = ;
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_pinefir_small', frs_small/2);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_pine_small', frs_small/2);
      [*] = ;      end;
      [*] = ;      1 : begin
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_mixed_big', frs_big);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_mixed_medium', frs_mid);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'stones', stn2);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_leaf_small', frs_small);
      [*] = ;      end;
      [*] = ;      2 : begin
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_leaf_big', frs_big/2);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_leaf_medium', frs_mid/2);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'stones', stn2);
      [*] = ;         _misc_SetupPatternsByType(pGenerator, envplHnd, 'forests_leaf_small', frs_small);
      [*] = ;      end;
      [*] = ;   end;}
      [*] = ;
      [*] = ;   //ParserSaveToFileByHandle(pGenerator, 'lastgen2');
      [*] = ;
      [*] = ;   _misc_ProfilerEnd('Place free forests');
      [*] = ;end;
      [*] = ;
      [*] = ;if (cPostStandPatterns) and (not bRandomMap) then
      [*] = ;begin
      [*] = ;   var pPostPatterns : Integer = _parser_ParserTemporary(True);
      [*] = ;   ParserLoadFromFileByHandle(pPostPatterns, '.\data\gen\bitmap\postpatterns.cfg');
      [*] = ;   var pMissions : Integer = ParserSelectByHandleByKey(pPostPatterns, 'Missions');
      [*] = ;   var i, j, k : Integer;
      [*] = ;   for i:=0 to ParserGetCountByHandle(pMissions)-1 do
      [*] = ;   begin
      [*] = ;      var pMission : Integer = ParserSelectByHandleByIndex(pMissions, i);
      [*] = ;      var name : String = ParserGetValueByKeyByHandle(pMission, 'MissionName');
      [*] = ;      if (name=missionname) then
      [*] = ;      begin
      [*] = ;         var pPatterns : Integer = ParserSelectByHandleByKey(pMission, 'Patterns');
      [*] = ;         for j:=0 to ParserGetCountByHandle(pPatterns)-1 do
      [*] = ;         begin
      [*] = ;            var pPattern : Integer = ParserSelectByHandleByIndex(pPatterns, j);
      [*] = ;            var patternName : String = ParserGetValueByKeyByHandle(pPattern, 'PatternName');
      [*] = ;            var x : Float = ParserGetFloatValueByKeyByHandle(pPattern, 'X');
      [*] = ;            var y : Float = ParserGetFloatValueByKeyByHandle(pPattern, 'Y');
      [*] = ;
      [*] = ;            if StrExists(patternName, 'mng') or StrExists(patternName, 'mni') or StrExists(patternName, 'mnc') then
      [*] = ;            begin
      [*] = ;               const r = 3;
      [*] = ;               GetGameObjectsInRadius(x, y, r, false, true, envPlHnd, -1, 0, false, false, false, false, false, false);
      [*] = ;               for k:=GetGameObjectListCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var goHnd : Integer = GetGameObjectListByIndex(k);
      [*] = ;                  var baseid : Integer = _misc_GetBaseObjID(goHnd);
      [*] = ;                  if (baseid=gc_baseid_nil) or (baseid=gc_baseid_res) then
      [*] = ;                  begin
      [*] = ;                     if (baseid=gc_baseid_res) then
      [*] = ;                     begin
      [*] = ;                        var pres : Pointer = _misc_GetObjectArgData(goHnd, gc_argenv_res);
      [*] = ;                        if (not ((TRes(pres).itype=gc_resource_type_wood) or (TRes(pres).itype=gc_resource_type_none))) then
      [*] = ;                        continue;
      [*] = ;                     end;
      [*] = ;                     var posX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;                     var posZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;                     if (VectorDistance(posX, 0, posZ, x, 0, y)<r) then
      [*] = ;                     GameObjectDestroyByHandle(goHnd);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;               // remove high trees
      [*] = ;               GetGameObjectsInRadius(x, y+3, r, false, true, envPlHnd, -1, 0, false, false, false, false, false, false);
      [*] = ;               for k:=GetGameObjectListCount-1 downto 0 do
      [*] = ;               begin
      [*] = ;                  var goHnd : Integer = GetGameObjectListByIndex(k);
      [*] = ;                  var baseid : Integer = _misc_GetBaseObjID(goHnd);
      [*] = ;                  if (baseid=gc_baseid_res) then
      [*] = ;                  begin
      [*] = ;                     var pres : Pointer = _misc_GetObjectArgData(goHnd, gc_argenv_res);
      [*] = ;                     if not (TRes(pres).itype=gc_resource_type_wood) then
      [*] = ;                     continue;
      [*] = ;                     var posX : Float = GetGameObjectPositionXByHandle(goHnd);
      [*] = ;                     var posZ : Float = GetGameObjectPositionZByHandle(goHnd);
      [*] = ;                     if (VectorDistance(posX, 0, posZ, x, 0, y+3)<r) then
      [*] = ;                     GameObjectDestroyByHandle(goHnd);
      [*] = ;                  end;
      [*] = ;               end;
      [*] = ;            end;
      [*] = ;
      [*] = ;            _misc_CheckStandPattern(patternName, envplHnd, x, y, 0, True, False, 0);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;for i:=GetPlayerGameObjectsCountByHandle(envplhnd)-1 downto lastenvcount do
      [*] = ;begin
      [*] = ;   var goHnd : Integer = GetGameObjectHandleByIndex(i, envplhnd);
      [*] = ;   var basename : String = GetGameObjectBaseNameByHandle(goHnd);
      [*] = ;   if StrExists(basename, 'terrainmesh') or (StrExists(basename, 'stones')) then
      [*] = ;   continue;
      [*] = ;   var scale : Float = GetGameObjectScaleXByHandle(goHnd);
      [*] = ;
      [*] = ;   var x, y, z, minfact, maxfact, deffact : Float;
      [*] = ;   var rand : Boolean;
      [*] = ;   GetGameObjectDefScalePropsByHandle(goHnd, x, y, z, rand, minfact, maxfact, deffact);
      [*] = ;   var scaleMid, scaleMin, scaleMax : Float;
      [*] = ;   if (rand) then
      [*] = ;   begin
      [*] = ;      scaleMin := x*minfact;
      [*] = ;      scaleMid := x*((minfact+maxfact)/2);
      [*] = ;      scaleMax := x*maxfact;
      [*] = ;
      [*] = ;      if ((Abs(scaleMax-scaleMin))>scaleMid*0.05) then
      [*] = ;      begin
      [*] = ;         if (scale<scaleMin) then
      [*] = ;         begin
      [*] = ;            const cDiffKoefMin = 0.875;
      [*] = ;            const cDiffKoefMax = 0.95;
      [*] = ;            var koef : Float = (scaleMin/scaleMid)*(cDiffKoefMin+random*(cDiffKoefMax-cDiffKoefMin));
      [*] = ;            var newscale : Float = scaleMid*koef;
      [*] = ;            if (newscale<scale) then
      [*] = ;            newscale := scale;
      [*] = ;            SetGameObjectScaleByHandle(goHnd, newscale, newscale, newscale);
      [*] = ;         end
      [*] = ;         else
      [*] = ;         if (scale>scaleMax) then
      [*] = ;         begin
      [*] = ;            const cDiffKoefMin = 1.04;
      [*] = ;            const cDiffKoefMax = 1.08;
      [*] = ;            var koef : Float = (scaleMax/scaleMid)*(cDiffKoefMin+random*(cDiffKoefMax-cDiffKoefMin));
      [*] = ;            var newscale : Float = scaleMid*koef;
      [*] = ;            if (newscale>scale) then
      [*] = ;            newscale := scale;
      [*] = ;            SetGameObjectScaleByHandle(goHnd, newscale, newscale, newscale);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;   if (z<>x) then
      [*] = ;   SetGameObjectScaleByHandle(goHnd, GetGameObjectScaleXByHandle(goHnd), GetGameObjectScaleYByHandle(goHnd), GetGameObjectScaleZByHandle(goHnd)*(z/x));
      [*] = ;end;
      [*] = ;
      [*] = ;if (cSeedWheat) and (not bRandomMap) then
      [*] = ;begin
      [*] = ;   if (GetCountOfPlayers>=gc_MaxPlayerCount) then
      [*] = ;   begin
      [*] = ;      var i, j : Integer;
      [*] = ;      for i:=0 to gc_MaxPlayerCount-1 do
      [*] = ;      begin
      [*] = ;         var plHnd : Integer = GetPlayerHandleByIndex(i);
      [*] = ;         for j:=GetPlayerGameObjectsCountByHandle(plHnd)-1 downto 0 do
      [*] = ;         begin
      [*] = ;            var goHnd : Integer = GetGameObjectHandleByIndex(j, plHnd);
      [*] = ;            var pobj : Pointer = _unit_GetTObj(goHnd);
      [*] = ;            if (pobj<>nil) and (gObjProp[TObj(pobj).cid][TObj(pobj).id].usage=gc_obj_usage_mill) and (TObj(pobj).bbuilt) and (not TObj(pobj).bdead) then
      [*] = ;            _unit_DoSeedWheat(goHnd, False);
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;if (cBuildTopology) and (not bRandomMap) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Build topology');
      [*] = ;   _misc_SetShoresCollision;
      [*] = ;   TopologyBuild;
      [*] = ;   _misc_BuildIslandsGrid;
      [*] = ;   _misc_ProfilerEnd('Build topology');
      [*] = ;end;
      [*] = ;
      [*] = ;if (cUpdateMinimap) and (not bRandomMap) then
      [*] = ;begin
      [*] = ;   _misc_ProfilerBegin('Update minimap');
      [*] = ;   //GUIMiniMapUpdate;
      [*] = ;   _misc_ProfilerEnd('Update minimap');
      [*] = ;end;
      [*] = ;
      [*] = ;FreeBitmap(srcbitmap);
      [*] = ;if (srcbitmap<>trgbitmap) then
      [*] = ;FreeBitmap(trgbitmap);
      [*] = ;
      [*] = ;var tmpbitmappath : String = '.\data\gen\bitmap\ext\';
      [*] = ;if (not bRandomMap) then
      [*] = ;begin
      [*] = ;   DeleteFileStream(tmpbitmappath+'bitmaprow.bmp');
      [*] = ;   DeleteFileStream(tmpbitmappath+'bitmaptrg.bmp');
      [*] = ;   DeleteFileStream(tmpbitmappath+'bitmaptrgnosmooth.bmp');
      [*] = ;end;
      [*] = ;//DeleteFileStream(tmpbitmappath+'tmp_env.bmp');
      [*] = ;//DeleteFileStream(tmpbitmappath+'tmp_forest_border.bmp');
   struct.end
section.end

