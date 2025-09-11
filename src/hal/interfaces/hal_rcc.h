#pragma once

/**
 * @file hal_rcc.h
 * @brief Portable RCC (Reset and Clock Control) Hardware Abstraction Layer Interface
 * 
 * This interface provides a portable API for clock management across
 * different STM32 microcontroller families.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Clock Source Types
 */
typedef enum {
    HAL_RCC_CLOCK_HSI,      // High Speed Internal oscillator
    HAL_RCC_CLOCK_HSE,      // High Speed External oscillator
    HAL_RCC_CLOCK_LSI,      // Low Speed Internal oscillator
    HAL_RCC_CLOCK_LSE,      // Low Speed External oscillator
    HAL_RCC_CLOCK_PLL,      // Phase-Locked Loop
    HAL_RCC_CLOCK_MSI       // Multi-Speed Internal (available on some STM32 families)
} hal_rcc_clock_source_t;

/**
 * @brief Peripheral Clock Domains
 */
typedef enum {
    HAL_RCC_BUS_AHB1,       // Advanced High-performance Bus 1
    HAL_RCC_BUS_AHB2,       // Advanced High-performance Bus 2
    HAL_RCC_BUS_AHB3,       // Advanced High-performance Bus 3
    HAL_RCC_BUS_APB1,       // Advanced Peripheral Bus 1
    HAL_RCC_BUS_APB2        // Advanced Peripheral Bus 2
} hal_rcc_bus_t;

/**
 * @brief Peripheral Identifiers (extensible per MCU family)
 */
typedef enum {
    // GPIO Peripherals
    HAL_RCC_PERIPH_GPIOA,
    HAL_RCC_PERIPH_GPIOB,
    HAL_RCC_PERIPH_GPIOC,
    HAL_RCC_PERIPH_GPIOD,
    HAL_RCC_PERIPH_GPIOE,
    HAL_RCC_PERIPH_GPIOF,
    HAL_RCC_PERIPH_GPIOG,
    HAL_RCC_PERIPH_GPIOH,
    HAL_RCC_PERIPH_GPIOI,
    
    // UART/USART Peripherals
    HAL_RCC_PERIPH_USART1,
    HAL_RCC_PERIPH_USART2,
    HAL_RCC_PERIPH_USART3,
    HAL_RCC_PERIPH_UART4,
    HAL_RCC_PERIPH_UART5,
    
    // SPI Peripherals
    HAL_RCC_PERIPH_SPI1,
    HAL_RCC_PERIPH_SPI2,
    HAL_RCC_PERIPH_SPI3,
    
    // I2C Peripherals
    HAL_RCC_PERIPH_I2C1,
    HAL_RCC_PERIPH_I2C2,
    HAL_RCC_PERIPH_I2C3,
    
    // Timer Peripherals
    HAL_RCC_PERIPH_TIM1,
    HAL_RCC_PERIPH_TIM2,
    HAL_RCC_PERIPH_TIM3,
    HAL_RCC_PERIPH_TIM4,
    HAL_RCC_PERIPH_TIM5,
    HAL_RCC_PERIPH_TIM6,
    HAL_RCC_PERIPH_TIM7,
    HAL_RCC_PERIPH_TIM8,
    
    // ADC Peripherals
    HAL_RCC_PERIPH_ADC1,
    HAL_RCC_PERIPH_ADC2,
    HAL_RCC_PERIPH_ADC3,
    
    // DMA Peripherals
    HAL_RCC_PERIPH_DMA1,
    HAL_RCC_PERIPH_DMA2,
    
    HAL_RCC_PERIPH_COUNT    // Total count of peripherals
} hal_rcc_periph_t;

/**
 * @brief Clock Configuration Structure
 */
typedef struct {
    hal_rcc_clock_source_t source;      // Clock source
    uint32_t frequency_hz;              // Desired frequency in Hz
    bool enabled;                       // Enable/disable state
} hal_rcc_clock_config_t;

/**
 * @brief System Clock Configuration Structure
 */
typedef struct {
    hal_rcc_clock_config_t hsi;         // HSI configuration
    hal_rcc_clock_config_t hse;         // HSE configuration
    hal_rcc_clock_config_t pll;         // PLL configuration
    hal_rcc_clock_config_t lsi;         // LSI configuration
    hal_rcc_clock_config_t lse;         // LSE configuration
    uint32_t system_clock_hz;           // System clock frequency
    uint32_t hclk_hz;                   // AHB clock frequency
    uint32_t pclk1_hz;                  // APB1 clock frequency
    uint32_t pclk2_hz;                  // APB2 clock frequency
} hal_rcc_system_config_t;

// =============================================================================
// Portable RCC API Functions
// =============================================================================

/**
 * @brief Initialize the RCC system
 * @return 0 on success, negative error code on failure
 */
int hal_rcc_init(void);

/**
 * @brief Configure system clocks
 * @param config System clock configuration
 * @return 0 on success, negative error code on failure
 */
int hal_rcc_configure_system_clock(const hal_rcc_system_config_t* config);

/**
 * @brief Enable peripheral clock
 * @param peripheral Peripheral identifier
 * @return 0 on success, negative error code on failure
 */
int hal_rcc_enable_peripheral_clock(hal_rcc_periph_t peripheral);

/**
 * @brief Disable peripheral clock
 * @param peripheral Peripheral identifier
 * @return 0 on success, negative error code on failure
 */
int hal_rcc_disable_peripheral_clock(hal_rcc_periph_t peripheral);

/**
 * @brief Check if peripheral clock is enabled
 * @param peripheral Peripheral identifier
 * @return true if enabled, false if disabled
 */
bool hal_rcc_is_peripheral_clock_enabled(hal_rcc_periph_t peripheral);

/**
 * @brief Get system clock frequency
 * @return System clock frequency in Hz
 */
uint32_t hal_rcc_get_system_clock_freq(void);

/**
 * @brief Get AHB clock frequency
 * @return AHB clock frequency in Hz
 */
uint32_t hal_rcc_get_hclk_freq(void);

/**
 * @brief Get APB1 clock frequency
 * @return APB1 clock frequency in Hz
 */
uint32_t hal_rcc_get_pclk1_freq(void);

/**
 * @brief Get APB2 clock frequency
 * @return APB2 clock frequency in Hz
 */
uint32_t hal_rcc_get_pclk2_freq(void);

/**
 * @brief Reset peripheral
 * @param peripheral Peripheral identifier
 * @return 0 on success, negative error code on failure
 */
int hal_rcc_reset_peripheral(hal_rcc_periph_t peripheral);

/**
 * @brief Get peripheral clock frequency
 * @param peripheral Peripheral identifier
 * @return Peripheral clock frequency in Hz, 0 if disabled or invalid
 */
uint32_t hal_rcc_get_peripheral_clock_freq(hal_rcc_periph_t peripheral);

#ifdef __cplusplus
}
#endif
