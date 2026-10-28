@echo off
setlocal
cd /d "%~dp0.."
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0overhaul_oneclick.ps1" -Update
if errorlevel 1 echo [ERROR] Test automation failed. Read the error above.
echo.
pause
