#include "mock_freertos.h"

// Global mock instance (legacy name for compatibility)
MockFreeRTOSImpl* g_freertos_mock = nullptr;

// Global mock instance (new name)
MockFreeRTOSImpl* g_mock_freertos = nullptr;

void SetMockFreeRTOS(MockFreeRTOSImpl* mock) {
    g_freertos_mock = mock;
    g_mock_freertos = mock;
}

void ClearMockFreeRTOS() {
    g_freertos_mock = nullptr;
    g_mock_freertos = nullptr;
}

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
