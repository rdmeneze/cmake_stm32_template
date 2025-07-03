/*
 * FreeRTOS SysTick Handler configuration for STM32L432KC
 * This file provides the SysTick handler that works with FreeRTOS
 */

#include "stm32l4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    xPortSysTickHandler();
  }

  HAL_IncTick();

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/**
  * @brief  Provides a tick value in millisecond.
  * @note   This function is declared as __weak to be overwritten in case of other
  *         implementations in user file.
  * @retval tick value
  */
__weak uint32_t HAL_GetTick(void)
{
  return uwTick;
}

/**
  * @brief This function provides minimum delay (in milliseconds) based
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @param Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
__weak void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY) {
    wait += (uint32_t)(uwTickFreq);
  }

  while ((HAL_GetTick() - tickstart) < wait) {
    // When using FreeRTOS, we should yield to other tasks instead of busy waiting
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
      taskYIELD();
    }
  }
}
