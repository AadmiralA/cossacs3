# NavalRamming diagnostic v2: verify script loading AND collision delivery.
# Runs in Windows PowerShell 5.1+. Never edits reference/ files.
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$enc = [System.Text.Encoding]::GetEncoding(28591) # byte-preserving Latin-1 for ASCII-only patches

function Get-Source([string]$relative) {
    $full = Join-Path $root $relative
    if (-not (Test-Path -LiteralPath $full)) { throw "Missing reference file: $full" }
    return [System.IO.File]::ReadAllText($full, $enc)
}
function Get-Newline([string]$text) {
    $crlf = [string][char]13 + [string][char]10
    $lf = [string][char]10
    if ($text.Contains($crlf)) { return $crlf }
    return $lf
}
function Insert-Once([string]$text, [string]$needle, [string]$insert, [string]$label) {
    $found = [regex]::Matches($text, [regex]::Escape($needle)).Count
    if ($found -ne 1) { throw "$label : expected exactly one anchor; got $found. No files written." }
    return $text.Replace($needle, $needle + $insert)
}
function Normalize-Newlines([string]$text, [string]$nl) {
    return (($text -split '\r?\n') -join $nl)
}

$collideOriginal = Get-Source 'reference\scripts\lib\miscext.script'
$initialOriginal = Get-Source 'reference\scripts\units\unit.inc\initial.inc'
$collideNl = Get-Newline $collideOriginal
$initialNl = Get-Newline $initialOriginal

# 1. Log arrival of all ship collision callbacks, even if enemy checks fail.
$callbackStart = 'procedure _misc_Collided(const hnd: Integer);' + $collideNl + 'begin'
$callbackNote = @'
   if (_unit_IsWaterUnit(hnd)) then
   Log('NavalRamming CALLBACK ship='+GetGameObjectBaseNameByHandle(hnd));
'@
$collidePatched = Insert-Once $collideOriginal $callbackStart ($collideNl + (Normalize-Newlines $callbackNote $collideNl)) 'collision-callback'

# 2. Log enemy ship collisions after original target validation.
$targetAnchor = '         var pobj : Pointer = _misc_GetObjectArgData(hnd, gc_argunit_obj);'
$targetNote = @'
         // NavalRamming PROBE: hostile water-unit collision, diagnostic only.
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
$collidePatched = Insert-Once $collidePatched $targetAnchor ($collideNl + (Normalize-Newlines $targetNote $collideNl)) 'collision-target'

# 3. Independently log the initializer of each new water unit.
$shipAnchor = '      [*] = ;   if (_unit_IsWaterUnit(myHnd)) then' + $initialNl + '      [*] = ;   begin'
$shipNote = "      [*] = ;      Log('NavalRamming INIT ship='+basename);"
$initialPatched = Insert-Once $initialOriginal $shipAnchor ($initialNl + $shipNote) 'ship-initializer'

# All checks have passed; write to mod/, NEVER to reference/.
$outCollide = Join-Path $root 'mod\data\scripts\lib\miscext.script'
$outInitial = Join-Path $root 'mod\data\scripts\units\unit.inc\initial.inc'
New-Item -ItemType Directory -Path (Split-Path -Parent $outCollide) -Force | Out-Null
New-Item -ItemType Directory -Path (Split-Path -Parent $outInitial) -Force | Out-Null
[System.IO.File]::WriteAllText($outCollide, $collidePatched, $enc)
[System.IO.File]::WriteAllText($outInitial, $initialPatched, $enc)

Write-Host 'NavalRamming diagnostic v2 generated:'
Write-Host $outCollide
Write-Host $outInitial
Write-Host 'Copy BOTH files into the enabled mod with the same relative data/ paths.'
Write-Host 'Search game logs for NavalRamming INIT, CALLBACK, and PROBE.'
Write-Host 'No damage is dealt. Use an offline match and test briefly.'
