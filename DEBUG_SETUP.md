# STM32L432KC Firmware Debugging in VS Code

## Complete Debug Configuration

To debug the STM32L432KC firmware on the microcontroller, you need:

### 1. **Required Hardware**
- **STM32L432KC Nucleo Board** (which you already have)
- **ST-Link V2.1** (integrated in the Nucleo)
- **USB Cable** (to connect Nucleo to PC)

### 2. **Software and VS Code Extensions**

#### A. Install VS Code Extensions:
```bash
# In VS Code, install the following extensions:
# 1. Cortex-Debug (marus25.cortex-debug)
# 2. C/C++ Extension Pack (ms-vscode.cpptools-extension-pack)
```

#### B. Install OpenOCD (Debug Server):
```bash
# Ubuntu/Debian:
sudo apt update
sudo apt install openocd

# Fedora/RHEL:
sudo dnf install openocd

# Arch Linux:
sudo pacman -S openocd
```

#### C. Verify ST-Link:
```bash
# Connect the Nucleo and verify it's detected:
lsusb | grep -i "st.*link"
# Should show something like: "STMicroelectronics ST-LINK/V2-1"
```

### 3. **Implemented Debug Configurations**

#### A. Launch Configuration (`.vscode/launch.json`):
Two debug options have been configured:

1. **OpenOCD + ST-Link** (Recommended for Nucleo)
2. **J-Link** (If you have an external J-Link)

#### B. Build Tasks (`.vscode/tasks.json`):
- Automatic build before debugging
- Compilation with debug symbols enabled

### 4. **How to Use Debug**

#### Step by Step:

1. **Connect Hardware**:
   ```
   STM32L432KC Nucleo <-> USB <-> PC
   ```

2. **Verify Compilation**:
   ```bash
   # Compile firmware with debug info
   cmake --build build --config Debug
   ```

3. **Start Debug in VS Code**:
   - Open VS Code
   - Go to "Run and Debug" tab (Ctrl+Shift+D)
   - Select "Debug STM32L432KC (OpenOCD + ST-Link)"
   - Press F5 or click "Start Debugging"

#### During Debug:
- **Breakpoints**: Click on left margin of code
- **Step Over**: F10
- **Step Into**: F11
- **Continue**: F5
- **Stop**: Shift+F5

### 5. **OpenOCD Configuration**

#### Configuration Files Used:
```bash
# Interface (integrated ST-Link)
/usr/share/openocd/scripts/interface/stlink.cfg

# Target (STM32L4 family)
/usr/share/openocd/scripts/target/stm32l4x.cfg
```

#### Automatic OpenOCD Commands:
```gdb
monitor reset init    # Reset and initialize target
monitor halt          # Stop execution
load                  # Load firmware
continue              # Continue execution
```

### 6. **Manual Debug Commands**

#### Using OpenOCD + GDB directly:

**Terminal 1 (OpenOCD Server)**:
```bash
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg
```

**Terminal 2 (GDB Client)**:
```bash
# Start GDB
/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb build/stm32l432_firmware

# Inside GDB:
(gdb) target extended-remote localhost:3333
(gdb) monitor reset init
(gdb) monitor halt
(gdb) load
(gdb) break main
(gdb) continue
```

### 7. **Available Debugging Features**

#### A. Processor Registers:
- **Cortex-M4 Core Registers**: R0-R15, PSR, etc.
- **FPU Registers**: S0-S31 (floating point)

#### B. STM32 Peripherals:
- **GPIO Registers**: GPIOx->ODR, IDR, MODER, etc.
- **Clock Registers**: RCC->CR, CFGR, etc.
- **Timer Registers**: TIMx->CNT, CCR, etc.

#### C. Memory Viewer:
- **Flash Memory**: 0x08000000 - 0x0803FFFF (256KB)
- **SRAM1**: 0x20000000 - 0x2000BFFF (48KB)
- **SRAM2**: 0x10000000 - 0x10003FFF (16KB)

### 8. **Troubleshooting**

#### Common Problems:

**A. ST-Link not detected**:
```bash
# Check USB permissions
sudo usermod -a -G dialout $USER
# Logout and login again
```

**B. OpenOCD fails to connect**:
```bash
# Check if target is connected
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg -c "init; targets; exit"
```

**C. Debug doesn't stop at breakpoints**:
- Check if firmware was compiled with `-g`
- Check if optimization is at `-O0` or `-Og`

### 9. **CMake Configuration for Debug**

#### Compilation Flags (already configured):
```cmake
# Debug flags enabled:
-g                    # Generate debug info
-gdwarf-2            # DWARF-2 debug format
-O0                  # No optimization
```

### 10. **Useful Scripts**

#### Flash Script (optional):
```bash
#!/bin/bash
# flash_firmware.sh
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg \
    -c "program build/stm32l432_firmware verify reset exit"
```

#### Quick Debug Script:
```bash
#!/bin/bash
# quick_debug.sh
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg &
sleep 2
/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb \
    -ex "target extended-remote localhost:3333" \
    -ex "monitor reset init" \
    -ex "load" \
    build/stm32l432_firmware
```

---

## Next Steps

1. **Install Cortex-Debug extension in VS Code**
2. **Install OpenOCD** on the system
3. **Connect STM32L432KC** via USB
4. **Test debug** with F5 in VS Code

Debug will allow you to:
- Stepping line by line
- Breakpoints in any function
- Watch variables in real time
- Memory inspection of registers and RAM
- Call stack analysis
- FreeRTOS task debugging
