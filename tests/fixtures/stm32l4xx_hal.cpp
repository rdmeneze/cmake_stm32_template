#include "stm32l4xx_hal.h"

// Dummy GPIO port instances for testing
static GPIO_TypeDef gpio_a_instance;
static GPIO_TypeDef gpio_b_instance;
static GPIO_TypeDef gpio_c_instance;
static GPIO_TypeDef gpio_d_instance;
static GPIO_TypeDef gpio_e_instance;
static GPIO_TypeDef gpio_f_instance;
static GPIO_TypeDef gpio_g_instance;
static GPIO_TypeDef gpio_h_instance;

GPIO_TypeDef* const GPIOA = &gpio_a_instance;
GPIO_TypeDef* const GPIOB = &gpio_b_instance;
GPIO_TypeDef* const GPIOC = &gpio_c_instance;
GPIO_TypeDef* const GPIOD = &gpio_d_instance;
GPIO_TypeDef* const GPIOE = &gpio_e_instance;
GPIO_TypeDef* const GPIOF = &gpio_f_instance;
GPIO_TypeDef* const GPIOG = &gpio_g_instance;
GPIO_TypeDef* const GPIOH = &gpio_h_instance;
