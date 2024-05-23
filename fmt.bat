@echo off
clang-format -i --style=file nearest.cpp||exit /b
git diff
