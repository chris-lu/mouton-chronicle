@echo off
rem ---------------------------------------------------------------------------
rem Mouton Chronicles - environment for the Open Watcom build and DOSBox-X.
rem
rem Reads KEY=VALUE lines from .env at the repository root (see .env.example),
rem applies defaults, and puts the Open Watcom tools on the PATH.
rem
rem   call scripts\env.cmd      from cmd, PowerShell (& cmd /k) or another script
rem
rem Variables: WATCOM, DOSBOX, CDIMAGE, DOSBOX_ARGS  (plus MC_ROOT = repo root)
rem ---------------------------------------------------------------------------
if defined MC_ENV_LOADED exit /b 0
for %%I in ("%~dp0..") do set "MC_ROOT=%%~fI"

if exist "%MC_ROOT%\.env" (
  for /f "usebackq eol=# tokens=1,* delims==" %%A in ("%MC_ROOT%\.env") do (
    if not "%%~A"=="" set "%%~A=%%~B"
  )
)
if not defined WATCOM set "WATCOM=C:\WATCOM"
if not defined DOSBOX set "DOSBOX=C:\DOSBox-X\dosbox-x.exe"

set "PATH=%WATCOM%\binnt64;%WATCOM%\binnt;%PATH%"
set "INCLUDE=%WATCOM%\h"
set "EDPATH=%WATCOM%\eddat"
set "MC_ENV_LOADED=1"

if not exist "%WATCOM%\binnt64\wcc.exe" if not exist "%WATCOM%\binnt\wcc.exe" (
  echo [env] Open Watcom not found under "%WATCOM%" - set WATCOM in .env
  exit /b 1
)
exit /b 0
