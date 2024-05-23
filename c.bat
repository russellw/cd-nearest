@echo off
for /f "delims=" %%i in ('nearest.exe %*') do set "targetDir=%%i"
if errorlevel 1 exit /b
cd /d "%targetDir%"
