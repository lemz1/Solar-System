@echo off

cmake -G -B build/windows/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/windows/release
