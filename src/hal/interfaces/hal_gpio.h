#pragma once

/**
 * @file hal_gpio.h
 * @brief Portable GPIO Hardware Abstraction Layer Interface
 * 
 * This interface provides a portable API for GPIO operations across
 * different STM32 microcontroller families.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// Forward declarations
typedef struct hal_gpio_port hal_gpio_port_t;
typedef struct hal_gpio_pin hal_gpio_pin_t;

/**
 * @brief GPIO Pin States
 */
typedef enum {
    HAL_GPIO_PIN_RESET = 0,
    HAL_GPIO_PIN_SET = 1
} hal_gpio_pin_state_t;

/**
 * @brief GPIO Pin Modes
 */
typedef enum {
    HAL_GPIO_MODE_INPUT,
    HAL_GPIO_MODE_OUTPUT_PP,        // Push-pull
    HAL_GPIO_MODE_OUTPUT_OD,        // Open-drain
    HAL_GPIO_MODE_AF_PP,            // Alternate function push-pull
    HAL_GPIO_MODE_AF_OD,            // Alternate function open-drain
    HAL_GPIO_MODE_ANALOG,           // Analog mode
    HAL_GPIO_MODE_IT_RISING,        // Interrupt rising edge
    HAL_GPIO_MODE_IT_FALLING,       // Interrupt falling edge
    HAL_GPIO_MODE_IT_RISING_FALLING // Interrupt both edges
} hal_gpio_mode_t;

/**
 * @brief GPIO Pull Configuration
 */
typedef enum {
    HAL_GPIO_NOPULL,
    HAL_GPIO_PULLUP,
    HAL_GPIO_PULLDOWN
} hal_gpio_pull_t;

/**
 * @brief GPIO Speed Configuration
 */
typedef enum {
    HAL_GPIO_SPEED_LOW,
    HAL_GPIO_SPEED_MEDIUM,
    HAL_GPIO_SPEED_HIGH,
    HAL_GPIO_SPEED_VERY_HIGH
} hal_gpio_speed_t;

/**
 * @brief GPIO Pin Configuration Structure
 */
typedef struct {
    uint16_t pin;                   // Pin number (HAL_GPIO_PIN_x)
    hal_gpio_mode_t mode;           // Pin mode
    hal_gpio_pull_t pull;           // Pull configuration
    hal_gpio_speed_t speed;         // Speed configuration
    uint32_t alternate;             // Alternate function (if applicable)
} hal_gpio_init_t;

/**
 * @brief GPIO Port Handle Structure
 */
struct hal_gpio_port {
    void* hw_port;                  // Hardware-specific port handle
    const char* name;               // Port name (e.g., "GPIOB")
    uint32_t base_address;          // Port base address
};

/**
 * @brief GPIO Pin Handle Structure
 */
struct hal_gpio_pin {
    hal_gpio_port_t* port;          // Associated GPIO port
    uint16_t pin;                   // Pin number
    hal_gpio_init_t config;         // Pin configuration
};

// =============================================================================
// Portable GPIO API Functions
// =============================================================================

/**
 * @brief Initialize a GPIO pin
 * @param port GPIO port handle
 * @param config Pin configuration
 * @return 0 on success, negative error code on failure
 */
int hal_gpio_init(hal_gpio_port_t* port, const hal_gpio_init_t* config);

/**
 * @brief Deinitialize a GPIO pin
 * @param port GPIO port handle
 * @param pin Pin number
 * @return 0 on success, negative error code on failure
 */
int hal_gpio_deinit(hal_gpio_port_t* port, uint16_t pin);

/**
 * @brief Read the state of a GPIO pin
 * @param port GPIO port handle
 * @param pin Pin number
 * @return Pin state (HAL_GPIO_PIN_SET or HAL_GPIO_PIN_RESET)
 */
hal_gpio_pin_state_t hal_gpio_read_pin(hal_gpio_port_t* port, uint16_t pin);

/**
 * @brief Write to a GPIO pin
 * @param port GPIO port handle
 * @param pin Pin number
 * @param state Pin state to set
 * @return 0 on success, negative error code on failure
 */
int hal_gpio_write_pin(hal_gpio_port_t* port, uint16_t pin, hal_gpio_pin_state_t state);

/**
 * @brief Toggle a GPIO pin
 * @param port GPIO port handle
 * @param pin Pin number
 * @return 0 on success, negative error code on failure
 */
int hal_gpio_toggle_pin(hal_gpio_port_t* port, uint16_t pin);

/**
 * @brief Enable GPIO port clock
 * @param port GPIO port handle
 * @return 0 on success, negative error code on failure
 */
int hal_gpio_enable_port_clock(hal_gpio_port_t* port);

/**
 * @brief Disable GPIO port clock
 * @param port GPIO port handle
 * @return 0 on success, negative error code on failure
 */
int hal_gpio_disable_port_clock(hal_gpio_port_t* port);

// =============================================================================
// Port Handle Accessors (implemented by BSP)
// =============================================================================

/**
 * @brief Get GPIO port handle by name
 * @param port_name Port name (e.g., "GPIOB", "GPIOA")
 * @return Pointer to port handle or NULL if not found
 */
hal_gpio_port_t* hal_gpio_get_port(const char* port_name);

/**
 * @brief Get GPIO port handle by index
 * @param port_index Port index (0=GPIOA, 1=GPIOB, etc.)
 * @return Pointer to port handle or NULL if invalid index
 */
hal_gpio_port_t* hal_gpio_get_port_by_index(uint8_t port_index);

#ifdef __cplusplus
}
#endif
