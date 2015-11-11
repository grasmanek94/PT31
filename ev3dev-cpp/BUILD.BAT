@echo off
cd /D "%~dp0"
set PATH=%CYGWIN64%\bin;%PATH%
make rsp_r rsp_tr ev3_r ev3_tr