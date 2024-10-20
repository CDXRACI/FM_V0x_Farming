@echo off
REM Get the date and time parts
for /f "tokens=1-4 delims=/.- " %%a in ("%date%") do (
    set year=%%c
    set month=%%a
    set day=%%b
)
for /f "tokens=1-3 delims=:., " %%a in ("%time%") do (
    set hour=%%a
    set minute=%%b
    set second=%%c
)

REM Create a formatted timestamp (YYYYMMDD_HHMMSS)
set timestamp=%year%%month%%day%_%hour%%minute%%second%

echo The formatted timestamp is: %timestamp%

pause
