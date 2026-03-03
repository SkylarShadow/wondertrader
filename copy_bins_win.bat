@echo off
set despath=%1
if "%despath%"=="" set despath="../VvtPy"
python ./scripts/copy_bins_win.py %despath%
