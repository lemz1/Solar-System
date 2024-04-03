#!/bin/bash

cmake -B build/linux/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/linux/debug
