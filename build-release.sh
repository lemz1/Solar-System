#!/bin/bash

vendor/premake/linux/premake5 gmake config=release

make config=release

cd export/linux/Debug/bin 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/media/lemz/Projects/Projects/C++/Development/Solar-System/vendor/webgpu/bin/linux-x86_64
#./Solar-System
