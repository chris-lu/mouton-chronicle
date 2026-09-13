@echo off
rem ---------------------------------------------------------------------------
rem Builds (unless --no-build), prepares build\watcom\run\ and starts a program
rem in DOSBox-X. The data folder is copied next to the EXE under the name
rem Mouton\, which is the path hard-coded in the game.
rem
rem   scripts\run.cmd                     the game (MOUTON.EXE)
rem   scripts\run.cmd credits             a tool: credits, greeting, credits2, rotpal, lignecnt
rem   scripts\run.cmd --no-build          skip the build step (can be combined: --no-build credits)
rem
rem Settings come from .env (DOSBOX, CDIMAGE, DOSBOX_ARGS), see .env.example.
rem ---------------------------------------------------------------------------
setlocal
set "HERE=%~dp0"
set "PROG=mouton"
set "DOBUILD=1"
:args
if "%~1"=="" goto :go
if /i "%~1"=="--no-build" (set "DOBUILD=0") else (set "PROG=%~1")
shift
goto :args

:go
call "%HERE%env.cmd" || exit /b 1
if not exist "%DOSBOX%" (
  echo [run] DOSBox-X not found at "%DOSBOX%" - set DOSBOX in .env
  exit /b 1
)
if "%DOBUILD%"=="1" call "%HERE%build.cmd" || exit /b 1

set "OUT=%MC_ROOT%\build\watcom\out"
set "RUN=%MC_ROOT%\build\watcom\run"
if not exist "%OUT%\%PROG%.exe" (
  echo [run] "%OUT%\%PROG%.exe" not found. Programs: mouton credits greeting credits2 rotpal lignecnt
  exit /b 1
)
if not exist "%RUN%" mkdir "%RUN%"
copy /y "%OUT%\%PROG%.exe" "%RUN%\" >nul
xcopy /e /i /y /q "%MC_ROOT%\data" "%RUN%\Mouton" >nul

set "CDARGS="
if defined CDIMAGE if exist "%CDIMAGE%" set CDARGS=-c "imgmount d \"%CDIMAGE%\" -t iso -fs iso"

echo [run] %PROG%.exe in "%RUN%"
"%DOSBOX%" -nomenu -fastlaunch -conf "%MC_ROOT%\build\watcom\dosbox.conf" -c "mount c \"%RUN%\"" %CDARGS% -c "c:" -c "%PROG%.exe" %DOSBOX_ARGS%
