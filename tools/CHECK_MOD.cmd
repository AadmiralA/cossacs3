@echo off
setlocal
cd /d "%~dp0.."
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0overhaul_oneclick.ps1" -Update -NoLaunch
if errorlevel 1 echo [ERROR] Offline build failed. Read the error above.
echo.
pause
