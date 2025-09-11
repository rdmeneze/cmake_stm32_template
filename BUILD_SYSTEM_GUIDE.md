# STM32 Portable Firmware - Build System Guide

## Overview

The build system is designed to be flexible, allowing you to choose the processor target at CMake call time. This provides maximum flexibility for different development environments.

## Build Methods

### 1. **Direct CMake (Recommended)**

This is the most flexible and transparent way to use the system:

```bash
# Configure for STM32L432KC (Debug)
cmake -B build-l432kc -DTARGET_BOARD=stm32l432kc -DCMAKE_BUILD_TYPE=Debug

# Configure for STM32F407VG (Release)
cmake -B build-f407vg -DTARGET_BOARD=stm32f407vg -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build-l432kc
cmake --build build-f407vg --config Release

# Build with parallelization
cmake --build build-l432kc -j$(nproc)
```

### 2. **Convenience Script**

For quick development use:

```bash
# Use defaults (stm32l432kc, Debug)
./build_portable.sh

# Specify target
./build_portable.sh stm32f407vg

# Specify target and build type
./build_portable.sh stm32f407vg Release

# Specific actions
./build_portable.sh stm32l432kc Debug clean
./build_portable.sh stm32f407vg Debug flash
```

### 3. **Specific Toolchain**

To use a specific toolchain:

```bash
cmake -B build-custom \
      -DTARGET_BOARD=stm32l432kc \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake
```

## CMake Parameters

### Main Parameters

| Parameter | Description | Values | Default |
|-----------|-------------|---------|---------|
| `TARGET_BOARD` | Target board | `stm32l432kc`, `stm32f407vg` | `stm32l432kc` |
| `CMAKE_BUILD_TYPE` | Build type | `Debug`, `Release` | `Debug` |
| `CMAKE_TOOLCHAIN_FILE` | Toolchain file | Path to `.cmake` | Auto-detected |

### Advanced Parameters

```bash
# Verbose build for debugging
cmake -B build-debug -DTARGET_BOARD=stm32l432kc -DCMAKE_VERBOSE_MAKEFILE=ON

# Specify specific compiler
cmake -B build-custom \
      -DTARGET_BOARD=stm32f407vg \
      -DCMAKE_C_COMPILER=arm-none-eabi-gcc-10 \
      -DCMAKE_CXX_COMPILER=arm-none-eabi-g++-10

# Build with custom configurations
cmake -B build-custom \
      -DTARGET_BOARD=stm32l432kc \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DENABLE_TESTING=ON
```

## Build Directory Structure

The system creates organized build directories:

```
project/
├── build-stm32l432kc-debug/     # Debug for L432KC
├── build-stm32l432kc-release/   # Release for L432KC
├── build-stm32f407vg-debug/     # Debug for F407VG
├── build-stm32f407vg-release/   # Release for F407VG
└── build-custom/                # Custom builds
```

## Common Workflows

### Iterative Development

```bash
# Initial setup
cmake -B build-dev -DTARGET_BOARD=stm32l432kc -DCMAKE_BUILD_TYPE=Debug

# Incremental build (fast)
cmake --build build-dev

# Complete rebuild when needed
cmake --build build-dev --clean-first
```

### Multi-Target Development

```bash
# Configure all targets
cmake -B build-l432kc-debug -DTARGET_BOARD=stm32l432kc -DCMAKE_BUILD_TYPE=Debug
cmake -B build-l432kc-release -DTARGET_BOARD=stm32l432kc -DCMAKE_BUILD_TYPE=Release
cmake -B build-f407vg-debug -DTARGET_BOARD=stm32f407vg -DCMAKE_BUILD_TYPE=Debug
cmake -B build-f407vg-release -DTARGET_BOARD=stm32f407vg -DCMAKE_BUILD_TYPE=Release

# Build all
cmake --build build-l432kc-debug &
cmake --build build-l432kc-release &
cmake --build build-f407vg-debug &
cmake --build build-f407vg-release &
wait
```

### CI/CD Pipeline

```bash
#!/bin/bash
# ci_build.sh

TARGETS=("stm32l432kc" "stm32f407vg")
BUILD_TYPES=("Debug" "Release")

for target in "${TARGETS[@]}"; do
    for build_type in "${BUILD_TYPES[@]}"; do
        echo "Building ${target} ${build_type}..."

        BUILD_DIR="build-${target}-${build_type,,}"

        cmake -B "$BUILD_DIR" \
              -DTARGET_BOARD="$target" \
              -DCMAKE_BUILD_TYPE="$build_type" \
              -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake

        cmake --build "$BUILD_DIR" -j$(nproc)

        # Check if binaries were generated
        if [[ ! -f "$BUILD_DIR/${target}_firmware.bin" ]]; then
            echo "ERROR: Binary not generated for $target $build_type"
            exit 1
        fi
    done
done
```

## Debug and Troubleshooting

### Verbose Build

```bash
# See complete executed commands
cmake -B build-debug -DTARGET_BOARD=stm32l432kc -DCMAKE_VERBOSE_MAKEFILE=ON
cmake --build build-debug --verbose
```

### Configuration Information

```bash
# See CMake variables
cmake -B build-debug -DTARGET_BOARD=stm32l432kc -LA

# See CMake cache
cmake -B build-debug -DTARGET_BOARD=stm32l432kc -LH
```

### Clean Build

```bash
# Clean specific build
rm -rf build-l432kc-debug

# Clean all builds
rm -rf build-*

# Clean rebuild
cmake --build build-l432kc-debug --clean-first
```

## Adding New Targets

### 1. Create Target Configuration

```cmake
# cmake/targets/my_board.cmake
set(MCU_FAMILY "STM32F4xx")
set(MCU_DEVICE "STM32F407xx")
set(MCU_DEVICE_LOWER "stm32f407xx")
set(MCU_FAMILY_LOWER "stm32f4xx")

set(MCU_DEFINES
    -DSTM32F407xx
    -DUSE_HAL_DRIVER
)

# ... rest of configuration
```

### 2. Use the New Target

```bash
# Immediately available!
cmake -B build-my-board -DTARGET_BOARD=my_board
cmake --build build-my-board
```

### 3. Update Script (Optional)

```bash
# Add to targets array in build_portable.sh
TARGETS=(
    "stm32l432kc"
    "stm32f407vg"
    "my_board"        # <-- Add here
)
```

## Advantages of This Approach

### **Total Flexibility**
- Target chosen at build-time, not compile-time
- Native CMake support for multi-configuration
- Easy integration with IDEs (CLion, VS Code, etc.)

### **Performance**
- Parallel builds for different targets
- Independent cache per configuration
- Efficient incremental builds

### **Maintainability**
- Centralized configuration in CMake files
- Easy addition of new targets
- Compatibility with standard tools

### **Testing**
- Each target can have its own test suite
- Easy CI/CD automation
- Configuration isolation

## Conclusion

This approach offers **maximum flexibility** while maintaining **ease of use**. You can:

- **Developers**: Use the script for convenience
- **CI/CD**: Use CMake directly for total control
- **IDEs**: Configure different targets as profiles
- **New Targets**: Add without modifying build system

Choosing the target at CMake time is truly the most elegant and flexible approach!
