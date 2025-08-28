@echo off
setlocal

:: Clean previous build directory if it exists
@REM if exist build (
@REM     echo Cleaning previous build directory...
@REM     rd /s /q build
@REM )

@REM :: Create build directory and enter it
@REM mkdir build

@REM echo Configuring with CMake (MinGW Makefiles)...
@REM cmake .. -G "Ninja"
@REM if errorlevel 1 goto :error

echo Building project...
cd build
ninja
if errorlevel 1 goto :error

cd ..

echo.
echo Running executable...
if exist build\RESTAURANT.exe (
    build\RESTAURANT.exe
) else if exist RESTAURANT.exe (
    RESTAURANT.exe
) else (
    echo [ERROR] Executable RESTAURANT.exe not found!
    pause
    exit /b 1
)

goto :eof

:error
echo [ERROR] Build or configuration failed!
pause
exit /b 1