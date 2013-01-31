@echo off
for %%X in (premake4.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND (
echo 'premake4' not found in your path.
exit /b
)

premake4 --file=premake.lua vs2010
