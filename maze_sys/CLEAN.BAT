@echo off
cd /D "%~dp0"
set PATH=%PROFTAAK_T31_CROSS_COMPILE_BIN%;%CYGWIN64%\bin;%PATH%
make clean