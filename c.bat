@echo off
:: Run calc-dir.exe and capture its output
for /f "delims=" %%i in ('nearest.exe %*') do set "targetDir=%%i"

:: Output the captured directory for debugging
rem echo Calculated directory: %targetDir%

:: Change to the captured directory
cd /d "%targetDir%"
