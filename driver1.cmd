@echo off
rem Driver 1 PC port on Windows: "driver1 setup <disc>", "driver1", "driver1 --window", or drop a .cue onto this file.
rem driver1.ps1 does the work (see there).
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0driver1.ps1" %*
set rc=%errorlevel%
rem Started from Explorer (a double-click or a dropped disc image; the window closes at the end): keep it open
rem after an error or a setup, to show what happened.
echo %cmdcmdline% | find /i "/c" > nul || exit /b %rc%
if not "%rc%"=="0" pause & exit /b %rc%
if exist "%~1" pause
exit /b %rc%
