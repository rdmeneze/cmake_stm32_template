#!/bin/bash
#
# Install SonarQube Scanner and Analysis Tools
#

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}🛠️  Installing SonarQube Scanner and Analysis Tools${NC}"
echo "=================================================="

# Create tools directory
TOOLS_DIR="$HOME/tools"
mkdir -p "$TOOLS_DIR"

# Install SonarQube Scanner
echo -e "${BLUE}📦 Installing SonarQube Scanner...${NC}"

SCANNER_VERSION="4.8.0.2856"
SCANNER_ZIP="sonar-scanner-cli-${SCANNER_VERSION}-linux.zip"
SCANNER_DIR="sonar-scanner-${SCANNER_VERSION}-linux"

cd "$TOOLS_DIR"

if [ ! -d "$SCANNER_DIR" ]; then
    if [ ! -f "$SCANNER_ZIP" ]; then
        echo -e "${BLUE}📥 Downloading SonarQube Scanner v${SCANNER_VERSION}...${NC}"
        wget -q --show-progress "https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/${SCANNER_ZIP}"
    fi

    echo -e "${BLUE}📦 Extracting SonarQube Scanner...${NC}"
    unzip -q "$SCANNER_ZIP"

    # Make executable
    chmod +x "$SCANNER_DIR/bin/sonar-scanner"
else
    echo -e "${GREEN}✅ SonarQube Scanner already installed${NC}"
fi

# Install Build Wrapper (for C/C++ analysis)
echo -e "${BLUE}📦 Installing Build Wrapper...${NC}"

BUILD_WRAPPER_ZIP="build-wrapper-linux-x86.zip"
BUILD_WRAPPER_DIR="build-wrapper-linux-x86"

if [ ! -d "$BUILD_WRAPPER_DIR" ]; then
    if [ ! -f "$BUILD_WRAPPER_ZIP" ]; then
        echo -e "${BLUE}📥 Downloading Build Wrapper...${NC}"
        wget -q --show-progress "https://sonarcloud.io/static/cpp/${BUILD_WRAPPER_ZIP}"
    fi

    echo -e "${BLUE}📦 Extracting Build Wrapper...${NC}"
    unzip -q "$BUILD_WRAPPER_ZIP"

    # Make executable
    chmod +x "$BUILD_WRAPPER_DIR/build-wrapper-linux-x86-64"
else
    echo -e "${GREEN}✅ Build Wrapper already installed${NC}"
fi

cd - > /dev/null

# Install system packages
echo -e "${BLUE}📦 Installing system packages...${NC}"

# Update package list
sudo apt-get update -q

# Install required packages
PACKAGES=(
    "lcov"          # Code coverage
    "gcov"          # Coverage data collection
    "cppcheck"      # Static analysis
    "unzip"         # For extracting archives
    "wget"          # For downloading
    "python3-pip"   # For Python tools
)

for package in "${PACKAGES[@]}"; do
    if ! dpkg -l | grep -q "^ii  $package "; then
        echo -e "${BLUE}  Installing $package...${NC}"
        sudo apt-get install -y "$package"
    else
        echo -e "${GREEN}✅ $package already installed${NC}"
    fi
done

# Install Python tools
echo -e "${BLUE}🐍 Installing Python analysis tools...${NC}"

PYTHON_PACKAGES=(
    "lizard"        # Complexity analysis
    "coverage"      # Python coverage (if needed)
)

for package in "${PYTHON_PACKAGES[@]}"; do
    if ! pip3 list | grep -q "$package"; then
        echo -e "${BLUE}  Installing $package...${NC}"
        pip3 install "$package" --break-system-packages 2>/dev/null || pip3 install "$package"
    else
        echo -e "${GREEN}✅ $package already installed${NC}"
    fi
done

# Create or update .bashrc entry
echo -e "${BLUE}⚙️  Configuring PATH...${NC}"

BASHRC_ENTRY="
# SonarQube Scanner and Build Tools
export PATH=\"\$HOME/tools/sonar-scanner-${SCANNER_VERSION}-linux/bin:\$PATH\"
export PATH=\"\$HOME/tools/build-wrapper-linux-x86:\$PATH\"
"

# Check if already in .bashrc
if ! grep -q "sonar-scanner-${SCANNER_VERSION}-linux" ~/.bashrc 2>/dev/null; then
    echo "$BASHRC_ENTRY" >> ~/.bashrc
    echo -e "${GREEN}✅ Added tools to PATH in ~/.bashrc${NC}"
else
    echo -e "${GREEN}✅ Tools already in PATH${NC}"
fi

# Export for current session
export PATH="$HOME/tools/sonar-scanner-${SCANNER_VERSION}-linux/bin:$PATH"
export PATH="$HOME/tools/build-wrapper-linux-x86:$PATH"

# Verify installations
echo -e "${BLUE}🔍 Verifying installations...${NC}"
echo "=================================="

# Check SonarQube Scanner
if command -v sonar-scanner &> /dev/null; then
    VERSION=$(sonar-scanner --version 2>/dev/null | head -1)
    echo -e "${GREEN}✅ SonarQube Scanner: $VERSION${NC}"
else
    echo -e "${RED}❌ SonarQube Scanner not found${NC}"
fi

# Check Build Wrapper
if command -v build-wrapper-linux-x86-64 &> /dev/null; then
    echo -e "${GREEN}✅ Build Wrapper: Available${NC}"
else
    echo -e "${RED}❌ Build Wrapper not found${NC}"
fi

# Check other tools
TOOLS_CHECK=(
    "lcov:Code Coverage"
    "gcov:Coverage Collection"
    "cppcheck:Static Analysis"
    "lizard:Complexity Analysis"
)

for tool_info in "${TOOLS_CHECK[@]}"; do
    IFS=':' read -r tool desc <<< "$tool_info"
    if command -v "$tool" &> /dev/null; then
        echo -e "${GREEN}✅ $desc ($tool): Available${NC}"
    else
        echo -e "${YELLOW}⚠️  $desc ($tool): Not available${NC}"
    fi
done

echo -e "\n${GREEN}🎉 Installation completed!${NC}"
echo -e "${BLUE}💡 To use the tools in new terminal sessions, run:${NC}"
echo -e "${YELLOW}   source ~/.bashrc${NC}"
echo -e "${BLUE}💡 Or restart your terminal${NC}"

echo -e "\n${BLUE}🚀 Ready to run SonarQube analysis!${NC}"
echo -e "${YELLOW}   ./scripts/sonar_analysis.sh${NC}"
