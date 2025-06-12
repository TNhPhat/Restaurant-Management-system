@echo off
echo ==== Creating build directory ====
if exist build (
    rmdir /s /q build
)
mkdir build
cd build

echo ==== Configuring with CMake and Ninja ====
cmake -G "Ninja" ..

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    exit /b %errorlevel%
)

echo ==== Building the project ====
cmake --build .

if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    exit /b %errorlevel%
)

echo ==== Running the executable ====
cd ..
RESTAURANT.exe