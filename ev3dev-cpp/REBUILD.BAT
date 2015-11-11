@echo off
cd /D "%~dp0"
set PATH=%PROFTAAK_T31_CROSS_COMPILE_BIN%;%PATH%
make clean
make rsp_r rsp_tr
make ev3_r ev3_tr
make t tr