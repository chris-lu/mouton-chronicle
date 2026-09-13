@echo off
rem Environment for building Mouton Chronicles with Open Watcom 2.0 on Windows.
rem Adjust WATCOM if the toolchain is installed elsewhere, then run:  wmake
if "%WATCOM%"=="" set WATCOM=C:\WATCOM
set PATH=%WATCOM%\binnt64;%WATCOM%\binnt;%PATH%
set INCLUDE=%WATCOM%\h
set EDPATH=%WATCOM%\eddat
set WIPFC=%WATCOM%\wipfc
echo Open Watcom environment set (WATCOM=%WATCOM%)
