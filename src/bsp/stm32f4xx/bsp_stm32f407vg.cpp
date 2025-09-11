/**
 * @file bsp_stm32f407vg.cpp
 * @brief BSP implementation for STM32F407VG (STM32F4-Discovery board)
 */

extern "C" {
#include "bsp.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "stm32f4xx_hal.h"
}

#include <cstring>

// STM32F407VG specific configuration
static const bsp_config_t bsp_config_f407vg = {
    .mcu_family = BSP_MCU_FAMILY_STM32F4XX,
    .flash_size_kb = 1024,  // 1MB Flash
    .ram_size_kb = 192,     // 192KB SRAM
    .max_clock_mhz = 168,   // 168MHz max
    .has_fpu = true,
    .has_dsp = true,
    .voltage_range = BSP_VOLTAGE_1V8_TO_3V6
};

// Pin configuration for STM32F4-Discovery board
static const bsp_pin_config_t pin_configs_f407vg[] = {
    // User LEDs (4 LEDs on Discovery board)
    {
        .port_name = "GPIOD",
        .pin = 12,
        .function = BSP_PIN_FUNCTION_LED,
        .function_index = 0,  // Green LED (LD4)
        .active_high = true,
        .description = "User LED Green"
    },
    {
        .port_name = "GPIOD",
        .pin = 13,
        .function = BSP_PIN_FUNCTION_LED,
        .function_index = 1,  // Orange LED (LD3)
        .active_high = true,
        .description = "User LED Orange"
    },
    {
        .port_name = "GPIOD",
        .pin = 14,
        .function = BSP_PIN_FUNCTION_LED,
        .function_index = 2,  // Red LED (LD5)
        .active_high = true,
        .description = "User LED Red"
    },
    {
        .port_name = "GPIOD",
        .pin = 15,
        .function = BSP_PIN_FUNCTION_LED,
        .function_index = 3,  // Blue LED (LD6)
        .active_high = true,
        .description = "User LED Blue"
    },
    
    // User Button
    {
        .port_name = "GPIOA",
        .pin = 0,
        .function = BSP_PIN_FUNCTION_BUTTON,
        .function_index = 0,
        .active_high = true,
        .description = "User Button B1"
    },
    
    // USART2 (ST-LINK Virtual COM Port)
    {
        .port_name = "GPIOA",
        .pin = 2,
        .function = BSP_PIN_FUNCTION_UART_TX,
        .function_index = 0,  // USART2
        .active_high = true,
        .description = "USART2 TX"
    },
    {
        .port_name = "GPIOA",
        .pin = 3,
        .function = BSP_PIN_FUNCTION_UART_RX,
        .function_index = 0,  // USART2
        .active_high = true,
        .description = "USART2 RX"
    },
    
    // SPI1 pins
    {
        .port_name = "GPIOA",
        .pin = 5,
        .function = BSP_PIN_FUNCTION_SPI_SCK,
        .function_index = 0,  // SPI1
        .active_high = true,
        .description = "SPI1 SCK"
    },
    {
        .port_name = "GPIOA",
        .pin = 6,
        .function = BSP_PIN_FUNCTION_SPI_MISO,
        .function_index = 0,  // SPI1
        .active_high = true,
        .description = "SPI1 MISO"
    },
    {
        .port_name = "GPIOA",
        .pin = 7,
        .function = BSP_PIN_FUNCTION_SPI_MOSI,
        .function_index = 0,  // SPI1
        .active_high = true,
        .description = "SPI1 MOSI"
    },
    
    // I2C1 pins
    {
        .port_name = "GPIOB",
        .pin = 8,
        .function = BSP_PIN_FUNCTION_I2C_SCL,
        .function_index = 0,  // I2C1
        .active_high = true,
        .description = "I2C1 SCL"
    },
    {
        .port_name = "GPIOB",
        .pin = 9,
        .function = BSP_PIN_FUNCTION_I2C_SDA,
        .function_index = 0,  // I2C1
        .active_high = true,
        .description = "I2C1 SDA"
    }
};

// STM32F407VG specific clock configuration
static int configure_system_clock_f407vg(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure the main internal regulator output voltage
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Initialize the RCC Oscillators according to the specified parameters
    // Using HSE 8MHz external crystal
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;   // VCO input = HSE/PLLM = 8MHz/8 = 1MHz
    RCC_OscInitStruct.PLL.PLLN = 336; // VCO output = VCO input * PLLN = 1MHz * 336 = 336MHz
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2; // PLLCLK = VCO output / PLLP = 336MHz/2 = 168MHz
    RCC_OscInitStruct.PLL.PLLQ = 7;   // USB OTG FS clock = VCO output / PLLQ = 336MHz/7 = 48MHz

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return -1;
    }

    // Initialize the CPU, AHB and APB buses clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;   // AHB = 168MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;    // APB1 = 42MHz (max 42MHz)
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;    // APB2 = 84MHz (max 84MHz)

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        return -2;
    }

    return 0;
}

// BSP Interface Implementation

extern "C" const char* bsp_get_board_name(void) {
    return "STM32F4-Discovery";
}

extern "C" const char* bsp_get_mcu_name(void) {
    return "STM32F407VGT6";
}

extern "C" bsp_mcu_family_t bsp_get_mcu_family(void) {
    return BSP_MCU_FAMILY_STM32F4XX;
}

extern "C" const bsp_config_t* bsp_get_config(void) {
    return &bsp_config_f407vg;
}

extern "C" const bsp_pin_config_t* bsp_get_pin_config_by_function(
    bsp_pin_function_t function, 
    uint8_t index
) {
    for (size_t i = 0; i < sizeof(pin_configs_f407vg) / sizeof(pin_configs_f407vg[0]); i++) {
        if (pin_configs_f407vg[i].function == function && 
            pin_configs_f407vg[i].function_index == index) {
            return &pin_configs_f407vg[i];
        }
    }
    return nullptr;
}

extern "C" const bsp_pin_config_t* bsp_get_pin_configs(size_t* count) {
    if (count) {
        *count = sizeof(pin_configs_f407vg) / sizeof(pin_configs_f407vg[0]);
    }
    return pin_configs_f407vg;
}

extern "C" int bsp_init(void) {
    // Initialize HAL Library
    if (HAL_Init() != HAL_OK) {
        return -1;
    }

    // Configure system clock
    if (configure_system_clock_f407vg() != 0) {
        return -2;
    }

    // Initialize systick for FreeRTOS (if using FreeRTOS)
    HAL_InitTick(TICK_INT_PRIORITY);

    return 0;
}

extern "C" void bsp_deinit(void) {
    HAL_DeInit();
}

extern "C" uint32_t bsp_get_system_clock_mhz(void) {
    return HAL_RCC_GetSysClockFreq() / 1000000U;
}

extern "C" void bsp_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}
