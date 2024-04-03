@echo off

cmake -B build/windows/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/windows/release

PAUSE
