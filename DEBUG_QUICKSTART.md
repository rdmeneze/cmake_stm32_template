# 🐛 Debug do STM32L432KC - Guia Prático

## ✅ Status da Configuração

- ✅ **Firmware compilado** com símbolos de debug
- ✅ **OpenOCD instalado** e funcionando 
- ✅ **Scripts automáticos** criados
- ✅ **VS Code configurado** para debug
- ✅ **GDB ARM toolchain** disponível

## 🚀 3 Formas de Fazer Debug

### 1. **Debug no VS Code (Recomendado)**

**Pré-requisitos:**
```bash
# Instalar extensão Cortex-Debug no VS Code
# Extensões recomendadas estão em .vscode/extensions.json
```

**Como usar:**
1. Conectar STM32L432KC via USB
2. Abrir VS Code na pasta do projeto
3. Ir para "Run and Debug" (Ctrl+Shift+D)
4. Selecionar configuração de debug
5. Pressionar F5

### 2. **Flash + Debug via Scripts**

**Flash do firmware:**
```bash
./flash_firmware.sh
```

**Debug manual:**
```bash
./debug_firmware.sh
```

### 3. **Debug Manual Completo**

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

## 🔧 Comandos Úteis no GDB

### Controle de Execução:
```gdb
break main              # Breakpoint na main
break led_task          # Breakpoint na task do LED
continue               # Continuar execução  
step                   # Step into (entra em funções)
next                   # Step over (pula funções)
finish                 # Sair da função atual
```

### Inspeção de Variáveis:
```gdb
print variable_name    # Ver valor de variável
print *pointer         # Ver valor apontado por ponteiro
print/x variable       # Ver em hexadecimal
watch variable         # Parar quando variável mudar
info locals           # Ver todas variáveis locais
```

### Registradores e Memória:
```gdb
info registers         # Ver todos registradores
print $r0              # Ver registrador R0
print $pc              # Ver Program Counter
x/16x 0x20000000      # Ver 16 words da RAM
x/i $pc               # Ver instrução atual
```

### Específico para STM32:
```gdb
# GPIO B registradores (LED está no PB3)
x/8x 0x48000400       # GPIOB base address
print *(int*)0x48000414  # GPIOB->ODR (output data)

# System Control Block
x/16x 0xE000ED00      # SCB base address

# SysTick
x/4x 0xE000E010       # SysTick registers
```

### FreeRTOS Debug:
```gdb
# Ver task atual
print (char*)pxCurrentTCB->pcTaskName

# Ver todas as tasks
info threads

# Trocar de thread/task
thread 2
```

## 📁 Arquivos de Debug Criados

### Scripts:
- `flash_firmware.sh` - Flash automático via ST-Link
- `debug_firmware.sh` - Debug GDB automático

### VS Code:
- `.vscode/launch.json` - Configurações de debug
- `.vscode/tasks.json` - Tarefas de build e debug
- `.vscode/extensions.json` - Extensões recomendadas
- `.vscode/settings.json` - Configurações do projeto

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
