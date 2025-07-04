#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "stm32_test_fixture.h"

// Include the functions we want to test
// Note: We'll need to extract testable functions from main.cpp
extern "C" {
    void GPIO_Init(void);
    // Add other function declarations you want to test
}

class MainFunctionsTest : public STM32TestFixture {
protected:
    void SetUp() override {
        STM32TestFixture::SetUp();

        // Additional setup specific to main functions
        SetupGPIOExpectations();
    }

    void SetupGPIOExpectations() {
        // Expect GPIO clock enable to be called
        // Note: This is a macro, so we'd need to mock the underlying RCC function

        // Expect GPIO_Init to be called with correct parameters
        EXPECT_CALL(*mock_hal, GPIO_Init(::testing::_, ::testing::_))
            .WillRepeatedly(::testing::Return());
    }
};

TEST_F(MainFunctionsTest, GPIO_Init_CallsHAL_GPIO_Init) {
    // Act
    GPIO_Init();

    // Assert is handled by the expectations set in SetupGPIOExpectations
}

TEST_F(MainFunctionsTest, GPIO_Init_ConfiguresCorrectPin) {
    // Arrange
    GPIO_InitTypeDef expected_config;
    expected_config.Pin = GPIO_PIN_3;
    expected_config.Mode = GPIO_MODE_OUTPUT_PP;
    expected_config.Pull = GPIO_NOPULL;
    expected_config.Speed = GPIO_SPEED_FREQ_LOW;

    // Expect GPIO_Init to be called with GPIOB and correct configuration
    EXPECT_CALL(*mock_hal, GPIO_Init(::testing::_, ::testing::_))
        .With(::testing::Args<0, 1>(::testing::AllOf(
            ::testing::Field(&GPIO_InitTypeDef::Pin, GPIO_PIN_3),
            ::testing::Field(&GPIO_InitTypeDef::Mode, GPIO_MODE_OUTPUT_PP),
            ::testing::Field(&GPIO_InitTypeDef::Pull, GPIO_NOPULL),
            ::testing::Field(&GPIO_InitTypeDef::Speed, GPIO_SPEED_FREQ_LOW)
        )));

    // Act
    GPIO_Init();
}

// Example test for task creation
TEST_F(MainFunctionsTest, TasksAreCreatedCorrectly) {
    // Arrange
    EXPECT_CALL(*mock_freertos, xTaskCreate(::testing::_,
                                           ::testing::StrEq("LED_Task"),
                                           256,
                                           ::testing::IsNull(),
                                           1,
                                           ::testing::_))
        .Times(1)
        .WillOnce(::testing::Return(1)); // pdPASS

    EXPECT_CALL(*mock_freertos, xTaskCreate(::testing::_,
                                           ::testing::StrEq("Heartbeat_Task"),
                                           256,
                                           ::testing::IsNull(),
                                           1,
                                           ::testing::_))
        .Times(1)
        .WillOnce(::testing::Return(1)); // pdPASS

    // Act
    // You would call the function that creates tasks here
    // For now, this is just an example of how to test task creation

    // Note: You'll need to extract the task creation logic into a separate
    // function to make it testable
}

// Example test for LED toggle functionality
class LedTaskTest : public STM32TestFixture {
protected:
    void SetUp() override {
        STM32TestFixture::SetUp();
    }
};

TEST_F(LedTaskTest, LedTask_TogglesGPIO) {
    // Arrange
    EXPECT_CALL(*mock_hal, GPIO_TogglePin(::testing::_, GPIO_PIN_3))
        .Times(1);

    EXPECT_CALL(*mock_freertos, vTaskDelay(::testing::_))
        .Times(1);

    // Act
    // You would call the LED task function here
    // For now, this demonstrates how to test the toggle functionality

    // Note: You'll need to extract the task logic into testable functions
}
