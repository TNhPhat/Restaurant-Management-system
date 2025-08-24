@echo off
setlocal

:: Clean previous build directory if it exists
if exist build (
    echo Cleaning previous build directory...
    rd /s /q build
)

:: Create build directory and enter it
mkdir build
cd build

echo Configuring with CMake (MinGW Makefiles)...
cmake .. -G "Ninja"
if errorlevel 1 goto :error

echo Building project...
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