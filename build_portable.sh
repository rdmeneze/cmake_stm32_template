#!/bin/bash

# STM32 Portable Firmware Build Script
# Usage: ./build_portable.sh [target] [build_type] [action]

set -e  # Exit on error

# Default values
DEFAULT_TARGET="stm32l432kc"
DEFAULT_BUILD_TYPE="Debug"
DEFAULT_ACTION="build"

# Available targets
TARGETS=(
    "stm32l432kc"
    "stm32f407vg"
)

# Available build types
BUILD_TYPES=("Debug" "Release")

# Available actions
ACTIONS=("configure" "build" "clean" "flash" "debug")

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Functions
print_usage() {
    echo "STM32 Portable Firmware Build Script"
    echo ""
    echo "Usage: $0 [target] [build_type] [action]"
    echo ""
    echo "Alternative CMake usage (more flexible):"
    echo "  cmake -B build-custom -DTARGET_BOARD=stm32f407vg -DCMAKE_BUILD_TYPE=Release"
    echo "  cmake --build build-custom"
    echo ""
    echo "Available Targets:"
    for target in "${TARGETS[@]}"; do
        echo "  - $target"
    done
    echo ""
    echo "Build Types:"
    for build_type in "${BUILD_TYPES[@]}"; do
        echo "  - $build_type"
    done
    echo ""
    echo "Actions:"
    for action in "${ACTIONS[@]}"; do
        echo "  - $action"
    done
    echo ""
    echo "Script Examples:"
    echo "  $0                          # Build STM32L432KC in Debug mode"
    echo "  $0 stm32f407vg              # Build STM32F407VG in Debug mode"
    echo "  $0 stm32l432kc Release      # Build STM32L432KC in Release mode"
    echo "  $0 stm32f407vg Debug clean  # Clean STM32F407VG Debug build"
    echo "  $0 stm32l432kc Debug flash  # Flash STM32L432KC firmware"
    echo ""
    echo "Direct CMake Examples (recommended for CI/CD):"
    echo "  cmake -B build-l432kc -DTARGET_BOARD=stm32l432kc"
    echo "  cmake -B build-f407vg -DTARGET_BOARD=stm32f407vg -DCMAKE_BUILD_TYPE=Release"
    echo "  cmake --build build-l432kc --config Debug"
}

print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

validate_target() {
    local target=$1
    for valid_target in "${TARGETS[@]}"; do
        if [[ "$target" == "$valid_target" ]]; then
            return 0
        fi
    done
    return 1
}

validate_build_type() {
    local build_type=$1
    for valid_type in "${BUILD_TYPES[@]}"; do
        if [[ "$build_type" == "$valid_type" ]]; then
            return 0
        fi
    done
    return 1
}

validate_action() {
    local action=$1
    for valid_action in "${ACTIONS[@]}"; do
        if [[ "$action" == "$valid_action" ]]; then
            return 0
        fi
    done
    return 1
}

check_dependencies() {
    # Check for required tools
    local tools=("cmake" "arm-none-eabi-gcc" "git")
    
    for tool in "${tools[@]}"; do
        if ! command -v "$tool" &> /dev/null; then
            print_error "$tool is not installed or not in PATH"
            return 1
        fi
    done
    
    # Check for STM32CubeL4 submodule
    if [[ ! -d "third_party/STM32CubeL4" ]]; then
        print_warning "STM32CubeL4 submodule not found. Attempting to initialize..."
        git submodule update --init --recursive
        if [[ $? -ne 0 ]]; then
            print_error "Failed to initialize STM32CubeL4 submodule"
            return 1
        fi
    fi
    
    return 0
}

configure_project() {
    local target=$1
    local build_type=$2
    local build_dir="build-${target}-${build_type,,}"  # Convert to lowercase
    
    print_info "Configuring project for $target ($build_type)..."
    print_info "Build directory: $build_dir"
    
    # Use portable CMakeLists.txt if it exists
    local cmake_file="CMakeLists.txt"
    if [[ -f "CMakeLists_portable.txt" ]]; then
        cmake_file="CMakeLists_portable.txt"
        print_info "Using portable CMake configuration"
    fi
    
    cmake -B "$build_dir" \
          -DCMAKE_BUILD_TYPE="$build_type" \
          -DTARGET_BOARD="$target" \
          -DCMAKE_TOOLCHAIN_FILE="cmake/arm-gcc-toolchain.cmake" \
          -f "$cmake_file"
    
    if [[ $? -eq 0 ]]; then
        print_success "Configuration completed successfully"
    else
        print_error "Configuration failed"
        return 1
    fi
}

