#!/bin/bash
#
# Minimal SonarQube Tools Check for CI/CD
# Only installs if tools are missing
#

set -e

echo "=== SonarQube Tools Quick Check ==="

# Check local sonar tools first (most likely location)
LOCAL_SONAR_DIR="$HOME/.local/share/sonar-tools"

# Check if sonar-scanner is available
if command -v sonar-scanner >/dev/null 2>&1; then
    echo "SUCCESS: SonarQube Scanner: $(which sonar-scanner)"
    echo "Version: $(sonar-scanner --version 2>/dev/null | head -1 || echo 'Unknown')"
    SCANNER_OK=true
elif [ -f "$LOCAL_SONAR_DIR/sonar-scanner-4.8.0.2856-linux/bin/sonar-scanner" ]; then
    echo "SUCCESS: SonarQube Scanner: $LOCAL_SONAR_DIR/sonar-scanner-4.8.0.2856-linux/bin/sonar-scanner"
    export PATH="$LOCAL_SONAR_DIR/sonar-scanner-4.8.0.2856-linux/bin:$PATH"
    SCANNER_OK=true
elif [ -f "/opt/sonar-tools/sonar-scanner-4.8.0.2856-linux/bin/sonar-scanner" ]; then
    echo "SUCCESS: SonarQube Scanner: /opt/sonar-tools/sonar-scanner-4.8.0.2856-linux/bin/sonar-scanner"
    export PATH="/opt/sonar-tools/sonar-scanner-4.8.0.2856-linux/bin:$PATH"
    SCANNER_OK=true
else
    echo "ERROR: SonarQube Scanner not found"
    SCANNER_OK=false
fi

# Check build wrapper
if command -v build-wrapper-linux-x86-64 >/dev/null 2>&1; then
    echo "SUCCESS: Build Wrapper: $(which build-wrapper-linux-x86-64)"
    WRAPPER_OK=true
elif [ -f "$LOCAL_SONAR_DIR/build-wrapper-linux-x86/build-wrapper-linux-x86-64" ]; then
    echo "SUCCESS: Build Wrapper: $LOCAL_SONAR_DIR/build-wrapper-linux-x86/build-wrapper-linux-x86-64"
    export PATH="$LOCAL_SONAR_DIR/build-wrapper-linux-x86:$PATH"
    WRAPPER_OK=true
elif [ -f "/opt/sonar-tools/build-wrapper-linux-x86/build-wrapper-linux-x86-64" ]; then
    echo "SUCCESS: Build Wrapper: /opt/sonar-tools/build-wrapper-linux-x86/build-wrapper-linux-x86-64"
    export PATH="/opt/sonar-tools/build-wrapper-linux-x86:$PATH"
    WRAPPER_OK=true
else
    echo "ERROR: Build Wrapper not found"
    WRAPPER_OK=false
fi

# Check system tools
echo ""
echo "=== System Analysis Tools ==="

# Detect OS for better package suggestions
if [ -f /etc/fedora-release ]; then
    OS_TYPE="fedora"
    INSTALL_HINT="sudo dnf install"
elif [ -f /etc/redhat-release ]; then
    OS_TYPE="rhel"
    INSTALL_HINT="sudo dnf install"
elif [ -f /etc/debian_version ]; then
    OS_TYPE="debian"
    INSTALL_HINT="sudo apt-get install"
else
    OS_TYPE="unknown"
    INSTALL_HINT="package manager install"
fi

echo "Detected OS: $OS_TYPE"

# Check analysis tools with install hints
if command -v cppcheck >/dev/null 2>&1; then
    echo "SUCCESS: cppcheck: $(which cppcheck)"
else
    echo "ERROR: cppcheck not available (install: $INSTALL_HINT cppcheck)"
fi

if command -v lcov >/dev/null 2>&1; then
    echo "SUCCESS: lcov: $(which lcov)"
else
    echo "ERROR: lcov not available (install: $INSTALL_HINT lcov)"
fi

if command -v lizard >/dev/null 2>&1; then
    echo "SUCCESS: lizard: $(which lizard)"
else
    echo "ERROR: lizard not available (install: pip3 install lizard)"
fi

if command -v gcov >/dev/null 2>&1; then
    echo "SUCCESS: gcov: $(which gcov)"
else
    echo "ERROR: gcov not available (install: $INSTALL_HINT gcc)"
fi

# Only run full setup if essential tools are missing
if [ "$SCANNER_OK" = false ] || [ "$WRAPPER_OK" = false ]; then
    echo ""
    echo "WARNING: Some tools are missing, running full setup..."
    ./scripts/setup_sonar_ci.sh
else
    echo ""
    echo "SUCCESS: All SonarQube tools are available!"
    echo "Skipping installation to save time."
fi

# Ensure all possible paths are in PATH for subsequent steps
export PATH="$LOCAL_SONAR_DIR/sonar-scanner-4.8.0.2856-linux/bin:$PATH"
export PATH="$LOCAL_SONAR_DIR/build-wrapper-linux-x86:$PATH"
export PATH="/opt/sonar-tools/sonar-scanner-4.8.0.2856-linux/bin:$PATH"
export PATH="/opt/sonar-tools/build-wrapper-linux-x86:$PATH"

echo "PATH updated with SonarQube tools"
