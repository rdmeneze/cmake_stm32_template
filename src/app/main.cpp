
extern "C" {
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_rcc.h"
#include "stm32l4xx_hal_gpio.h"
#include "version.h"
#include "FreeRTOS.h"
#include "task.h"
}

void SystemClock_Config(){};

// Task handles
TaskHandle_t ledTaskHandle = NULL;
TaskHandle_t heartbeatTaskHandle = NULL;

// LED Task - Blinks LED on PB3
void ledTask(void *pvParameters) {
    (void)pvParameters;
    
    while (1) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        vTaskDelay(pdMS_TO_TICKS(500));  // 500ms delay
    }
}

// Heartbeat Task - Demonstrates multiple tasks
void heartbeatTask(void *pvParameters) {
    (void)pvParameters;
    uint32_t counter = 0;
    
    while (1) {
        counter++;
        // This task runs every 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Initialize GPIO for LED
void GPIO_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio{};
    gpio.Pin = GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);
}

extern "C" int main() {
    HAL_Init();
    SystemClock_Config();
    
    // Initialize GPIO
    GPIO_Init();
    
    // Create tasks
    xTaskCreate(ledTask, "LED_Task", 256, NULL, 1, &ledTaskHandle);
    xTaskCreate(heartbeatTask, "Heartbeat_Task", 256, NULL, 1, &heartbeatTaskHandle);
    
    // Start the scheduler
    vTaskStartScheduler();
    
    // We should never reach here if everything went well
    while (1) {
        // This should never execute
    }
}

// FreeRTOS hooks
extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];
    
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)xTask;
    (void)pcTaskName;
    
    // Stack overflow detected - handle error
    while (1) {
        // Infinite loop for debugging
    }
}

// Required for FreeRTOS Timer Task when using static allocation
extern "C" void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];
    
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
