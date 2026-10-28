# Cossacks 3 Gameplay Overhaul: one-command build, deployment and log reporting.
# Windows PowerShell 5.1 compatible. Does not change the original reference/ files.
param(
    [string]$GameRoot = '',
    [ValidateSet('Compact','Full')][string]$Mode = 'Compact',
    [switch]$Update,
    [switch]$NoLaunch,
    [switch]$Editor,
    [switch]$CollectOnly
)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$configBase = if ($env:LOCALAPPDATA) { $env:LOCALAPPDATA } else { $env:TEMP }
$configDir = Join-Path $configBase 'Cossacks3Overhaul'
New-Item -ItemType Directory -Path $configDir -Force | Out-Null
$configPath = Join-Path $configDir 'game-root.txt'
if ([string]::IsNullOrWhiteSpace($GameRoot) -and (Test-Path -LiteralPath $configPath)) {
    $GameRoot = [System.IO.File]::ReadAllText($configPath, [System.Text.Encoding]::UTF8).Trim()
}
if ([string]::IsNullOrWhiteSpace($GameRoot)) {
    $candidates = @(
        (Join-Path ([Environment]::GetFolderPath('ProgramFilesX86')) 'Steam\steamapps\common\Cossacks 3'),
        (Join-Path ([Environment]::GetFolderPath('ProgramFiles')) 'Steam\steamapps\common\Cossacks 3')
    )
    foreach ($candidate in $candidates) {
        if (Test-Path -LiteralPath (Join-Path $candidate 'cossacks.exe')) {
            $GameRoot = $candidate; break
        }
    }
}
if ([string]::IsNullOrWhiteSpace($GameRoot)) {
    throw 'Game path unknown. First run must specify -GameRoot "D:\Steam\steamapps\common\Cossacks 3"'
}
$GameRoot = [System.IO.Path]::GetFullPath($GameRoot)
if (-not (Test-Path -LiteralPath (Join-Path $GameRoot 'cossacks.exe'))) {
    throw "cossacks.exe not found under: $GameRoot"
}
[System.IO.File]::WriteAllText($configPath, $GameRoot, [System.Text.Encoding]::UTF8)
$runId = (Get-Date).ToString('yyyyMMdd_HHmmss_fff')
$runDir = Join-Path $configDir ('runs\' + $runId)
New-Item -ItemType Directory -Path $runDir -Force | Out-Null

function Collect-OverhaulLog([datetime]$since, [bool]$freshOnly) {
    $locations = @(
        (Join-Path $GameRoot 'log'),
        (Join-Path $GameRoot 'logs'),
        (Join-Path $env:USERPROFILE 'Documents\cossacks\log'),
        (Join-Path $env:USERPROFILE 'Documents\cossacks\logs')
    )
    $logs = @()
    foreach ($dir in $locations) {
        if (Test-Path -LiteralPath $dir) {
            $logs += @(Get-ChildItem -LiteralPath $dir -Filter 'cos_*.log' -File -ErrorAction SilentlyContinue)
        }
    }
    if ($freshOnly) { $logs = @($logs | Where-Object { $_.LastWriteTime -ge $since }) }
    $latest = $logs | Sort-Object LastWriteTime -Descending | Select-Object -First 1
    if ($null -eq $latest) {
        Write-Warning 'No new cos_*.log found. Check cossacks.ini LogFileEnabled=true, LogFileRoot=true.'
        Write-Host 'Use -CollectOnly after the test if the log arrives later.'
        return
    }
    $destLog = Join-Path $runDir $latest.Name
    Copy-Item -LiteralPath $latest.FullName -Destination $destLog -Force
    $report = Join-Path $runDir 'overhaul_report.txt'
    $analyzer = Join-Path $PSScriptRoot 'analyze_overhaul_log.py'
    $python = Get-Command 'python.exe' -ErrorAction SilentlyContinue
    $pyLauncher = Get-Command 'py.exe' -ErrorAction SilentlyContinue
    if ($null -ne $python) {
        & $python.Source $analyzer $destLog -o $report
    } elseif ($null -ne $pyLauncher) {
        & $pyLauncher.Source -3 $analyzer $destLog -o $report
    } else {
        Write-Warning 'Python not found; log has been archived without the detailed report.'
    }
    $logText = [System.IO.File]::ReadAllText($destLog)
    $markers = @(
        'OVDIAG CAV_HIT_CHECK',
        'CavalryCharge HIT',
        'OVDIAG CAV_BLOCK reason=PIKES',
        'OVDIAG HP_APPLY',
        'OVDIAG RELOAD sid=cannon',
        'OVDIAG CAV_BOOST_MAX',
        'NavalRamming HIT'
    )
    $summary = @('Test: ' + $runId, 'Log: ' + $latest.FullName)
    foreach ($marker in $markers) {
        $count = [regex]::Matches($logText, [regex]::Escape($marker)).Count
        $summary += "$marker : $count"
    }
    $summaryPath = Join-Path $runDir 'summary.txt'
    [System.IO.File]::WriteAllLines($summaryPath, [string[]]$summary, [System.Text.Encoding]::UTF8)
    Write-Host "TEST LOG:   $destLog"
    Write-Host "SUMMARY:    $summaryPath"
    if (Test-Path -LiteralPath $report) { Write-Host "REPORT:     $report" }
}
if ($CollectOnly) {
    Collect-OverhaulLog ([datetime]::MinValue) $false
    exit 0
}
if ($Update) {
    $git = Get-Command 'git.exe' -ErrorAction SilentlyContinue
    if ($null -eq $git) { throw 'Git not found; omit -Update or install Git for Windows.' }
    $current = & $git.Source -C $root branch --show-current
    if ($LASTEXITCODE -ne 0) { throw 'Git branch read failed.' }
    if ($current.Trim() -ne 'feature/gameplay-overhaul-phase1') {
        throw "Expected feature/gameplay-overhaul-phase1 branch, found '$current'. Switch first."
    }
    & $git.Source -C $root pull --ff-only origin feature/gameplay-overhaul-phase1
    if ($LASTEXITCODE -ne 0) { throw 'Git pull failed; no files deployed.' }
}
Write-Host "Generating overhaul ($Mode)..."
& (Join-Path $PSScriptRoot 'build_gameplay_overhaul_diagnostics.ps1') -LogMode $Mode
if (-not $?) { throw 'Generator failed; nothing deployed.' }
$paths = @(
    'scripts\dmscript.global',
    'scripts\units\unit.inc\initial.inc',
    'scripts\units\unit.inc\nothing.inc',
    'scripts\units\global.inc\progresseconomicai.inc',
    'scripts\lib\unit.script',
    'scripts\lib\miscext2.script',
    'scripts\lib\country.script',
    'scripts\lib\player.script'
)
$sourceDir = Join-Path $root 'mod\data'
$targetDir = Join-Path $GameRoot 'mods\NavalRamming\data'
if (-not (Test-Path -LiteralPath $targetDir)) {
    throw "Mod directory missing: $targetDir. Install/enable NavalRamming first."
}
foreach ($rel in $paths) {
    $src = Join-Path $sourceDir $rel
    if (-not (Test-Path -LiteralPath $src)) { throw "Generated file missing: $src" }
    if ((Get-Item -LiteralPath $src).Length -lt 200) { throw "Generated file suspiciously short: $src" }
}
$latin = [System.Text.Encoding]::GetEncoding(28591)
$initial = [System.IO.File]::ReadAllText((Join-Path $sourceDir 'scripts\units\unit.inc\initial.inc'), $latin)
$unit = [System.IO.File]::ReadAllText((Join-Path $sourceDir 'scripts\lib\unit.script'), $latin)
$damage = [System.IO.File]::ReadAllText((Join-Path $sourceDir 'scripts\lib\miscext2.script'), $latin)
if (-not $initial.Contains('SetGameObjectMyTrackPointMoveStep(0.006)')) { throw 'PRECHECK gun movement failed' }
if (-not $unit.Contains('objbase.weapon[1].pause := objbase.weapon[1].pause*1.5')) { throw 'PRECHECK secondary reload failed' }
if (-not $initial.Contains('SetGameObjectMyTrackPointUseAnimationCycles(false)')) { throw 'PRECHECK cavalry movement mode failed' }
if (-not $initial.Contains('SetGameObjectMyTrackPointMoveStep(0.045*arg_obj.cavalrySpeedFraction)')) { throw 'PRECHECK cavalry base speed failed' }
$motion = [System.IO.File]::ReadAllText((Join-Path $sourceDir 'scripts\units\unit.inc\nothing.inc'), $latin)
if (-not $motion.Contains('cavStartTime : Float = 1;')) { throw 'PRECHECK cavalry 1-second acceleration failed' }
if (-not $motion.Contains('0.045*arg_obj.cavalrySpeedFraction*arg_obj.cavalryChargeBoost')) { throw 'PRECHECK mounted charge step failed' }
if (-not $damage.Contains('pOverhaulNoFormation')) { throw 'PRECHECK formation damage failed' }
if (-not $damage.Contains('OVDIAG HP_APPLY')) { throw 'PRECHECK HP logger failed' }
Write-Host 'All eight files and key invariants checked (not a game-engine compile).'

$backupDir = Join-Path $runDir 'backup'
$original = @{}
foreach ($rel in $paths) {
    $dest = Join-Path $targetDir $rel
    $exists = Test-Path -LiteralPath $dest
    $original[$rel] = $exists
    if ($exists) {
        $backupFile = Join-Path $backupDir $rel
        New-Item -ItemType Directory -Path (Split-Path -Parent $backupFile) -Force | Out-Null
        Copy-Item -LiteralPath $dest -Destination $backupFile -Force
    }
}
try {
    foreach ($rel in $paths) {
        $dest = Join-Path $targetDir $rel
        New-Item -ItemType Directory -Path (Split-Path -Parent $dest) -Force | Out-Null
        Copy-Item -LiteralPath (Join-Path $sourceDir $rel) -Destination $dest -Force
        $sourceHash = (Get-FileHash -LiteralPath (Join-Path $sourceDir $rel) -Algorithm SHA256).Hash
        $destHash = (Get-FileHash -LiteralPath $dest -Algorithm SHA256).Hash
        if ($sourceHash -ne $destHash) { throw "SHA256 mismatch: $rel" }
    }
} catch {
    $failure = $_
    Write-Warning 'Deployment failed, rolling back.'
    foreach ($rel in $paths) {
        $dest = Join-Path $targetDir $rel
        if ($original[$rel]) {
            Copy-Item -LiteralPath (Join-Path $backupDir $rel) -Destination $dest -Force
        } else {
            Remove-Item -LiteralPath $dest -Force -ErrorAction SilentlyContinue
        }
    }
    throw $failure
}
Write-Host "DEPLOYED 8 overrides. Backup: $backupDir"
if ($NoLaunch) {
    Write-Host 'Game launch skipped (-NoLaunch).'
    exit 0
}
$sessionStart = Get-Date
if ($Editor) {
    $editorExe = Join-Path $GameRoot 'editor.exe'
    if (-not (Test-Path -LiteralPath $editorExe)) { throw "Editor missing: $editorExe" }
    Start-Process -FilePath $editorExe -WorkingDirectory $GameRoot | Out-Null
    $processName = 'editor'
    Write-Host 'Editor launched: Ctrl+T can switch editing/testing modes.'
} else {
    Start-Process -FilePath 'explorer.exe' -ArgumentList 'steam://rungameid/333420' | Out-Null
    $processName = 'cossacks'
    Write-Host 'Steam game launched.'
}
Write-Host "Leave this terminal open during test. Results: $runDir"
$found = $false
for ($i = 0; $i -lt 180; $i++) {
    if ($null -ne (Get-Process -Name $processName -ErrorAction SilentlyContinue)) {
        $found = $true
        break
    }
    Start-Sleep -Seconds 1
}
if (-not $found) {
    Write-Warning "Process '$processName' not seen within 180s. Use -CollectOnly after testing."
    exit 0
}
while ($null -ne (Get-Process -Name $processName -ErrorAction SilentlyContinue)) {
    Start-Sleep -Seconds 3
}
Start-Sleep -Seconds 2
Collect-OverhaulLog $sessionStart $true
