# cmake_stm32_template

[![CI/CD](https://github.com/rdmeneze/cmake_stm32_template/actions/workflows/ci.yml/badge.svg)](https://github.com/rdmeneze/cmake_stm32_template/actions/workflows/ci.yml)
[![Firmware Build](https://img.shields.io/badge/firmware-STM32L432KC-blue)]()
[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-v10.3.1-green)]()
[![Google Test](https://img.shields.io/badge/tests-GoogleTest-orange)]()
[![ARM GCC](https://img.shields.io/badge/toolchain-ARM%20GCC%2014.2-red)]()

# TODO
-[x] move third_party/STM32CubeL4 to use https://github.com/STMicroelectronics/STM32CubeL4/blob/master/README.md as submodule
-[x] add freertos
-[x] add googletest
-[x] mock hardware functions

# Usage
## 1. Clone the repository
    git clone https://github.com/rdmeneze/cmake_stm32_template.git
## 2. Initialize submodules
    git submodule update --init --recursive
## 3. Build the project
    mkdir build && cd build
    cmake .. -GNinja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake
    ninja

## 4. Quick Start
```bash
# Build firmware
cmake --build build --config Release

# Run tests
./run_tests.sh

# Flash firmware
./flash_firmware.sh

# Debug
./debug_firmware.sh
```
