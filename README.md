# cmake_stm32_template

# TODO
-[x] move third_party/STM32CubeL4 to use https://github.com/STMicroelectronics/STM32CubeL4/blob/master/README.md as submodule
-[ ] add freertos
-[ ] add googletest
-[ ] mock hardware functions

# Usage
## 1. Clone the repository
    git clone https://github.com/rdmeneze/cmake_stm32_template.git
## 2. Initialize submodules
    git submodule update --init --recursive
## 3. Build the project
    mkdir build && cd build
    cmake .. -GNinja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake
    ninja
