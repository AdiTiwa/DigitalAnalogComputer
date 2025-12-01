# Hybrid Digital-Analog Simulator

This project demonstrates a hybrid co-simulation using **SystemVerilog** (via Verilator) for digital components and **Xyce** for analog components, orchestrated by **C++**.

## Prerequisites

- **Verilator**: For compiling SystemVerilog to C++.
- **Xyce**: For analog circuit simulation.
- **CMake**: Build system.
- **C++ Compiler**: GCC or Clang.

## Installation (macOS)

### 1. Install Homebrew Dependencies
```bash
brew install verilator cmake
```

### 2. Install Xyce
Xyce is not available directly via Homebrew. You must download the official binary installer from Sandia National Laboratories.

1.  Go to the [Xyce Downloads Page](https://xyce.sandia.gov/downloads/).
2.  Download the **macOS** package (usually a `.zip` containing a `.pkg`).
3.  Run the installer.
4.  **Important**: You may need to add Xyce to your PATH. The installer typically places it in `/usr/local/Xyce-Release-<version>/bin`.
    Add this to your `~/.zshrc`:
    ```bash
    export PATH=$PATH:/usr/local/Xyce-Release-7.8-NORAD/bin
    ```
    (Check the actual path after installation).

## Directory Structure

- `digital/`: SystemVerilog source files (`counter.sv`).
- `analog/`: Xyce netlist templates (`template.cir`).
- `src/`: C++ source files (`main.cpp`, `xyce_wrapper.cpp`, `xyce_wrapper.h`).
- `CMakeLists.txt`: Build configuration.

## How it Works

1.  **Digital Simulation**: The `counter.sv` module is compiled to a C++ class by Verilator. It outputs an 8-bit count.
2.  **Analog Simulation**: The `template.cir` represents an RC circuit. The voltage source is updated dynamically.
3.  **Orchestration**:
    -   The C++ `main` loop clocks the digital model.
    -   The digital output is converted to an analog voltage (DAC).
    -   The `XyceWrapper` generates a temporary netlist with the new voltage and the previous capacitor voltage (Initial Condition).
    -   Xyce is invoked to run for one time step.
    -   The output voltage is parsed and fed back (conceptually) or logged.

## Build and Run

I have provided a convenience script `run.sh` that handles compilation and execution, setting the necessary paths for Xyce.

```bash
chmod +x run.sh
./run.sh
```

## Expected Output

The simulation will print the cycle count, digital output, DAC voltage, and the capacitor voltage.

```
Starting Hybrid Simulation...
Cycle   Digital(Count)  DAC(V)  Analog(CapV)
...
98      98      1.92157 1.73514
99      99      1.94118 1.75475
Simulation Finished.
```