build_project() {
    local target=$1
    local build_type=$2
    local build_dir="build-${target}-${build_type,,}"
    
    if [[ ! -d "$build_dir" ]]; then
        print_warning "Build directory $build_dir not found. Configuring first..."
        configure_project "$target" "$build_type"
        if [[ $? -ne 0 ]]; then
            return 1
        fi
    fi
    
    print_info "Building project for $target ($build_type)..."
    
    cmake --build "$build_dir" --config "$build_type" -j$(nproc)
    
    if [[ $? -eq 0 ]]; then
        print_success "Build completed successfully"
        
        # Show memory usage if available
        local elf_file="$build_dir/${target}_firmware"
        if [[ -f "$elf_file" ]]; then
            print_info "Memory usage:"
            arm-none-eabi-size "$elf_file"
        fi
        
        # Show generated files
        print_info "Generated files:"
        ls -la "$build_dir"/*.{bin,hex,map} 2>/dev/null || true
    else
        print_error "Build failed"
        return 1
    fi
}

clean_project() {
    local target=$1
    local build_type=$2
    local build_dir="build-${target}-${build_type,,}"
    
    print_info "Cleaning $target ($build_type)..."
    
    if [[ -d "$build_dir" ]]; then
        rm -rf "$build_dir"
        print_success "Clean completed"
    else
        print_warning "Build directory $build_dir does not exist"
    fi
}

flash_firmware() {
    local target=$1
    local build_type=$2
    local build_dir="build-${target}-${build_type,,}"
    local hex_file="$build_dir/${target}_firmware.hex"
    
    if [[ ! -f "$hex_file" ]]; then
        print_error "Firmware hex file not found: $hex_file"
        print_info "Please build the project first"
        return 1
    fi
    
    print_info "Flashing firmware to $target..."
    
    # Use existing flash script if available
    if [[ -f "flash_firmware.sh" ]]; then
        ./flash_firmware.sh "$hex_file"
    else
        # Try st-flash if available
        if command -v st-flash &> /dev/null; then
            st-flash --format ihex write "$hex_file"
        else
            print_error "No flash tool available. Install st-link tools or create flash_firmware.sh script"
            return 1
        fi
    fi
    
    if [[ $? -eq 0 ]]; then
        print_success "Firmware flashed successfully"
    else
        print_error "Flash failed"
        return 1
    fi
}

debug_firmware() {
    local target=$1
    local build_type=$2
    local build_dir="build-${target}-${build_type,,}"
    local elf_file="$build_dir/${target}_firmware"
    
    if [[ ! -f "$elf_file" ]]; then
        print_error "Firmware ELF file not found: $elf_file"
        print_info "Please build the project first"
        return 1
    fi
    
    print_info "Starting debug session for $target..."
    
    # Use existing debug script if available
    if [[ -f "debug_firmware.sh" ]]; then
        ./debug_firmware.sh "$elf_file"
    else
        print_error "Debug script not found. Create debug_firmware.sh script"
        return 1
    fi
}

# Main script

# Parse arguments
TARGET=${1:-$DEFAULT_TARGET}
BUILD_TYPE=${2:-$DEFAULT_BUILD_TYPE}
ACTION=${3:-$DEFAULT_ACTION}

# Show help if requested
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    print_usage
    exit 0
fi

# Validate arguments
if ! validate_target "$TARGET"; then
    print_error "Invalid target: $TARGET"
    print_usage
    exit 1
fi

if ! validate_build_type "$BUILD_TYPE"; then
    print_error "Invalid build type: $BUILD_TYPE"
    print_usage
    exit 1
fi

if ! validate_action "$ACTION"; then
    print_error "Invalid action: $ACTION"
    print_usage
    exit 1
fi

# Check dependencies
if ! check_dependencies; then
    print_error "Dependency check failed"
    exit 1
fi

# Print configuration
print_info "Configuration:"
print_info "  Target: $TARGET"
print_info "  Build Type: $BUILD_TYPE"
print_info "  Action: $ACTION"
echo ""

# Execute action
case "$ACTION" in
    "configure")
        configure_project "$TARGET" "$BUILD_TYPE"
        ;;
    "build")
        configure_project "$TARGET" "$BUILD_TYPE"
        build_project "$TARGET" "$BUILD_TYPE"
        ;;
    "clean")
        clean_project "$TARGET" "$BUILD_TYPE"
        ;;
    "flash")
        flash_firmware "$TARGET" "$BUILD_TYPE"
        ;;
    "debug")
        debug_firmware "$TARGET" "$BUILD_TYPE"
        ;;
    *)
        print_error "Unknown action: $ACTION"
        exit 1
        ;;
esac

exit $?
