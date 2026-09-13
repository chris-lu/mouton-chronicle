@echo off
rem Starts the game from the run\ folder (created by "wmake run") in DOSBox-X.
rem Set DOSBOX to the path of dosbox-x.exe if it is not in the default location.
cd /d "%~dp0"
if "%DOSBOX%"=="" set DOSBOX=C:\DOSBox-X\dosbox-x.exe
if not exist "%DOSBOX%" (
  echo DOSBox-X not found at %DOSBOX% - install it or set DOSBOX
  exit /b 1
)
if not exist run\MOUTON.EXE (
  echo run\MOUTON.EXE missing - build with "wmake run" first
  exit /b 1
)
"%DOSBOX%" -conf dosbox.conf -c "mount c \"%~dp0run\"" -c "c:" -c "MOUTON.EXE"
