# SonarQube Analysis for STM32 Firmware

This directory contains scripts for code quality analysis using SonarQube Community Edition.

## Tool Installation

### Automatic Installation

Run the installation script that will download and configure all necessary tools:

```bash
./scripts/install_sonar_tools.sh
```

This script will install:
- **SonarQube Scanner** (v4.8.0.2856)
- **Build Wrapper** (for accurate C/C++ analysis)
- **lcov/gcov** (code coverage)
- **cppcheck** (static analysis)
- **lizard** (complexity analysis)

### Manual Installation

If you prefer to install manually:

```bash
# 1. Install system dependencies
sudo apt-get update
sudo apt-get install -y lcov gcov cppcheck unzip wget python3-pip

# 2. Install Python tools
pip3 install lizard coverage

# 3. Download SonarQube Scanner
cd ~/tools
wget https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-4.8.0.2856-linux.zip
unzip sonar-scanner-cli-4.8.0.2856-linux.zip
export PATH="$HOME/tools/sonar-scanner-4.8.0.2856-linux/bin:$PATH"

# 4. Download Build Wrapper
wget https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip
unzip build-wrapper-linux-x86.zip
export PATH="$HOME/tools/build-wrapper-linux-x86:$PATH"
```

## SonarQube Configuration

### 1. Configure SonarQube Server

Edit the `scripts/sonar_analysis.sh` file and configure:

```bash
SONAR_SERVER="http://your-server:9000"
```

### 2. Create Access Token

On your SonarQube server:
1. Go to **My Account > Security > Generate Tokens**
2. Generate a token named "stm32-firmware-analysis"
3. Set it as an environment variable:

```bash
export SONAR_TOKEN="your_token_here"
```

### 3. Create Project in SonarQube

1. Access your SonarQube server
2. Click **"Create Project"**
3. Use the key: `stm32-firmware-template`
4. Configure Quality Gates for embedded firmware

## Running the Analysis

### Complete Analysis

```bash
# Run complete analysis with coverage
./scripts/sonar_analysis.sh
```

### Analysis with Token

```bash
# Using specific token
SONAR_TOKEN=your_token ./scripts/sonar_analysis.sh
```

### Local Analysis (without upload)

```bash
# Run tools locally only
SONAR_ANALYSIS_LOCAL=1 ./scripts/sonar_analysis.sh
```

## Analyzed Metrics

### Code Quality
- **Cyclomatic Complexity** (CCN < 15)
- **Code Duplication** (< 5%)
- **Maintainability** (A-E rating)
- **Reliability** (bugs and vulnerabilities)

### Firmware Specific
- **Memory Usage** (Flash/RAM)
- **Static Analysis** (cppcheck)
- **Test Coverage** (gcov/lcov)
- **Dangerous Pattern Detection** (malloc in ISR, recursion, etc.)

### Tools Used
- **SonarQube**: General analysis and dashboard
- **Cppcheck**: C/C++ static analysis
- **Lizard**: Cyclomatic complexity
- **LCOV**: Code coverage
- **Build Wrapper**: Accurate compilation analysis

## File Structure

```
scripts/
├── install_sonar_tools.sh    # Automatic tool installation
├── sonar_analysis.sh         # Main analysis script
└── README_SONAR.md          # This documentation

sonar-project.properties      # SonarQube project configuration
.github/workflows/
└── sonarqube.yml            # Automatic CI/CD pipeline
```

## Interpreting Results

### SonarQube Dashboard

Access: `http://your-server:9000/dashboard?id=stm32-firmware-template`

### Important Metrics for Firmware

1. **Memory Usage**: Check if Flash/RAM are within limits
2. **Cyclomatic Complexity**: Functions with CCN > 15 need refactoring
3. **Code Coverage**: Target > 70% for critical code
4. **Security Hotspots**: Review usage of unsafe functions
5. **Code Smells**: Maintainability improvements

### Quality Gates

The project passes if:
- Reliability Rating ≤ A
- Security Rating ≤ A
- Maintainability Rating ≤ A
- Coverage > 70%
- Duplicated Lines < 5%
- Complexity per function < 15

## Troubleshooting

### Error: "sonar-scanner not found"
```bash
# Reinstall tools
./scripts/install_sonar_tools.sh

# Or add manually to PATH
export PATH="$HOME/tools/sonar-scanner-4.8.0.2856-linux/bin:$PATH"
source ~/.bashrc
```

### Error: "Connection refused to SonarQube"
```bash
# Check if server is running
curl http://your-server:9000/api/system/status

# Check configuration in script
grep SONAR_SERVER scripts/sonar_analysis.sh
```

### Error: "Authentication required"
```bash
# Check token
echo $SONAR_TOKEN

# Generate new token in SonarQube if necessary
```

### Build Wrapper Error
```bash
# Check if build wrapper is in PATH
which build-wrapper-linux-x86-64

# Run without build wrapper (limited analysis)
BUILD_WRAPPER_AVAILABLE=false ./scripts/sonar_analysis.sh
```

## CI/CD Integration

The `.github/workflows/sonarqube.yml` workflow automatically runs analysis on:
- **Push** to main/develop branches
- **Pull Requests** to main

### Configure GitHub Secrets

```bash
# Settings > Secrets and Variables > Actions
SONAR_TOKEN=your_sonarqube_token
SONAR_HOST_URL=http://your-server:9000
```

## Recursos Adicionais

- [SonarQube Documentation](https://docs.sonarqube.org/)
- [C/C++ Analysis](https://docs.sonarqube.org/latest/analysis/languages/cfamily/)
- [Quality Gates](https://docs.sonarqube.org/latest/user-guide/quality-gates/)
- [Build Wrapper](https://docs.sonarqube.org/latest/analysis/languages/cfamily/#build-wrapper)
