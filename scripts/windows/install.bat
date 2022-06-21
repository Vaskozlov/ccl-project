@echo off

set tempPath=".\tmp"

if not exist "%tempPath%\tmp"      mkdir "%tempPath%\tmp"

cd "%tempPath%"
set CMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
set cmake_dir=C:\\\"Program Files\"\CMake\bin

powershell -Command "C:\vcpkg\vcpkg install fmt:x64-windows"

cd ..\..\..

mkdir build

echo "SUCCESS! Installation is completed"

echo %CMAKE_TOOLCHAIN_FILE%

