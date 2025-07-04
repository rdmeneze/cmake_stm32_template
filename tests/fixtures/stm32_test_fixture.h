#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mock_hal.h"
#include "mock_freertos.h"

// Base test fixture that sets up common mocks
class STM32TestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Create mock instances
        mock_hal = std::make_unique<MockHALImpl>();
        mock_freertos = std::make_unique<MockFreeRTOSImpl>();

        // Set global mock instances
        SetMockHAL(mock_hal.get());
        SetMockFreeRTOS(mock_freertos.get());

        // Set up default expectations that are commonly used
        SetupDefaultExpectations();
    }

    void TearDown() override {
        // Clear global mock instances
        ClearMockHAL();
        ClearMockFreeRTOS();

        // Reset mocks
        mock_hal.reset();
        mock_freertos.reset();
    }

    void SetupDefaultExpectations() {
        // Common HAL expectations
        EXPECT_CALL(*mock_hal, HAL_Init())
            .WillRepeatedly(::testing::Return(HAL_OK));

        EXPECT_CALL(*mock_hal, HAL_GetTick())
            .WillRepeatedly(::testing::Return(0));

        // Common FreeRTOS expectations
        EXPECT_CALL(*mock_freertos, xTaskCreate(::testing::_, ::testing::_, ::testing::_,
                                               ::testing::_, ::testing::_, ::testing::_))
            .WillRepeatedly(::testing::Return(1)); // pdPASS

        EXPECT_CALL(*mock_freertos, vTaskStartScheduler())
            .WillRepeatedly(::testing::Return());
    }

    // Mock instances available to derived test classes
    std::unique_ptr<MockHALImpl> mock_hal;
    std::unique_ptr<MockFreeRTOSImpl> mock_freertos;
};
