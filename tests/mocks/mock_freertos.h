#pragma once

#include <gmock/gmock.h>
#include <cstdint>

// Simple FreeRTOS mock interface
class MockFreeRTOS {
public:
    virtual ~MockFreeRTOS() = default;

    // Simple task management functions
    virtual void vTaskDelay(uint32_t xTicksToDelay) = 0;
    virtual uint32_t xTaskGetTickCount() = 0;
    virtual void vTaskStartScheduler() = 0;
};

// GMock implementation
class MockFreeRTOSImpl : public MockFreeRTOS {
public:
    MOCK_METHOD(void, vTaskDelay, (uint32_t xTicksToDelay), (override));
    MOCK_METHOD(uint32_t, xTaskGetTickCount, (), (override));
    MOCK_METHOD(void, vTaskStartScheduler, (), (override));
};

// Global mock instance
extern MockFreeRTOSImpl* g_freertos_mock;

// Global mock instance
extern MockFreeRTOSImpl* g_mock_freertos;

// Function to set the global mock instance
void SetMockFreeRTOS(MockFreeRTOSImpl* mock);

// Function to clear the global mock instance
void ClearMockFreeRTOS();
