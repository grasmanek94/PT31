@echo off
cd /D "%~dp0"
set PATH=%CYGWIN64%\bin;%PATH%
start "" "%CYGWIN64%\bin\mintty.exe"