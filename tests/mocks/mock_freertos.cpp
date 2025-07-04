#include "mock_freertos.h"

// Global mock instance
MockFreeRTOSImpl* g_freertos_mock = nullptr;

// Simple C wrapper functions for FreeRTOS API
extern "C" {

void vTaskDelay(uint32_t xTicksToDelay) {
    if (g_freertos_mock) {
        g_freertos_mock->vTaskDelay(xTicksToDelay);
    }
}

uint32_t xTaskGetTickCount() {
    if (g_freertos_mock) {
        return g_freertos_mock->xTaskGetTickCount();
    }
    return 0;
}

void vTaskStartScheduler() {
    if (g_freertos_mock) {
        g_freertos_mock->vTaskStartScheduler();
    }
}

}
