@echo off
rem ---------------------------------------------------------------------------
rem Builds the game and the tools with Open Watcom (runs wmake in build\watcom).
rem
rem   scripts\build.cmd                  build everything into build\watcom\out\
rem   scripts\build.cmd clean            delete out\ and run\
rem   scripts\build.cmd out\credits.exe  build one target
rem ---------------------------------------------------------------------------
setlocal
call "%~dp0env.cmd" || exit /b 1
pushd "%MC_ROOT%\build\watcom" || exit /b 1
wmake -h %*
set "RC=%ERRORLEVEL%"
popd
exit /b %RC%
