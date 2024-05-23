@echo off
for /f "delims=" %%i in ('nearest.exe %*') do set "targetDir=%%i"
rem echo %targetDir%
cd /d "%targetDir%"
