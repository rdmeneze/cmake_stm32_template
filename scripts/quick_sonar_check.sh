#!/bin/bash
#
# Minimal SonarQube Tools Check for CI/CD
# Only installs if tools are missing
#

set -e

echo "=== SonarQube Tools Quick Check ==="

# Check if sonar-scanner is available
if command -v sonar-scanner >/dev/null 2>&1; then
    echo "✅ SonarQube Scanner: $(which sonar-scanner)"
    echo "Version: $(sonar-scanner --version 2>/dev/null | head -1 || echo 'Unknown')"
    SCANNER_OK=true
else
    echo "❌ SonarQube Scanner not found"
    SCANNER_OK=false
fi

# Check build wrapper
if command -v build-wrapper-linux-x86-64 >/dev/null 2>&1; then
    echo "✅ Build Wrapper: $(which build-wrapper-linux-x86-64)"
    WRAPPER_OK=true
elif [ -f "$HOME/.local/share/sonar-tools/build-wrapper/build-wrapper-linux-x86-64" ]; then
    echo "✅ Build Wrapper: $HOME/.local/share/sonar-tools/build-wrapper/build-wrapper-linux-x86-64"
    export PATH="$HOME/.local/share/sonar-tools/build-wrapper:$PATH"
    WRAPPER_OK=true
elif [ -f "/opt/sonar-tools/build-wrapper/build-wrapper-linux-x86-64" ]; then
    echo "✅ Build Wrapper: /opt/sonar-tools/build-wrapper/build-wrapper-linux-x86-64"
    export PATH="/opt/sonar-tools/build-wrapper:$PATH"
    WRAPPER_OK=true
else
    echo "❌ Build Wrapper not found"
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
    echo "✅ cppcheck: $(which cppcheck)"
else
    echo "❌ cppcheck not available (install: $INSTALL_HINT cppcheck)"
fi

if command -v lcov >/dev/null 2>&1; then
    echo "✅ lcov: $(which lcov)"
else
    echo "❌ lcov not available (install: $INSTALL_HINT lcov)"
fi

if command -v lizard >/dev/null 2>&1; then
    echo "✅ lizard: $(which lizard)"
else
    echo "❌ lizard not available (install: pip3 install lizard)"
fi

if command -v gcov >/dev/null 2>&1; then
    echo "✅ gcov: $(which gcov)"
else
    echo "❌ gcov not available (install: $INSTALL_HINT gcc)"
fi

# Only run full setup if essential tools are missing
if [ "$SCANNER_OK" = false ] || [ "$WRAPPER_OK" = false ]; then
    echo ""
    echo "🔧 Some tools are missing, running full setup..."
    ./scripts/setup_sonar_ci.sh
else
    echo ""
    echo "✅ All SonarQube tools are available!"
    echo "Skipping installation to save time."
fi

# Ensure tools are in PATH for subsequent steps
export PATH="$HOME/.local/share/sonar-tools/sonar-scanner/bin:$PATH"
export PATH="$HOME/.local/share/sonar-tools/build-wrapper:$PATH"
export PATH="/opt/sonar-tools/sonar-scanner/bin:$PATH"
export PATH="/opt/sonar-tools/build-wrapper:$PATH"

echo "PATH updated with SonarQube tools"
