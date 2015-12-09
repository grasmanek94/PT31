@echo off
cd /D "%~dp0"
set PATH=%PROFTAAK_T31_CROSS_COMPILE_BIN%;%PATH%
rem make rsp_r rsp_tr
rem make rsp_d rsp_td
rem make ev3_r ev3_tr
make ev3_d ev3_td
rem make r tr
rem make d td