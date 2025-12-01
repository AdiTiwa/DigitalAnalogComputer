#!/bin/bash
set -e

# Configuration
DIGITAL_SRC="digital/counter.sv"
CPP_SRC="src/main.cpp src/xyce_wrapper.cpp"
BUILD_DIR="build_verilator"
XYCE_PATH="/usr/local/XyceNF_7.10/bin"

# Add Xyce to PATH
export PATH=$PATH:$XYCE_PATH

# Clean build directory
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

echo "Compiling with Verilator..."
# Run Verilator
# --cc: Create C++ output
# --exe: Link with C++ files
# --build: Automatically run make
# -CFLAGS: Add include path for src
verilator --cc $DIGITAL_SRC --top-module counter --prefix Vcounter \
    --Mdir $BUILD_DIR \
    --exe $CPP_SRC \
    -CFLAGS "-I../src -std=c++14" \
    --build

echo "Build complete."

echo "Running Simulation..."
./$BUILD_DIR/Vcounter

echo "Done."
