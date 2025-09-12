/**
 * @file hal_gpio_stm32l4xx.c
 * @brief GPIO HAL implementation for STM32L4xx microcontrollers
 */

#include "hal_gpio.h"
#include "stm32l4xx_hal.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Static hal_gpio_port_t instances
static hal_gpio_port_t gpio_port_handles[8] = {
    {.hw_port = (void*)GPIOA, .name = "GPIOA", .base_address = GPIOA_BASE},
    {.hw_port = (void*)GPIOB, .name = "GPIOB", .base_address = GPIOB_BASE},
    {.hw_port = (void*)GPIOC, .name = "GPIOC", .base_address = GPIOC_BASE},
#ifdef GPIOD
    {.hw_port = (void*)GPIOD, .name = "GPIOD", .base_address = GPIOD_BASE},
#else
    {.hw_port = NULL, .name = "GPIOD", .base_address = 0},
#endif
#ifdef GPIOE
    {.hw_port = (void*)GPIOE, .name = "GPIOE", .base_address = GPIOE_BASE},
#else
    {.hw_port = NULL, .name = "GPIOE", .base_address = 0},
#endif
#ifdef GPIOF
    {.hw_port = (void*)GPIOF, .name = "GPIOF", .base_address = GPIOF_BASE},
#else
    {.hw_port = NULL, .name = "GPIOF", .base_address = 0},
#endif
#ifdef GPIOG
    {.hw_port = (void*)GPIOG, .name = "GPIOG", .base_address = GPIOG_BASE},
#else
    {.hw_port = NULL, .name = "GPIOG", .base_address = 0},
#endif
#ifdef GPIOH
    {.hw_port = (void*)GPIOH, .name = "GPIOH", .base_address = GPIOH_BASE}
#else
    {.hw_port = NULL, .name = "GPIOH", .base_address = 0}
#endif
};

// Clock enable functions for each port
static void enable_port_clocks(GPIO_TypeDef* port) {
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
#ifdef GPIOD
    } else if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
#endif
#ifdef GPIOE
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
#endif
#ifdef GPIOF
    } else if (port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
#endif
#ifdef GPIOG
    } else if (port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
#endif
#ifdef GPIOH
    } else if (port == GPIOH) {
        __HAL_RCC_GPIOH_CLK_ENABLE();
#endif
    }
}

// Clock disable functions for each port
static void disable_port_clocks(GPIO_TypeDef* port) {
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_DISABLE();
    } else if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_DISABLE();
    } else if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_DISABLE();
#ifdef GPIOD
    } else if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_DISABLE();
#endif
#ifdef GPIOE
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_DISABLE();
#endif
#ifdef GPIOF
    } else if (port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_DISABLE();
#endif
#ifdef GPIOG
    } else if (port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_DISABLE();
#endif
#ifdef GPIOH
    } else if (port == GPIOH) {
        __HAL_RCC_GPIOH_CLK_DISABLE();
#endif
    }
}

// Convert HAL GPIO mode to STM32 HAL mode
static uint32_t convert_gpio_mode(hal_gpio_mode_t mode) {
    switch (mode) {
        case HAL_GPIO_MODE_INPUT:
            return GPIO_MODE_INPUT;
        case HAL_GPIO_MODE_OUTPUT_PP:
            return GPIO_MODE_OUTPUT_PP;
        case HAL_GPIO_MODE_OUTPUT_OD:
            return GPIO_MODE_OUTPUT_OD;
        case HAL_GPIO_MODE_AF_PP:
            return GPIO_MODE_AF_PP;
        case HAL_GPIO_MODE_AF_OD:
            return GPIO_MODE_AF_OD;
        case HAL_GPIO_MODE_ANALOG:
            return GPIO_MODE_ANALOG;
        default:
            return GPIO_MODE_INPUT;
    }
}

// Convert HAL GPIO pull to STM32 HAL pull
static uint32_t convert_gpio_pull(hal_gpio_pull_t pull) {
    switch (pull) {
        case HAL_GPIO_NOPULL:
            return GPIO_NOPULL;
        case HAL_GPIO_PULLUP:
            return GPIO_PULLUP;
        case HAL_GPIO_PULLDOWN:
            return GPIO_PULLDOWN;
        default:
            return GPIO_NOPULL;
    }
}

