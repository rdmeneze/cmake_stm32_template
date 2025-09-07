#!/bin/bash

# Script para fazer flash do firmware no STM32L432KC via ST-Link
# Usage: ./flash_firmware.sh

set -e

echo "🔧 STM32L432KC Firmware Flash Script"
echo "===================================="

# Verificar se o firmware existe
FIRMWARE_PATH="build/stm32l432_firmware"
if [ ! -f "$FIRMWARE_PATH" ]; then
    echo "❌ Firmware não encontrado em: $FIRMWARE_PATH"
    echo "   Execute primeiro: cmake --build build --config Debug"
    exit 1
fi

echo "📁 Firmware encontrado: $FIRMWARE_PATH"
echo "💾 Tamanho: $(ls -lh $FIRMWARE_PATH | awk '{print $5}')"

# Verificar se OpenOCD está disponível
if ! command -v openocd &> /dev/null; then
    echo "❌ OpenOCD não está instalado!"
    echo "   Instale com: sudo apt install openocd"
    exit 1
fi

echo "🔗 Conectando ao STM32L432KC via ST-Link..."

# Flash do firmware
openocd \
    -f interface/stlink.cfg \
    -f target/stm32l4x.cfg \
    -c "program $FIRMWARE_PATH verify reset exit"

if [ $? -eq 0 ]; then
    echo "✅ Firmware gravado com sucesso!"
    echo "🚀 O microcontrolador foi resetado e está executando o novo firmware"
else
    echo "❌ Erro durante a gravação do firmware"
    exit 1
fi
