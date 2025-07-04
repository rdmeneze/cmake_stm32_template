#!/bin/bash

# Script to build and run unit tests for STM32L432 project
# This script builds the tests using the host compiler (not ARM cross-compiler)

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}STM32L432 Unit Test Runner${NC}"
echo "=================================="

# Project directory
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_DIR="${PROJECT_DIR}/tests"
BUILD_DIR="${TEST_DIR}/build"

# Create build directory for tests
echo -e "${YELLOW}Creating test build directory...${NC}"
mkdir -p "${BUILD_DIR}"

# Configure CMake for host testing (not cross-compiling)
echo -e "${YELLOW}Configuring CMake for host testing...${NC}"
cd "${BUILD_DIR}"

cmake "${TEST_DIR}" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_C_STANDARD=11

# Build the tests
echo -e "${YELLOW}Building unit tests...${NC}"
make unit_tests

# Run the tests
echo -e "${YELLOW}Running unit tests...${NC}"
echo "=================================="

if ./unit_tests; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed!${NC}"
    exit 1
fi
