#pragma once

// Minimal STM32L4xx definitions for unit testing
// This file provides the necessary types and definitions to compile 
// STM32 HAL code in a host environment for testing

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// Basic STM32 types
typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef enum {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState;

// GPIO definitions
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define GPIO_MODE_INPUT                        ((uint32_t)0x00000000)   /*!< Input Floating Mode                   */
#define GPIO_MODE_OUTPUT_PP                    ((uint32_t)0x00000001)   /*!< Output Push Pull Mode                 */
#define GPIO_MODE_OUTPUT_OD                    ((uint32_t)0x00000011)   /*!< Output Open Drain Mode                */
#define GPIO_MODE_AF_PP                        ((uint32_t)0x00000002)   /*!< Alternate Function Push Pull Mode     */
#define GPIO_MODE_AF_OD                        ((uint32_t)0x00000012)   /*!< Alternate Function Open Drain Mode    */
#define GPIO_MODE_ANALOG                       ((uint32_t)0x00000003)   /*!< Analog Mode  */

#define GPIO_NOPULL                            ((uint32_t)0x00000000)   /*!< No Pull-up or Pull-down activation  */
#define GPIO_PULLUP                            ((uint32_t)0x00000001)   /*!< Pull-up activation                  */
#define GPIO_PULLDOWN                          ((uint32_t)0x00000002)   /*!< Pull-down activation                */

#define GPIO_SPEED_FREQ_LOW                    ((uint32_t)0x00000000)  /*!< IO works at 2 MHz, please refer to the product datasheet */
#define GPIO_SPEED_FREQ_MEDIUM                 ((uint32_t)0x00000001)  /*!< range 12,5 MHz to 25 MHz, please refer to the product datasheet */
#define GPIO_SPEED_FREQ_HIGH                   ((uint32_t)0x00000002)  /*!< range 25 MHz to 50 MHz, please refer to the product datasheet  */
#define GPIO_SPEED_FREQ_VERY_HIGH              ((uint32_t)0x00000003)  /*!< range 50 MHz to 80 MHz, please refer to the product datasheet  */

// GPIO port structure (dummy for testing)
typedef struct {
    volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
    volatile uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
    volatile uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
    volatile uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    volatile uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
    volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
    volatile uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
    volatile uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    volatile uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
    volatile uint32_t BRR;      /*!< GPIO Bit Reset register,               Address offset: 0x28      */
    volatile uint32_t ASCR;     /*!< GPIO analog switch control register,   Address offset: 0x2C     */
} GPIO_TypeDef;

// GPIO initialization structure
typedef struct {
    uint32_t Pin;       /*!< Specifies the GPIO pins to be configured.
                             This parameter can be any value of @ref GPIO_pins_define */

    uint32_t Mode;      /*!< Specifies the operating mode for the selected pins.
                             This parameter can be a value of @ref GPIO_mode_define */

    uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                             This parameter can be a value of @ref GPIO_pull_define */

    uint32_t Speed;     /*!< Specifies the speed for the selected pins.
                             This parameter can be a value of @ref GPIO_speed_define */

    uint32_t Alternate; /*!< Peripheral to be connected to the selected pins.
                             This parameter can be a value of @ref GPIO_Alternate_function_selection */
} GPIO_InitTypeDef;

// Dummy GPIO port addresses for testing
extern GPIO_TypeDef* const GPIOA;
extern GPIO_TypeDef* const GPIOB;
extern GPIO_TypeDef* const GPIOC;
extern GPIO_TypeDef* const GPIOD;
extern GPIO_TypeDef* const GPIOE;
extern GPIO_TypeDef* const GPIOF;
extern GPIO_TypeDef* const GPIOG;
extern GPIO_TypeDef* const GPIOH;

// HAL function declarations (these will be mocked)
HAL_StatusTypeDef HAL_Init(void);
void HAL_Delay(uint32_t Delay);
uint32_t HAL_GetTick(void);
void HAL_IncTick(void);

void HAL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init);
void HAL_GPIO_DeInit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif
