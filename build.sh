#!/bin/bash

export SPLAT_DIR=/home/oatmealine/storage/splat
export TOOLCHAIN=/home/oatmealine/git/clang-msvc-sdk/clang-cl-msvc.cmake
# this is supposed to default to x86 but it doesnt for some reason
export HOST_ARCH=x86

cmake \
  -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
  -DGEODE_DISABLE_FMT_CONSTEVAL=1 \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -B build
  
cmake --build build --config RelWithDebInfo