@echo off
cd /D "%~dp0"
set PATH=%CYGWIN64%\usr\bin;%CYGWIN64%\bin;%CYGWIN64%\usr\sbin;%CYGWIN64%\usr\local\bin;
start "" "%CYGWIN64%\bin\mintty.exe"