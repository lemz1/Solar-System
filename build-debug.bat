@echo off

cmake -B build/windows/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/windows/debug
