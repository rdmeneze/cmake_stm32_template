# STM32L432KC debug - practical guide

## Configuration status

- **Compilated firmware** with debug symbols
- **OpenOCD installed** and working
- **Created Automatic scripts**
- **Configurated VS Code** for debug
- **GDB ARM toolchain** available

## 3 ways to debugging

### 1. **VS Code debugging (Recommended)**

**Pre-requirements:**
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

### 3. **Complete manual debug**

**Terminal 1 (OpenOCD Server):**
```bash
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg
```

**Terminal 2 (GDB Debug):**
```bash
/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb build/stm32l432_firmware

# No GDB:
(gdb) target extended-remote localhost:3333
(gdb) monitor reset init
(gdb) load
(gdb) break main
(gdb) continue
```

## Useful GDB commands

### Execution control:
```gdb
break main              # Breakpoint on main
break led_task          # Breakpoint on LED task
continue               # continue execution
step                   # Step into
next                   # Step over
finish
```

### Variable inspection:
```gdb
print variable_name    # see variable value
print *pointer         # see value pointed by
print/x variable       # see in hex
watch variable         # stop when variable changes the value
info locals           # see all local variables
```

### Registers and memory:
```gdb
info registers         # Inspect all registers
print $r0              # Inspect register R0
print $pc              # Inspect Program Counter
x/16x 0x20000000      # Inspect 16 words on RAM
x/i $pc               # Inspect current instruction
```

### Specific for  STM32:
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

# thread/task switch
thread 2
```

## Created debug file

### Scripts:
- `flash_firmware.sh` - Flash automatic via ST-Link
- `debug_firmware.sh` - Debug GDB automatic

### VS Code:
- `.vscode/launch.json` - debug configuration
- `.vscode/tasks.json` - Build and debug tasks
- `.vscode/extensions.json` - recommended extensions
- `.vscode/settings.json` - Project confugurations

## 🎯 Funcionalidades de Debug Disponíveis

### ✅ Breakpoints
- Linha por linha
- Condicionais
- Em funções específicas
- Em endereços de memória

### ✅ Stepping
- Step Into (F11) - Entra em funções
- Step Over (F10) - Pula funções
- Step Out (Shift+F11) - Sai da função

### ✅ Watch Variables
- Variáveis locais e globais
- Ponteiros e arrays
- Estruturas FreeRTOS
- Registradores do processador

### ✅ Memory Inspection
- Flash memory (0x08000000)
- SRAM (0x20000000)
- Registradores de periféricos
- Stack e heap

### ✅ Real-time Features
- Pausar/continuar execução
- Reset do microcontrolador
- Hot-reload de firmware
- Live variable editing

## 🔍 Debug do FreeRTOS

### Tasks Debug:
```gdb
# Ver task atual
print (char*)pxCurrentTCB->pcTaskName

# Ver stack usage de uma task
print pxCurrentTCB->pxTopOfStack
print pxCurrentTCB->pxStack

# Ver estado das tasks
info threads
```

### Semáforos e Queues:
```gdb
# Se você tiver uma queue chamada xQueue
print *xQueue
```

## ⚡ Dicas de Performance

### 1. **Optimize Debug Build**
No CMakeLists.txt, use `-Og` em vez de `-O0`:
```cmake
set(CMAKE_C_FLAGS_DEBUG "-g -Og") # Debug otimizado
```

### 2. **Breakpoints Eficientes**
- Use breakpoints condicionais para loops
- Evite breakpoints em ISRs de alta frequência

### 3. **Memory Watchpoints**
```gdb
watch *0x20000100    # Para quando endereço for modificado
rwatch *0x20000100   # Para quando endereço for lido
```

## 🚨 Troubleshooting

### ST-Link não conecta:
```bash
# Verificar detecção USB
lsusb | grep -i st

# Verificar permissões
sudo usermod -a -G dialout $USER
# Logout e login novamente
```

### OpenOCD falha:
```bash
# Testar conexão
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg -c "init; halt; exit"
```

### Debug não para em breakpoints:
- Verificar se firmware tem símbolos (`-g`)
- Verificar se otimização não está muito alta
- Usar `info sources` no GDB para ver arquivos com debug info

---

## 🎯 Próximos Passos

1. **Instalar Cortex-Debug** no VS Code
2. **Conectar STM32L432KC** via USB
3. **Testar flash** com `./flash_firmware.sh`
4. **Testar debug** com F5 no VS Code

O debug te permitirá ver exatamente como o FreeRTOS e suas tasks estão funcionando em tempo real!
