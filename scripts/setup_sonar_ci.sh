#!/bin/bash
#
# Fast SonarQube Tools Installation for CI/CD Runner
# Optimized for self-hosted runners with caching
#

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}🚀 Fast SonarQube Tools Setup for CI/CD${NC}"
echo "=========================================="

# Global tools directory (persistent across builds)
GLOBAL_TOOLS_DIR="/opt/sonar-tools"
LOCAL_TOOLS_DIR="$HOME/.local/share/sonar-tools"

# Try global first, fallback to local
if [ -w "/opt" ] 2>/dev/null; then
    TOOLS_DIR="$GLOBAL_TOOLS_DIR"
    echo -e "${GREEN}✅ Using global tools directory: $TOOLS_DIR${NC}"
else
    TOOLS_DIR="$LOCAL_TOOLS_DIR"
    echo -e "${YELLOW}⚠️  Using local tools directory: $TOOLS_DIR${NC}"
fi

mkdir -p "$TOOLS_DIR"

# Tool versions
SCANNER_VERSION="4.8.0.2856"
SCANNER_DIR="$TOOLS_DIR/sonar-scanner-${SCANNER_VERSION}-linux"
BUILD_WRAPPER_DIR="$TOOLS_DIR/build-wrapper-linux-x86"

# Check if tools are already available globally
TOOLS_READY=true

# Check SonarQube Scanner
if [ -x "$SCANNER_DIR/bin/sonar-scanner" ]; then
    echo -e "${GREEN}✅ SonarQube Scanner found at $SCANNER_DIR${NC}"
else
    if command -v sonar-scanner &> /dev/null; then
        echo -e "${GREEN}✅ SonarQube Scanner found in system PATH${NC}"
    else
        TOOLS_READY=false
    fi
fi

# Check Build Wrapper
if [ -x "$BUILD_WRAPPER_DIR/build-wrapper-linux-x86-64" ]; then
    echo -e "${GREEN}✅ Build Wrapper found at $BUILD_WRAPPER_DIR${NC}"
else
    if command -v build-wrapper-linux-x86-64 &> /dev/null; then
        echo -e "${GREEN}✅ Build Wrapper found in system PATH${NC}"
    else
        TOOLS_READY=false
    fi
fi

# Install tools if not found
if [ "$TOOLS_READY" = false ]; then
    echo -e "${BLUE}📦 Installing missing SonarQube tools...${NC}"

    cd "$TOOLS_DIR"

    # Install SonarQube Scanner
    if [ ! -x "$SCANNER_DIR/bin/sonar-scanner" ]; then
        SCANNER_ZIP="sonar-scanner-cli-${SCANNER_VERSION}-linux.zip"

        if [ ! -f "$SCANNER_ZIP" ]; then
            echo -e "${BLUE}📥 Downloading SonarQube Scanner...${NC}"
            wget -q --show-progress "https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/${SCANNER_ZIP}"
        fi

        echo -e "${BLUE}📦 Extracting SonarQube Scanner...${NC}"
        unzip -q "$SCANNER_ZIP"
        chmod +x "$SCANNER_DIR/bin/sonar-scanner"
    fi

    # Install Build Wrapper
    if [ ! -x "$BUILD_WRAPPER_DIR/build-wrapper-linux-x86-64" ]; then
        BUILD_WRAPPER_ZIP="build-wrapper-linux-x86.zip"

        if [ ! -f "$BUILD_WRAPPER_ZIP" ]; then
            echo -e "${BLUE}📥 Downloading Build Wrapper...${NC}"
            wget -q --show-progress "https://sonarcloud.io/static/cpp/${BUILD_WRAPPER_ZIP}"
        fi

        echo -e "${BLUE}📦 Extracting Build Wrapper...${NC}"
        unzip -q "$BUILD_WRAPPER_ZIP"
        chmod +x "$BUILD_WRAPPER_DIR/build-wrapper-linux-x86-64"
    fi

    cd - > /dev/null
fi

# Install system packages (only if not available)
echo -e "${BLUE}📦 Checking system packages...${NC}"

PACKAGES_TO_INSTALL=()

# Detect package manager and required packages
if command -v dnf >/dev/null 2>&1; then
    # Fedora/RHEL/CentOS
    PKG_MANAGER="dnf"
    REQUIRED_PACKAGES=("lcov" "cppcheck" "unzip" "wget" "python3-pip")
    CHECK_CMD="rpm -q"
    INSTALL_CMD="sudo dnf install -y"
    UPDATE_CMD="sudo dnf check-update || true"
