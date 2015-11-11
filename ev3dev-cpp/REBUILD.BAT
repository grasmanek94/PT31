@echo off
cd /D "%~dp0"
set PATH=%PROFTAAK_T31_CROSS_COMPILE_BIN%;%PATH%
make clean
make rsp_r rsp_tr rsp_d rsp_td
make ev3_r ev3_tr ev3_d ev3_td
make t tr d td