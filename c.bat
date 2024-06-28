@echo off
set targetDir=
for /f "delims=" %%i in ('nearest.exe %*') do set "targetDir=%%i"
cd /d "%targetDir%"