elif command -v yum >/dev/null 2>&1; then
    # Older RHEL/CentOS
    PKG_MANAGER="yum"
    REQUIRED_PACKAGES=("lcov" "cppcheck" "unzip" "wget" "python3-pip")
    CHECK_CMD="rpm -q"
    INSTALL_CMD="sudo yum install -y"
    UPDATE_CMD="sudo yum check-update || true"
elif command -v apt-get >/dev/null 2>&1; then
    # Debian/Ubuntu
    PKG_MANAGER="apt"
    REQUIRED_PACKAGES=("lcov" "cppcheck" "unzip" "wget" "python3-pip")
    CHECK_CMD="dpkg -l | grep -q '^ii'"
    INSTALL_CMD="sudo apt-get install -y"
    UPDATE_CMD="sudo apt-get update -q"
else
    echo -e "${YELLOW}⚠️  Unknown package manager. Skipping system package installation.${NC}"
    PKG_MANAGER="unknown"
fi

# Check required packages
PACKAGES_TO_INSTALL=()

if [ "$PKG_MANAGER" != "unknown" ]; then
    for package in "${REQUIRED_PACKAGES[@]}"; do
        if [ "$PKG_MANAGER" = "apt" ]; then
            if ! dpkg -l | grep -q "^ii  $package " 2>/dev/null; then
                PACKAGES_TO_INSTALL+=("$package")
            fi
        else
            # RPM-based (dnf/yum)
            if ! rpm -q "$package" >/dev/null 2>&1; then
                PACKAGES_TO_INSTALL+=("$package")
            fi
        fi
    done
fi

# Install missing packages
if [ ${#PACKAGES_TO_INSTALL[@]} -gt 0 ]; then
    echo -e "${BLUE}📦 Installing packages with $PKG_MANAGER: ${PACKAGES_TO_INSTALL[*]}${NC}"
    $UPDATE_CMD
    $INSTALL_CMD "${PACKAGES_TO_INSTALL[@]}"
else
    echo -e "${GREEN}✅ All system packages already installed${NC}"
fi

# Install Python tools
echo -e "${BLUE}🐍 Checking Python tools...${NC}"

PYTHON_TOOLS=("lizard")
for tool in "${PYTHON_TOOLS[@]}"; do
    if ! pip3 list 2>/dev/null | grep -q "$tool"; then
        echo -e "${BLUE}  Installing $tool...${NC}"
        pip3 install "$tool" --break-system-packages 2>/dev/null || pip3 install "$tool"
    else
        echo -e "${GREEN}✅ $tool already installed${NC}"
    fi
done

# Export PATH for current session
export PATH="$SCANNER_DIR/bin:$PATH"
export PATH="$BUILD_WRAPPER_DIR:$PATH"

# Create environment file for GitHub Actions
if [ -n "$GITHUB_ENV" ]; then
    echo "PATH=$SCANNER_DIR/bin:$BUILD_WRAPPER_DIR:$PATH" >> "$GITHUB_ENV"
fi

# Verification
echo -e "${BLUE}🔍 Verification${NC}"
echo "===================="

SUCCESS=true

if command -v sonar-scanner &> /dev/null; then
    VERSION=$(sonar-scanner --version 2>/dev/null | head -1)
    echo -e "${GREEN}✅ SonarQube Scanner: $VERSION${NC}"
else
    echo -e "${RED}❌ SonarQube Scanner not found${NC}"
    SUCCESS=false
fi

if command -v build-wrapper-linux-x86-64 &> /dev/null; then
    echo -e "${GREEN}✅ Build Wrapper: Available${NC}"
else
    echo -e "${RED}❌ Build Wrapper not found${NC}"
    SUCCESS=false
fi

SYSTEM_TOOLS=("lcov:Coverage" "cppcheck:Static Analysis" "lizard:Complexity")
for tool_info in "${SYSTEM_TOOLS[@]}"; do
    IFS=':' read -r tool desc <<< "$tool_info"
    if command -v "$tool" &> /dev/null; then
        echo -e "${GREEN}✅ $desc: Available${NC}"
    else
        echo -e "${YELLOW}⚠️  $desc: Not available${NC}"
    fi
done

if [ "$SUCCESS" = true ]; then
    echo -e "\n${GREEN}🎉 All SonarQube tools ready!${NC}"
    exit 0
else
    echo -e "\n${RED}❌ Some tools missing. Please check the installation.${NC}"
    exit 1
fi
