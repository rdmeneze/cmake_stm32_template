#!/bin/bash

# Script para debug manual do STM32L432KC
# Usage: ./debug_firmware.sh

set -e

echo "🐛 STM32L432KC Debug Script"
echo "=========================="

# Verificar se o firmware existe
FIRMWARE_PATH="build/stm32l432_firmware"
if [ ! -f "$FIRMWARE_PATH" ]; then
    echo "❌ Firmware não encontrado em: $FIRMWARE_PATH"
    echo "   Execute primeiro: cmake --build build --config Debug"
    exit 1
fi

echo "📁 Firmware: $FIRMWARE_PATH"

# Verificar GDB ARM
GDB_PATH="/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb"
if [ ! -f "$GDB_PATH" ]; then
    echo "❌ GDB ARM não encontrado em: $GDB_PATH"
    echo "   Verifique a instalação do ARM toolchain"
    exit 1
fi

echo "🔧 GDB ARM: $GDB_PATH"

# Verificar se OpenOCD está disponível
if ! command -v openocd &> /dev/null; then
    echo "❌ OpenOCD não está instalado!"
    exit 1
fi

echo "🔗 Iniciando OpenOCD server..."

# Iniciar OpenOCD em background
openocd -f interface/stlink.cfg -f target/stm32l4x.cfg &
OPENOCD_PID=$!

# Aguardar OpenOCD inicializar
sleep 3

echo "🐛 Iniciando GDB debug session..."
echo ""
echo "📖 Comandos úteis no GDB:"
echo "   (gdb) break main          # Breakpoint na função main"
echo "   (gdb) continue           # Continuar execução"
echo "   (gdb) step               # Step into"
echo "   (gdb) next               # Step over"
echo "   (gdb) info registers     # Ver registradores"
echo "   (gdb) x/16x 0x20000000   # Ver memória RAM"
echo "   (gdb) quit               # Sair"
echo ""

# Função de cleanup
cleanup() {
    echo "🛑 Finalizando OpenOCD..."
    kill $OPENOCD_PID 2>/dev/null || true
    exit
}

# Trap para cleanup ao sair
trap cleanup EXIT INT TERM

# Iniciar GDB com comandos automáticos
$GDB_PATH \
    -ex "target extended-remote localhost:3333" \
    -ex "monitor reset init" \
    -ex "monitor halt" \
    -ex "load" \
    -ex "break main" \
    -ex "continue" \
    $FIRMWARE_PATH
