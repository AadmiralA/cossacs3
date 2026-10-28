$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$source = Join-Path $root 'reference\scripts\lib\miscext.script'
$output = Join-Path $root 'mod\data\scripts\lib\miscext.script'
if (-not (Test-Path -LiteralPath $source)) { throw "Missing reference: $source" }
$enc = [System.Text.Encoding]::Default
$original = [System.IO.File]::ReadAllText($source, $enc)
$anchor = '      [*] = ;         var pobj : Pointer = _misc_GetObjectArgData(hnd, gc_argunit_obj);'
if (($original.Split([string[]]@($anchor), [System.StringSplitOptions]::None).Count - 1) -ne 1) {
    throw 'Expected one collision callback insertion point; nothing written.'
}
$probe = @'
      [*] = ;         // NavalRamming: DIAGNOSTIC only, no damage.
      [*] = ;         if (pobj<>nil) and (not TObj(pobj).bdead) and (not TObj(pobj2).bdead) then
      [*] = ;         begin
      [*] = ;            var ourProp : Pointer = gObjProp[TObj(pobj).cid][TObj(pobj).id];
      [*] = ;            var theirProp : Pointer = gObjProp[TObj(pobj2).cid][TObj(pobj2).id];
      [*] = ;            if (TObjProp(ourProp).media=gc_obj_media_water) and (TObjProp(theirProp).media=gc_obj_media_water) then
      [*] = ;            if ((gPlayer[TObj(pobj).pl].enemyplmask and gPlayer[TObj(pobj2).pl].myplmask)<>0) then
      [*] = ;            Log('NavalRamming PROBE ship='+TObjProp(ourProp).sid+' target='+TObjProp(theirProp).sid);
      [*] = ;         end;
'@
$nl = [Environment]::NewLine
$patched = $original.Replace($anchor, $anchor + $nl + $probe.Replace([string][char]10, $nl).Replace([string][char]13, ''))
New-Item -ItemType Directory -Path (Split-Path -Parent $output) -Force | Out-Null
[System.IO.File]::WriteAllText($output, $patched, $enc)
Write-Host "Created: $output"
Write-Host 'Diagnostic only: use for brief offline tests; collision logs may repeat.'
