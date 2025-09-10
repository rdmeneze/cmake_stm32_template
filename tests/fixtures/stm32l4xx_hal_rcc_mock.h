#pragma once

// Mock implementation for RCC macros used in tests
// This file should be included in test environments to override STM32 RCC macros

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations for RCC clock enable functions
void __HAL_RCC_GPIOB_CLK_ENABLE_IMPL(void);
void __HAL_RCC_GPIOA_CLK_ENABLE_IMPL(void);
void __HAL_RCC_GPIOC_CLK_ENABLE_IMPL(void);

#ifdef __cplusplus
}
#endif

// Redefine the HAL macros to call our mock functions instead of accessing hardware
#undef __HAL_RCC_GPIOB_CLK_ENABLE
#undef __HAL_RCC_GPIOA_CLK_ENABLE
#undef __HAL_RCC_GPIOC_CLK_ENABLE

#define __HAL_RCC_GPIOB_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE_IMPL()
#define __HAL_RCC_GPIOA_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE_IMPL()
#define __HAL_RCC_GPIOC_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE_IMPL()
