#!/usr/bin/env bash

# Usage: outputs valid .clangd configuration for the project on Unix-like operating systems 

cflags=$(pkg-config --cflags Qt6Widgets)

# Build .clangd file
echo "CompileFlags:
  Add: [-std=c++17,"
echo $cflags | sed 's/ /, /g' 

if [[ $(uname) == "Darwin" ]]; then
    echo ", -xobjective-c++"
fi

echo "]"
