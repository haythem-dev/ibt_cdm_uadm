@echo off
setlocal enabledelayedexpansion

REM Iterate over all config_*.ini files in the current directory
for %%F in (config_*.ini) do (
    echo Testing config file: %%F
    echo Press any key to continue...
    pause >nul
    ..\..\x64\Release\esqltest.exe %%F
    echo.
)

echo All tests completed.
echo Press any key to continue...
pause >nul