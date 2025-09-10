# STM32L432KC Debug - Practical Guide

## Configuration Status

- **Compiled firmware** with debug symbols
- **OpenOCD installed** and working
- **Created automatic scripts**
- **Configured VS Code** for debug
- **GDB ARM toolchain** available

## 3 Ways to Debug

### 1. **VS Code Debugging (Recommended)**

**Prerequisites:**
```bash
# Install Cortex-Debug extension on VS Code
# Recommended extensions are in .vscode/extensions.json
```

**How to use:**
1. Connect STM32L432KC via USB
2. Open project on VS Code
3. "Run and Debug" (Ctrl+Shift+D)
4. Select debug configuration
5. Press F5

### 2. **Flash + Debug via Scripts**

**Firmware flash:**
```bash
./flash_firmware.sh
```

**Manual debug:**
```bash
./debug_firmware.sh
```

### 3. **Complete Manual Debug**

**Terminal 1 (OpenOCD Server):**
```bash
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg
```

**Terminal 2 (GDB Debug):**
```bash
/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb build/stm32l432_firmware

# In GDB:
(gdb) target extended-remote localhost:3333
(gdb) monitor reset init
(gdb) load
(gdb) break main
(gdb) continue
```

## Useful GDB Commands

### Execution Control:
```gdb
break main              # Breakpoint on main
break led_task          # Breakpoint on LED task
continue               # Continue execution
step                   # Step into
next                   # Step over
finish                 # Step out
```

### Variable Inspection:
```gdb
print variable_name    # See variable value
print *pointer         # See value pointed by
print/x variable       # See in hexadecimal
watch variable         # Stop when variable changes value
info locals           # See all local variables
```

### Registers and Memory:
```gdb
info registers         # Inspect all registers
print $r0              # Inspect register R0
print $pc              # Inspect Program Counter
x/16x 0x20000000      # Inspect 16 words in RAM
x/i $pc               # Inspect current instruction
```

### STM32 Specific:
```gdb
# GPIO B registers (LED is on PB3)
x/8x 0x48000400       # GPIOB base address
print *(int*)0x48000414  # GPIOB->ODR (output data)

# System Control Block
x/16x 0xE000ED00      # SCB base address

# SysTick
x/4x 0xE000E010       # SysTick registers
```

### FreeRTOS Debug:
```gdb
# Verify current task
print (char*)pxCurrentTCB->pcTaskName

# Verify all tasks
info threads

# Thread/task switch
thread 2
```

## Created Debug Files

### Scripts:
- `flash_firmware.sh` - Automatic flash via ST-Link
- `debug_firmware.sh` - Automatic GDB debug

### VS Code:
- `.vscode/launch.json` - Debug configuration
- `.vscode/tasks.json` - Build and debug tasks
- `.vscode/extensions.json` - Recommended extensions
- `.vscode/settings.json` - Project configurations

## Available Debug Functionalities

### Breakpoints
- Line by line
- Conditional
- Specific functions
- Memory address

### Stepping
- Step Into (F11)
- Step Over (F10)
- Step Out (Shift+F11)

## FreeRTOS Debug

### Task Debug:
```gdb
# Current task
print (char*)pxCurrentTCB->pcTaskName

# Task stack usage
print pxCurrentTCB->pxTopOfStack
print pxCurrentTCB->pxStack

# Task state
info threads
```

### Semaphores and Queues:
```gdb
# If you have a queue called xQueue
print *xQueue
```

## Performance Tips

### 1. **Optimize Debug Build**
In CMakeLists.txt, use `-Og` instead of `-O0`:
```cmake
set(CMAKE_C_FLAGS_DEBUG "-g -Og") # Optimized debug
```

### 2. **Efficient Breakpoints**
- Use conditional breakpoints in loops
- Avoid breakpoints in high frequency ISRs

### 3. **Memory Watchpoints**
```gdb
watch *0x20000100    # When address is changed
rwatch *0x20000100   # When address is read
```

## Troubleshooting

### ST-Link Does Not Connect:
```bash
# Verify USB detection
lsusb | grep -i st

# Verify permissions
sudo usermod -a -G dialout $USER
# Then logout and login again
```

### OpenOCD Failure:
```bash
# Test connection
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg -c "init; halt; exit"
```

### Debug Doesn't Stop at Breakpoints:
- Verify firmware was compiled with `-g` option
- Verify code optimization is not too high
- Use `info sources` in GDB to see files with debug info

---

## Next Steps

1. **Install Cortex-Debug** in VS Code
2. **Connect STM32L432KC** via USB
3. **Test flash** with `./flash_firmware.sh`
4. **Test debug** with F5 in VS Code

The debug will enable you to see exactly how FreeRTOS and its tasks are working in real time!