// Convert HAL GPIO speed to STM32 HAL speed
static uint32_t convert_gpio_speed(hal_gpio_speed_t speed) {
    switch (speed) {
        case HAL_GPIO_SPEED_LOW:
            return GPIO_SPEED_FREQ_LOW;
        case HAL_GPIO_SPEED_MEDIUM:
            return GPIO_SPEED_FREQ_MEDIUM;
        case HAL_GPIO_SPEED_HIGH:
            return GPIO_SPEED_FREQ_HIGH;
        case HAL_GPIO_SPEED_VERY_HIGH:
            return GPIO_SPEED_FREQ_VERY_HIGH;
        default:
            return GPIO_SPEED_FREQ_LOW;
    }
}

// Public API implementations

hal_gpio_port_t* hal_gpio_get_port(const char* port_name) {
    if (!port_name) {
        return NULL;
    }
    
    for (int i = 0; i < 8; i++) {
        if (strcmp(port_name, gpio_port_handles[i].name) == 0) {
            return &gpio_port_handles[i];
        }
    }
    
    return NULL;
}

hal_gpio_port_t* hal_gpio_get_port_by_index(uint8_t index) {
    if (index >= 8) {
        return NULL;
    }
    return &gpio_port_handles[index];
}

int hal_gpio_enable_port_clock(hal_gpio_port_t* port) {
    if (!port || !port->hw_port) {
        return -1;  // Error
    }
    
    enable_port_clocks((GPIO_TypeDef*)port->hw_port);
    return 0;  // Success
}

int hal_gpio_disable_port_clock(hal_gpio_port_t* port) {
    if (!port || !port->hw_port) {
        return -1;  // Error
    }
    
    disable_port_clocks((GPIO_TypeDef*)port->hw_port);
    return 0;  // Success
}

int hal_gpio_init(hal_gpio_port_t* port, const hal_gpio_init_t* init) {
    if (!port || !port->hw_port || !init) {
        return -1;  // Error
    }
    
    GPIO_InitTypeDef gpio_init = {0};
    
    // Convert pin number to pin mask
    gpio_init.Pin = (1U << init->pin);
    gpio_init.Mode = convert_gpio_mode(init->mode);
    gpio_init.Pull = convert_gpio_pull(init->pull);
    gpio_init.Speed = convert_gpio_speed(init->speed);
    gpio_init.Alternate = init->alternate;
    
    HAL_GPIO_Init((GPIO_TypeDef*)port->hw_port, &gpio_init);
    
    return 0;  // Success
}

int hal_gpio_deinit(hal_gpio_port_t* port, uint16_t pin) {
    if (!port || !port->hw_port || pin > 15) {
        return -1;  // Error
    }
    
    HAL_GPIO_DeInit((GPIO_TypeDef*)port->hw_port, (1U << pin));
    return 0;  // Success
}

int hal_gpio_write_pin(hal_gpio_port_t* port, uint16_t pin, hal_gpio_pin_state_t state) {
    if (!port || !port->hw_port || pin > 15) {
        return -1;  // Error
    }
    
    GPIO_PinState pin_state = (state == HAL_GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin((GPIO_TypeDef*)port->hw_port, (1U << pin), pin_state);
    
    return 0;  // Success
}

hal_gpio_pin_state_t hal_gpio_read_pin(hal_gpio_port_t* port, uint16_t pin) {
    if (!port || !port->hw_port || pin > 15) {
        return HAL_GPIO_PIN_RESET;
    }
    
    GPIO_PinState state = HAL_GPIO_ReadPin((GPIO_TypeDef*)port->hw_port, (1U << pin));
    return (state == GPIO_PIN_SET) ? HAL_GPIO_PIN_SET : HAL_GPIO_PIN_RESET;
}

int hal_gpio_toggle_pin(hal_gpio_port_t* port, uint16_t pin) {
    if (!port || !port->hw_port || pin > 15) {
        return -1;  // Error
    }
    
    HAL_GPIO_TogglePin((GPIO_TypeDef*)port->hw_port, (1U << pin));
    return 0;  // Success
}

void hal_gpio_write_port(hal_gpio_port_t* port, uint16_t value) {
    if (!port || !port->hw_port) {
        return;
    }
    
    // Write to output data register
    ((GPIO_TypeDef*)port->hw_port)->ODR = value;
}

uint16_t hal_gpio_read_port(hal_gpio_port_t* port) {
    if (!port || !port->hw_port) {
        return 0;
    }
    
    return (uint16_t)(((GPIO_TypeDef*)port->hw_port)->IDR & 0xFFFF);
}

#ifdef __cplusplus
}
#endif
