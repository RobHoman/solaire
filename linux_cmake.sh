#!/bin/zsh
mkdir -p build/linux
cd build/linux
cmake ../.. -G Ninja -DCMAKE_TOOLCHAIN_FILE=../../toolchain_linux_clang.cmake
