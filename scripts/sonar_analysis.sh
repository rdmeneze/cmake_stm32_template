#!/bin/bash
#
# SonarQube Analysis Script for STM32 Firmware
#
# Prerequisites:
# - SonarQube Scanner installed and in PATH
# - Build tools (cmake, make, gcc)
# - Analysis tools (cppcheck, lizard, lcov) - optional
# - Build wrapper for C/C++ analysis - optional
#
# Usage:
#   SONAR_TOKEN=your_token ./scripts/sonar_analysis.sh
#   SONAR_ANALYSIS_LOCAL=1 ./scripts/sonar_analysis.sh  # Local analysis only
#

set -e

# Configuration
SONAR_SERVER="http://fedora-server.local:9000"  # Replace with your server
SONAR_TOKEN="${SONAR_TOKEN:-}"  # Set as environment variable
PROJECT_KEY="stm32-firmware-template"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}Starting SonarQube Analysis for STM32 Firmware${NC}"
echo "=================================="

# Check if sonar-scanner is available
if ! command -v sonar-scanner &> /dev/null; then
    echo -e "${RED}ERROR: sonar-scanner not found in PATH${NC}"
    echo "Please install SonarQube Scanner using:"
    echo "  ./scripts/install_sonar_tools.sh"
    echo "Or refer to scripts/README_SONAR.md for installation instructions"
    exit 1
fi

# Check for build-wrapper (for C/C++ analysis)
if ! command -v build-wrapper-linux-x86-64 &> /dev/null; then
    echo -e "${YELLOW}WARNING: build-wrapper not found. C/C++ analysis may be limited.${NC}"
    BUILD_WRAPPER_AVAILABLE=false
else
    BUILD_WRAPPER_AVAILABLE=true
    echo "Using build-wrapper for enhanced C/C++ analysis"
fi

# Clean previous analysis data
echo "Cleaning previous analysis data..."
rm -rf build-wrapper-output coverage .scannerwork

# Create coverage directory
mkdir -p coverage

echo "Generating code coverage..."

# Build with coverage flags
if [ "$BUILD_WRAPPER_AVAILABLE" = true ]; then
    echo "Building with build-wrapper for accurate C/C++ analysis..."

    # Clean build
    make clean || true
    rm -rf build-*

    # Configure with coverage
    cmake -B build-coverage \
          -DCMAKE_BUILD_TYPE=Debug \
          -DCMAKE_C_FLAGS="--coverage -fprofile-arcs -ftest-coverage" \
          -DCMAKE_CXX_FLAGS="--coverage -fprofile-arcs -ftest-coverage" \
          -DCMAKE_EXE_LINKER_FLAGS="--coverage"

    # Build with wrapper
    build-wrapper-linux-x86-64 --out-dir build-wrapper-output \
        cmake --build build-coverage
else
    echo -e "${YELLOW}WARNING: Building without build-wrapper...${NC}"
    ./build_portable.sh stm32l432kc Debug
fi

# Run tests to generate coverage
echo "Running tests for coverage..."
if [ -f "./run_tests.sh" ]; then
    ./run_tests.sh

    # Process coverage data
    echo "Processing coverage data..."

    # Generate XML coverage report
    if command -v gcov &> /dev/null; then
        find . -name "*.gcno" -exec gcov {} \;

        # Convert to XML if lcov is available
        if command -v lcov &> /dev/null; then
            lcov --capture --directory . --output-file coverage/coverage.info
            lcov --remove coverage/coverage.info '/usr/*' --output-file coverage/coverage.info
            lcov --remove coverage/coverage.info '*/tests/*' --output-file coverage/coverage.info
            lcov --remove coverage/coverage.info '*/third_party/*' --output-file coverage/coverage.info

            # Generate XML for SonarQube
            if command -v genhtml &> /dev/null; then
                genhtml coverage/coverage.info --output-directory coverage/html
            fi
        fi
    fi
else
    echo -e "${YELLOW}WARNING: No test script found, skipping coverage...${NC}"
fi

# Run static analysis tools
echo "Running static analysis..."

# Cppcheck analysis
if command -v cppcheck &> /dev/null; then
    echo "  Running Cppcheck..."
    cppcheck --enable=all --xml --xml-version=2 \
             --suppress=missingIncludeSystem \
             --suppress=unmatchedSuppression \
             --suppress=unusedFunction \
             src/ 2> coverage/cppcheck.xml || true
fi

# Complexity analysis
if command -v lizard &> /dev/null; then
    echo "  Running complexity analysis..."
    lizard src/ --xml > coverage/lizard.xml || true
fi

# Run SonarQube analysis
echo "Running SonarQube Scanner..."

# Check if local analysis only
if [ "${SONAR_ANALYSIS_LOCAL:-}" = "1" ]; then
    echo "LOCAL ANALYSIS MODE - Results will not be uploaded to SonarQube server"
fi

SONAR_PARAMS=(
    "-Dsonar.projectKey=${PROJECT_KEY}"
    "-Dsonar.sources=src"
    "-Dsonar.tests=tests"
    "-Dsonar.host.url=${SONAR_SERVER}"
)

# Add token if provided and not in local mode
if [ "${SONAR_ANALYSIS_LOCAL:-}" != "1" ]; then
    if [ -n "${SONAR_TOKEN}" ]; then
        SONAR_PARAMS+=("-Dsonar.login=${SONAR_TOKEN}")
    elif [ -n "${SONAR_LOGIN_TOKEN}" ]; then
        SONAR_PARAMS+=("-Dsonar.login=${SONAR_LOGIN_TOKEN}")
    else
        echo -e "${YELLOW}WARNING: No SonarQube token provided. Make sure server allows anonymous analysis.${NC}"
    fi
fi

# Add build wrapper output if available
if [ "$BUILD_WRAPPER_AVAILABLE" = true ] && [ -d "build-wrapper-output" ]; then
    SONAR_PARAMS+=("-Dsonar.cfamily.build-wrapper-output=build-wrapper-output")
fi

# Execute scanner
if [ "${SONAR_ANALYSIS_LOCAL:-}" = "1" ]; then
    echo "Skipping SonarQube upload (local analysis mode)"
else
    sonar-scanner "${SONAR_PARAMS[@]}"
fi

echo -e "${GREEN}SonarQube analysis completed successfully${NC}"
echo -e "${BLUE}View results at: ${SONAR_SERVER}/dashboard?id=${PROJECT_KEY}${NC}"

# Generate summary report
echo ""
echo "Analysis Summary"
echo "=================================="
echo "Project: ${PROJECT_KEY}"
echo "Server: ${SONAR_SERVER}"
echo "Build Wrapper: $([ "$BUILD_WRAPPER_AVAILABLE" = true ] && echo "Used" || echo "Not available")"
echo "Coverage: $([ -f "coverage/coverage.info" ] && echo "Generated" || echo "Not generated")"
echo "Static Analysis: $([ -f "coverage/cppcheck.xml" ] && echo "Cppcheck executed" || echo "Cppcheck not available")"
echo "Complexity Analysis: $([ -f "coverage/lizard.xml" ] && echo "Lizard executed" || echo "Lizard not available")"

if [ "${SONAR_ANALYSIS_LOCAL:-}" != "1" ]; then
    if [ -f ".scannerwork/report-task.txt" ]; then
        echo -e "${GREEN}Analysis uploaded successfully${NC}"
        echo ""
        echo "Report Details:"
        cat .scannerwork/report-task.txt
    else
        echo -e "${RED}Analysis may have failed - no report task found${NC}"
        exit 1
    fi
else
    echo "Local analysis completed - no upload performed"
fi
