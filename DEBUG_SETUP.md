# Debug do Firmware STM32L432KC no VS Code

## 🎯 Configuração Completa de Debug

Para debugar o firmware STM32L432KC no microcontrolador, você precisa de:

### 1. **Hardware Necessário**
- **STM32L432KC Nucleo Board** (que você já tem)
- **ST-Link V2.1** (integrado na Nucleo)
- **Cabo USB** (para conectar a Nucleo ao PC)

### 2. **Software e Extensões VS Code**

#### A. Instalar Extensões no VS Code:
```bash
# No VS Code, instale as seguintes extensões:
# 1. Cortex-Debug (marus25.cortex-debug)
# 2. C/C++ Extension Pack (ms-vscode.cpptools-extension-pack)
```

#### B. Instalar OpenOCD (Debug Server):
```bash
# Ubuntu/Debian:
sudo apt update
sudo apt install openocd

# Fedora/RHEL:
sudo dnf install openocd

# Arch Linux:
sudo pacman -S openocd
```

#### C. Verificar ST-Link:
```bash
# Conecte a Nucleo e verifique se é detectada:
lsusb | grep -i "st.*link"
# Deve mostrar algo como: "STMicroelectronics ST-LINK/V2-1"
```

### 3. **Configurações de Debug Implementadas**

#### A. Launch Configuration (`.vscode/launch.json`):
Duas opções de debug foram configuradas:

1. **OpenOCD + ST-Link** (Recomendado para Nucleo)
2. **J-Link** (Caso tenha um J-Link externo)

#### B. Tarefas de Build (`.vscode/tasks.json`):
- Build automático antes do debug
- Compilação com símbolos de debug habilitados

### 4. **Como Usar o Debug**

#### Passo a Passo:

1. **Conectar Hardware**:
   ```
   STM32L432KC Nucleo ←→ USB ←→ PC
   ```

2. **Verificar Compilação**:
   ```bash
   # Compilar firmware com debug info
   cmake --build build --config Debug
   ```

3. **Iniciar Debug no VS Code**:
   - Abrir VS Code
   - Ir para aba "Run and Debug" (Ctrl+Shift+D)
   - Selecionar "Debug STM32L432KC (OpenOCD + ST-Link)"
   - Pressionar F5 ou clicar "Start Debugging"

#### Durante o Debug:
- **Breakpoints**: Clique na margem esquerda do código
- **Step Over**: F10
- **Step Into**: F11
- **Continue**: F5
- **Stop**: Shift+F5

### 5. **Configuração OpenOCD**

#### Arquivos de Configuração Utilizados:
```bash
# Interface (ST-Link integrado)
/usr/share/openocd/scripts/interface/stlink.cfg

# Target (STM32L4 family)
/usr/share/openocd/scripts/target/stm32l4x.cfg
```

#### Comandos OpenOCD Automáticos:
```gdb
monitor reset init    # Reset e inicializar target
monitor halt          # Parar execução
load                  # Carregar firmware
continue              # Continuar execução
```

### 6. **Comandos Manuais de Debug**

#### Usando OpenOCD + GDB diretamente:

**Terminal 1 (OpenOCD Server)**:
```bash
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg
```

**Terminal 2 (GDB Client)**:
```bash
# Iniciar GDB
/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb build/stm32l432_firmware

# Dentro do GDB:
(gdb) target extended-remote localhost:3333
(gdb) monitor reset init
(gdb) monitor halt
(gdb) load
(gdb) break main
(gdb) continue
```

### 7. **Debugging Features Disponíveis**

#### A. Registradores do Processador:
- **Cortex-M4 Core Registers**: R0-R15, PSR, etc.
- **FPU Registers**: S0-S31 (ponto flutuante)

#### B. Periféricos STM32:
- **GPIO Registers**: GPIOx->ODR, IDR, MODER, etc.
- **Clock Registers**: RCC->CR, CFGR, etc.
- **Timer Registers**: TIMx->CNT, CCR, etc.

#### C. Memory Viewer:
- **Flash Memory**: 0x08000000 - 0x0803FFFF (256KB)
- **SRAM1**: 0x20000000 - 0x2000BFFF (48KB)
- **SRAM2**: 0x10000000 - 0x10003FFF (16KB)

### 8. **Troubleshooting**

#### Problemas Comuns:

**A. ST-Link não detectado**:
```bash
# Verificar permissões USB
sudo usermod -a -G dialout $USER
# Logout e login novamente
```

**B. OpenOCD falha ao conectar**:
```bash
# Verificar se o target está conectado
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg -c "init; targets; exit"
```

**C. Debug não para em breakpoints**:
- Verificar se firmware foi compilado com `-g`
- Verificar se optimization está em `-O0` ou `-Og`

### 9. **Configurações CMake para Debug**

#### Flags de Compilação (já configuradas):
```cmake
# Debug flags habilitados:
-g                    # Generate debug info
-gdwarf-2            # DWARF-2 debug format
-O0                  # No optimization
```

### 10. **Scripts Úteis**

#### Script de Flash (opcional):
```bash
#!/bin/bash
# flash_firmware.sh
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg \
    -c "program build/stm32l432_firmware verify reset exit"
```

#### Script de Debug Rápido:
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

## 🚀 Próximos Passos

1. **Instalar Cortex-Debug extension no VS Code**
2. **Instalar OpenOCD** no sistema
3. **Conectar STM32L432KC** via USB
4. **Testar debug** com F5 no VS Code

O debug te permitirá:
- ✅ **Stepping** linha por linha
- ✅ **Breakpoints** em qualquer função
- ✅ **Watch variables** em tempo real
- ✅ **Memory inspection** de registradores e RAM
- ✅ **Call stack** analysis
- ✅ **FreeRTOS task debugging**
