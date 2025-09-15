# STM32 Portable Firmware Architecture Guide

## Overview

This guide explains how to use the portable firmware architecture to create applications that work across different STM32 microcontrollers and development boards.

## Architecture Layers

```
Application Layer (Portable)
    ↓
Hardware Abstraction Layer (HAL) - Portable Interfaces
    ↓
Board Support Package (BSP) - Board/MCU Specific
    ↓
STM32 HAL Drivers - Vendor Specific
    ↓
Hardware
```

## Key Components

### 1. Hardware Abstraction Layer (HAL)

Located in `src/hal/interfaces/`, these provide portable APIs for hardware peripherals:

- **GPIO**: `hal_gpio.h` - Portable GPIO operations
- **RCC**: `hal_rcc.h` - Portable clock and peripheral management
- **UART**: `hal_uart.h` - Portable serial communication
- **SPI**: `hal_spi.h` - Portable SPI communication
- **I2C**: `hal_i2c.h` - Portable I2C communication

### 2. Board Support Package (BSP)

Located in `src/bsp/`, provides board-specific configurations:

- **Common Interface**: `src/bsp/common/bsp.h` - Standard BSP API
- **STM32L4**: `src/bsp/stm32l4xx/` - STM32L4 family implementations
- **STM32F4**: `src/bsp/stm32f4xx/` - STM32F4 family implementations

### 3. Target Configuration

Located in `cmake/targets/`, provides build-time configuration:

- **STM32L432KC**: `cmake/targets/stm32l432kc.cmake`
- **STM32F407VG**: `cmake/targets/stm32f407vg.cmake`

## Writing Portable Applications

### Step 1: Use BSP for Hardware Discovery

```cpp
// Find LED pin configuration from BSP
const bsp_pin_config_t* led_pin = bsp_get_pin_config_by_function(BSP_PIN_FUNCTION_LED, 0);
if (!led_pin) {
    // No LED available on this board
    return -1;
}
```

### Step 2: Use HAL for Hardware Operations

```cpp
// Get GPIO port handle (portable)
hal_gpio_port_t* gpio_port = hal_gpio_get_port(led_pin->port_name);

// Configure GPIO pin (portable)
hal_gpio_init_t gpio_config = {0};
gpio_config.pin = (1U << led_pin->pin);
gpio_config.mode = HAL_GPIO_MODE_OUTPUT_PP;
gpio_config.pull = HAL_GPIO_NOPULL;
gpio_config.speed = HAL_GPIO_SPEED_LOW;

hal_gpio_init(gpio_port, &gpio_config);
```

### Step 3: Handle Board-Specific Logic

```cpp
// Handle active high/low LED logic automatically
hal_gpio_pin_state_t led_on_state = led_pin->active_high ? 
                                    HAL_GPIO_PIN_SET : HAL_GPIO_PIN_RESET;

hal_gpio_write_pin(gpio_port, (1U << led_pin->pin), led_on_state);
```

## Adding Support for New Boards

### 1. Create BSP Implementation

Create a new BSP file for your board:

```cpp
// src/bsp/stm32f4xx/bsp_my_custom_board.cpp

static const bsp_config_t bsp_config_custom = {
    .mcu_family = BSP_MCU_FAMILY_STM32F4XX,
    .flash_size_kb = 512,
    .ram_size_kb = 128,
    .max_clock_mhz = 180,
    .has_fpu = true,
    .has_dsp = true,
    .voltage_range = BSP_VOLTAGE_1V8_TO_3V6
};

static const bsp_pin_config_t pin_configs_custom[] = {
    {
        .port_name = "GPIOC",
        .pin = 13,
        .function = BSP_PIN_FUNCTION_LED,
        .function_index = 0,
        .active_high = false,  // Active low LED
        .description = "User LED"
    },
    // Add more pins...
};

// Implement BSP interface functions...
```

### 2. Create Target Configuration

Create a new CMake target file:

```cmake
# cmake/targets/my_custom_board.cmake

set(MCU_FAMILY "STM32F4xx")
set(MCU_DEVICE "STM32F407xx")

set(MCU_DEFINES
    -DSTM32F407xx
    -DUSE_HAL_DRIVER
)

set(HAL_MODULES
    "GPIO"
    "RCC" 
    "CORTEX"
    "UART"
    "SPI"
    "I2C"
)

set(TARGET_COMPILE_OPTIONS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Memory layout
set(FLASH_SIZE "512K")
set(RAM_SIZE "128K")
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F407VGTX_FLASH.ld")

# BSP source file
set(BSP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/stm32f4xx/bsp_my_custom_board.cpp")
```

### 3. Build for New Target

```bash
# Configure for your custom board
cmake -B build-custom -DTARGET_BOARD=my_custom_board

# Build firmware
cmake --build build-custom
```

## Building for Different Targets

### STM32L432KC (Nucleo-L432KC)
```bash
cmake -B build-l432kc -DTARGET_BOARD=stm32l432kc
cmake --build build-l432kc
```

### STM32F407VG (STM32F4-Discovery)
```bash
cmake -B build-f407vg -DTARGET_BOARD=stm32f407vg
cmake --build build-f407vg
```

## Application Example

See `src/app/portable_led_example.cpp` for a complete example of a portable application that:

1. Uses BSP to discover hardware capabilities
2. Uses HAL for portable hardware operations
3. Handles board-specific differences automatically
4. Works on any supported board without code changes

## Key Benefits

### 1. Code Reusability
- Write application code once
- Deploy to multiple boards
- Reduce development time

### 2. Hardware Abstraction
- No direct register access in application code
- Consistent API across MCU families
- Easy hardware debugging and testing

### 3. Maintainability
- Clear separation of concerns
- Modular architecture
- Easy to add new boards/MCUs

### 4. Testing
- Mock BSP for unit testing
- Test application logic independently
- Comprehensive test coverage

## Best Practices

### 1. Always Use BSP for Hardware Discovery
```cpp
// ❌ Don't hardcode pins
hal_gpio_write_pin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

// ✅ Use BSP to find pins
const bsp_pin_config_t* led = bsp_get_pin_config_by_function(BSP_PIN_FUNCTION_LED, 0);
hal_gpio_write_pin(hal_gpio_get_port(led->port_name), (1U << led->pin), GPIO_PIN_SET);
```

### 2. Handle Optional Features Gracefully
```cpp
// Check if feature is available
const bsp_pin_config_t* button = bsp_get_pin_config_by_function(BSP_PIN_FUNCTION_BUTTON, 0);
if (button) {
    // Button is available - configure it
    configure_button(button);
} else {
    // No button - use alternative UI method
    use_uart_commands();
}
```

### 3. Use HAL Return Codes
```cpp
// Always check return values
if (hal_gpio_init(port, &config) != 0) {
    // Handle initialization error
    return -1;
}
```

### 4. Keep Application Logic Portable
```cpp
// ❌ Don't use MCU-specific features in application
if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
    // MCU-specific code
}

// ✅ Use portable abstractions
if (is_button_pressed(0)) {
    // Portable application logic
}
```

## Extending the Architecture

### Adding New Peripheral Types

1. Define HAL interface in `src/hal/interfaces/`
2. Implement in MCU-specific HAL implementations
3. Add BSP pin function types for new peripheral
4. Update BSP implementations with pin configurations

### Adding New MCU Families

1. Create new BSP directory: `src/bsp/stm32xxxx/`
2. Implement BSP interface for new family
3. Create target configuration: `cmake/targets/`
4. Add linker scripts and startup files
5. Test with example applications

This architecture provides a solid foundation for portable STM32 firmware development while maintaining flexibility and performance.
