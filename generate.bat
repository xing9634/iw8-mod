@echo off
.\tools\premake\premake5.exe vs2019
if %ERRORLEVEL% neq	0 (
	pause
)
