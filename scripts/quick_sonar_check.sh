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

# Check system tools (Fedora packages)
echo ""
echo "=== System Analysis Tools ==="
command -v cppcheck >/dev/null 2>&1 && echo "✅ cppcheck: $(which cppcheck)" || echo "❌ cppcheck not available"
command -v lcov >/dev/null 2>&1 && echo "✅ lcov: $(which lcov)" || echo "❌ lcov not available" 
command -v lizard >/dev/null 2>&1 && echo "✅ lizard: $(which lizard)" || echo "❌ lizard not available"
command -v gcov >/dev/null 2>&1 && echo "✅ gcov: $(which gcov)" || echo "❌ gcov not available"

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
