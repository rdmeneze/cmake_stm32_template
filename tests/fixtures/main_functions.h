#pragma once

// Function declarations for main functions that we want to test

#ifdef __cplusplus
extern "C" {
#endif

// Initialize GPIO for LED
void GPIO_Init(void);

// System clock configuration
void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif
