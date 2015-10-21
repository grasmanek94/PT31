@echo off
cd /D "%~dp0"
set PATH=%CYGWIN64%\bin;%PATH%
make clean
make all