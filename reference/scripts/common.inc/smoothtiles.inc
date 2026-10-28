section.begin
   Name = SmoothTiles
   Code : struct.begin
      [*] = ;var pTileSmoothLists : Integer = _parser_ParserTemporary(True);
      [*] = ;ParserLoadFromFileByHandle(pTileSmoothLists, '.\data\gen\tilesmooth_scripts.cfg');
      [*] = ;pTileSmoothLists := ParserSelectByHandleByKey(pTileSmoothLists, 'TileSmoothLists');
      [*] = ;var Count : Integer = ParserGetCountByHandle(pTileSmoothLists);
      [*] = ;
      [*] = ;type TSmoothTile = class
      [*] = ;   TileName : String;
      [*] = ;   TileIndex : Integer;
      [*] = ;   Prob : Float;
      [*] = ;end;
      [*] = ;
      [*] = ;type TSmoothTileList = class
      [*] = ;   SumProb : Float;
      [*] = ;   TileCount : Integer;
      [*] = ;   SmoothTile : array [0..15] of TSmoothTile;
      [*] = ;end;
      [*] = ;
      [*] = ;type TItem = class
      [*] = ;   TileListsCount : Integer;
      [*] = ;   SmoothTiles : array [0..23] of TSmoothTileList;
      [*] = ;end;
      [*] = ;
      [*] = ;var Items : array [0..63] of TItem;
      [*] = ;var atilemap : array [0..640] of array [0..640] of Integer;
      [*] = ;
      [*] = ;var i, j, k : Integer;
      [*] = ;for j:=0 to GetMapHeight-1 do
      [*] = ;for i:=0 to GetMapWidth-1 do
      [*] = ;atilemap[i, j] := GetTileIndex(i, j);
      [*] = ;
      [*] = ;ApplicationProcessMessages;
      [*] = ;for i:=0 to ParserGetCountByHandle(pTileSmoothLists)-1 do
      [*] = ;begin
      [*] = ;   var pTileSmoothList : Integer = ParserSelectByHandleByIndex(pTileSmoothLists, i);
      [*] = ;   var pSmoothTilesLists : Integer = ParserSelectByHandleByKey(pTileSmoothList, 'SmoothTiles');
      [*] = ;   Items[i].TileListsCount := ParserGetCountByHandle(pSmoothTilesLists);
      [*] = ;   for j:=0 to Items[i].TileListsCount-1 do
      [*] = ;   begin
      [*] = ;      var sumProb : Float;
      [*] = ;      var pSmoothTileList : Integer = ParserSelectByHandleByIndex(pSmoothTilesLists, j);
      [*] = ;      var pTileLists : Integer = ParserSelectByHandleByKey(pSmoothTileList, 'TileNames');
      [*] = ;      var tileCount : Integer = ParserGetCountByHandle(pTileLists);
      [*] = ;      Items[i].SmoothTiles[j].TileCount := tileCount;
      [*] = ;      for k:=0 to Items[i].SmoothTiles[j].tileCount-1 do
      [*] = ;      begin
      [*] = ;         var pTileName : Integer = ParserSelectByHandleByIndex(pTileLists, k);
      [*] = ;         var tileName : String = ParserGetValueByKeyByHandle(pTileName, 'TileName');
      [*] = ;         var tileIndex : Integer = MapGetTileIndexByTileBlock(tileName);
      [*] = ;         var prob : Float = ParserGetFloatValueByKeyByHandle(pTileName, 'Prob');
      [*] = ;         sumProb := sumProb+prob;
      [*] = ;         Items[i].SmoothTiles[j].SmoothTile[k].Prob := prob;
      [*] = ;         Items[i].SmoothTiles[j].SmoothTile[k].TileName := tileName;
      [*] = ;         Items[i].SmoothTiles[j].SmoothTile[k].TileIndex := tileIndex;
      [*] = ;      end;
      [*] = ;      Items[i].SmoothTiles[j].SumProb := sumProb;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;ApplicationProcessMessages;
      [*] = ;
      [*] = ;function FindRandomTileIndex(i, j : Integer) : Integer;
      [*] = ;begin
      [*] = ;   Result := -1;
      [*] = ;   var sumProb : Float = Items[i].SmoothTiles[j].SumProb;
      [*] = ;   var rnd : Float = RandomExt*sumProb;
      [*] = ;   var tmp : Float;
      [*] = ;   var k : Integer;
      [*] = ;   for k:=0 to Items[i].SmoothTiles[j].TileCount-1 do
      [*] = ;   begin
      [*] = ;      var prob : Float = Items[i].SmoothTiles[j].SmoothTile[k].Prob;
      [*] = ;      tmp := tmp+prob;
      [*] = ;      if (prob>0) and (rnd<tmp) then
      [*] = ;      begin
      [*] = ;         Result := Items[i].SmoothTiles[j].SmoothTile[k].TileIndex;
      [*] = ;         exit;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// GetNeighbourTiles
      [*] = ;//
      [*] = ;procedure GetNeighbourTiles(tindex1, tindex2 : Integer; var outtile1, outtile2 : Integer);
      [*] = ;begin
      [*] = ;   var ind1, ind2 : Integer;
      [*] = ;   var i1, i2     : Integer;
      [*] = ;   var mindist    : Integer;
      [*] = ;   var i, j, k    : Integer;
      [*] = ;   var ilist      : Integer;
      [*] = ;   var tilename1  : String;
      [*] = ;   var tilename2  : String;
      [*] = ;   var i1ProbZero, i2ProbZero : Boolean;
      [*] = ;   var ind1ProbZero, ind2ProbZero : Boolean;
      [*] = ;   tilename1 := MapGetTileBlockByTileIndex(tindex1);
      [*] = ;   tilename2 := MapGetTileBlockByTileIndex(tindex2);
      [*] = ;
      [*] = ;   ilist := -1;
      [*] = ;   mindist := gc_MaxInt;
      [*] = ;   i1 := -1;
      [*] = ;   i2 := -1;
      [*] = ;   for i := 0 to Count-1 do
      [*] = ;   begin
      [*] = ;      ind1 := -1;
      [*] = ;      ind2 := -1;
      [*] = ;      ind1ProbZero := False;
      [*] = ;      ind2ProbZero := False;
      [*] = ;      for j := 0 to Items[i].TileListsCount-1 do
      [*] = ;      begin
      [*] = ;         for k := 0 to Items[i].SmoothTiles[j].TileCount-1 do
      [*] = ;         begin
      [*] = ;            if Items[i].SmoothTiles[j].SmoothTile[k].TileName = tilename1 then
      [*] = ;            begin
      [*] = ;               ind1 := j;
      [*] = ;               ind1ProbZero := (Items[i].SmoothTiles[j].SmoothTile[k].prob=0);
      [*] = ;            end;
      [*] = ;            if Items[i].SmoothTiles[j].SmoothTile[k].TileName = tilename2 then
      [*] = ;            begin
      [*] = ;               ind2 := j;
      [*] = ;               ind2ProbZero := (Items[i].SmoothTiles[j].SmoothTile[k].prob=0);
      [*] = ;            end;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;      if ((tilename1='rock5') and (tilename2='rock2')) or ((tilename1='rock2') and (tilename2='rock5')) then
      [*] = ;      ind2ProbZero := ind2ProbZero;
      [*] = ;      if (ind1<>ind2) then
      [*] = ;      if (ind1 > -1) and (ind2 > -1) then
      [*] = ;      if Abs(ind1 - ind2) < mindist then
      [*] = ;      begin
      [*] = ;         mindist := round(Abs(ind1 - ind2));
      [*] = ;         i1ProbZero := ind1ProbZero;
      [*] = ;         i2ProbZero := ind2ProbZero;
      [*] = ;         i1 := ind1;
      [*] = ;         i2 := ind2;
      [*] = ;         ilist := i;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;
      [*] = ;   if ilist > -1 then
      [*] = ;   begin
      [*] = ;      // tiles 1 and 2 are compatible
      [*] = ;      if (Abs(i1 - i2) = 1) and ((not i1ProbZero) or (not i2ProbZero)) then
      [*] = ;      begin
      [*] = ;         {if (i1ProbZero) or (i2ProbZero) then
      [*] = ;         begin
      [*] = ;            if (i1ProbZero) then
      [*] = ;            outtile1 := FindRandomTileIndex(ilist, i1 + );
      [*] = ;            if (i2ProbZero) then
      [*] = ;            outtile2 := FindRandomTileIndex(ilist, i2 + i1);
      [*] = ;         end
      [*] = ;         else}
      [*] = ;         begin
      [*] = ;            outtile1 := -1;
      [*] = ;            outtile2 := -1;
      [*] = ;         end;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      // tiles 1 and 2 are incompatible
      [*] = ;      if i1 - i2 = 2 then
      [*] = ;      begin
      [*] = ;         outtile1 := FindRandomTileIndex(ilist, (i1 + i2) div 2);
      [*] = ;         outtile2 := -1;
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if i2 - i1 = 2 then
      [*] = ;      begin
      [*] = ;         outtile1 := -1;
      [*] = ;         outtile2 := FindRandomTileIndex(ilist, (i1 + i2) div 2);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      if i1 < i2 then
      [*] = ;      begin
      [*] = ;         outtile1 := -1;
      [*] = ;         outtile2 := FindRandomTileIndex(ilist, i1 + 1);
      [*] = ;      end
      [*] = ;      else
      [*] = ;      begin
      [*] = ;         outtile1 := FindRandomTileIndex(ilist, i2 + 1);
      [*] = ;         outtile2 := -1;
      [*] = ;      end;
      [*] = ;   end
      [*] = ;   else
      [*] = ;   begin
      [*] = ;      outtile1 := -1;
      [*] = ;      outtile2 := -1;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;// SmoothTileMap
      [*] = ;//
      [*] = ;procedure SmoothTileMap(mini, minj, maxi, maxj, passes : Integer);
      [*] = ;begin
      [*] = ;   var tchange1, tchange2 : Integer;
      [*] = ;   var tind1, tind2       : Integer;
      [*] = ;   var tname1, tname2     : String;
      [*] = ;   var i, j, k            : Integer;
      [*] = ;   for k := 0 to passes-1 do
      [*] = ;   begin
      [*] = ;      ApplicationProcessMessages;
      [*] = ;      for i := mini to maxi-1 do
      [*] = ;      for j := minj to maxj do
      [*] = ;      begin
      [*] = ;         tind1 := atilemap[i, j];
      [*] = ;         tind2 := atilemap[i+1, j];
      [*] = ;         if tind1 <> tind2 then
      [*] = ;         begin
      [*] = ;            GetNeighbourTiles(tind1, tind2, tchange1, tchange2);
      [*] = ;
      [*] = ;            if tchange1 > -1 then
      [*] = ;            atilemap[i, j] := tchange1;
      [*] = ;
      [*] = ;            if tchange2 > -1 then
      [*] = ;            atilemap[i+1, j] := tchange2;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;
      [*] = ;      ApplicationProcessMessages;
      [*] = ;      for j := minj to maxj-1 do
      [*] = ;      for i := mini to maxi do
      [*] = ;      begin
      [*] = ;         tind1 := atilemap[i, j];
      [*] = ;         tind2 := atilemap[i, j+1];
      [*] = ;         if tind1 <> tind2 then
      [*] = ;         begin
      [*] = ;            GetNeighbourTiles(tind1, tind2, tchange1, tchange2);
      [*] = ;
      [*] = ;            if tchange1 > -1 then
      [*] = ;            atilemap[i, j] := tchange1;
      [*] = ;
      [*] = ;            if tchange2 > -1 then
      [*] = ;            atilemap[i, j+1] := tchange2;
      [*] = ;         end;
      [*] = ;      end;
      [*] = ;   end;
      [*] = ;end;
      [*] = ;
      [*] = ;SmoothTileMap(0, 0, GetMapWidth, GetMapHeight, 3);
      [*] = ;ApplicationProcessMessages;
      [*] = ;
      [*] = ;for j:=0 to GetMapHeight-1 do
      [*] = ;for i:=0 to GetMapWidth-1 do
      [*] = ;SetTileIndex(i, j, atilemap[i, j]);
      [*] = ;ApplicationProcessMessages;
   struct.end
section.end

