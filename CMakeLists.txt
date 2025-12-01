cmake_minimum_required(VERSION 3.10)
project(hybrid_sim)

# Find Verilator
find_package(Verilator REQUIRED)

# Define source files
set(DIGITAL_SRC ${CMAKE_SOURCE_DIR}/digital/counter.sv)
message(STATUS "Digital Source: ${DIGITAL_SRC}")
set(CPP_SRC 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/xyce_wrapper.cpp
)

# Run Verilator to generate C++ model
# This creates a target 'Vcounter'
verilate(
    SOURCES digital/counter.sv
    TOP_MODULE counter
    PREFIX Vcounter
)

# Create the executable
add_executable(hybrid_sim ${CPP_SRC})

# Link against the Verilated model
target_link_libraries(hybrid_sim Vcounter::Vcounter)

# Include directories
target_include_directories(hybrid_sim PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Ensure C++14 or later
set(CMAKE_CXX_STANDARD 14)
