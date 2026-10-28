# Generate an offline-only collision logging probe without changing reference/.
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$source = Join-Path $root 'reference\scripts\lib\miscext.script'
$output = Join-Path $root 'mod\data\scripts\lib\miscext.script'
if (-not (Test-Path -LiteralPath $source)) { throw "Missing reference: $source" }

# Latin-1 maps all byte values 1:1. All inserted code is ASCII.
# This preserves the source's original bytes regardless of its game text encoding.
$enc = [System.Text.Encoding]::GetEncoding(28591)
$original = [System.IO.File]::ReadAllText($source, $enc)
$anchor = '         var pobj : Pointer = _misc_GetObjectArgData(hnd, gc_argunit_obj);'
$count = [regex]::Matches($original, [regex]::Escape($anchor)).Count

if ($count -ne 1) {
    throw "Expected one insertion anchor in reference/scripts/lib/miscext.script, found $count. Source was not modified."
}
if (-not $original.Contains('procedure _misc_Collided(const hnd: Integer);')) {
    throw 'Expected _misc_Collided handler not found; source was not modified.'
}
if ($original.Contains('// NavalRamming DIAGNOSTIC')) {
    throw 'Reference already contains the diagnostic probe; refusing to insert twice.'
}

$probe = @'
         // NavalRamming DIAGNOSTIC: no game damage, observation only.
         if (pobj<>nil) then
         if (not TObj(pobj).bdead) and (not TObj(pobj2).bdead) then
         begin
            var ourProp : Pointer = gObjProp[TObj(pobj).cid][TObj(pobj).id];
            var theirProp : Pointer = gObjProp[TObj(pobj2).cid][TObj(pobj2).id];
            if (TObjProp(ourProp).media=gc_obj_media_water) and (TObjProp(theirProp).media=gc_obj_media_water) then
            if ((gPlayer[TObj(pobj).pl].enemyplmask and gPlayer[TObj(pobj2).pl].myplmask)<>0) then
            Log('NavalRamming PROBE ship='+TObjProp(ourProp).sid+' target='+TObjProp(theirProp).sid);
         end;
'@

$nl = if ($original.Contains("\`r\`n")) { "\`r\`n" } else { "\`n" }
$normalizedProbe = ($probe -split '\r?\n') -join $nl
$patched = $original.Replace($anchor, $anchor + $nl + $normalizedProbe)

New-Item -ItemType Directory -Path (Split-Path -Parent $output) -Force | Out-Null
[System.IO.File]::WriteAllText($output, $patched, $enc)
Write-Host "Created diagnostic mod file: $output"
Write-Host 'Offline diagnostic only: no damage. Collision log entries can be frequent.'
