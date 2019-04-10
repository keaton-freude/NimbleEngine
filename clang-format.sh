#!/usr/bin/env bash

# Runs clang-format across all files. Hook this up to a build system, or run it manually
# to fix annoying formatting issues!

# .cpp files
echo "Running clang format!"
# Only consider the frontend/ folder and src/ folder
clang-format -i -style=file src/**/*.cpp
clang-format -i -style=file frontend/**/*.cpp

# .h files

clang-format -i -style=file include/**/*.h
