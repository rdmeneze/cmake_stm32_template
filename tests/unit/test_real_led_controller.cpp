#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "stm32_test_fixture.h"
#include "real_led_controller.h"

// Test the RealLedController that actually uses STM32 HAL functions
class RealLedControllerTest : public STM32TestFixture {
protected:
    void SetUp() override {
        STM32TestFixture::SetUp();
        
        // Create controller instance (GPIOB, PIN_3)
        led_controller = std::make_unique<RealLedController>(GPIOB, GPIO_PIN_3);
    }
    
    void TearDown() override {
        led_controller.reset();
        STM32TestFixture::TearDown();
    }
    
    std::unique_ptr<RealLedController> led_controller;
};

TEST_F(RealLedControllerTest, InitializationCallsHALGPIOInit) {
    // Expect that HAL_GPIO_Init is called with correct parameters
    EXPECT_CALL(*mock_hal, GPIO_Init(GPIOB, ::testing::_))
        .Times(1)
        .WillOnce(::testing::DoAll(
            // Verify the GPIO_InitTypeDef structure has correct values
            ::testing::Invoke([](GPIO_TypeDef* port, GPIO_InitTypeDef* init) {
                EXPECT_EQ(init->Pin, GPIO_PIN_3);
                EXPECT_EQ(init->Mode, GPIO_MODE_OUTPUT_PP);
                EXPECT_EQ(init->Pull, GPIO_NOPULL);
                EXPECT_EQ(init->Speed, GPIO_SPEED_FREQ_LOW);
            })
        ));
    
    led_controller->init();
}

TEST_F(RealLedControllerTest, TurnOnCallsHALGPIOWritePin) {
    // Expect that HAL_GPIO_WritePin is called with GPIO_PIN_SET
    EXPECT_CALL(*mock_hal, GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET))
        .Times(1);
    
    led_controller->turnOn();
    EXPECT_TRUE(led_controller->isOn());
}

TEST_F(RealLedControllerTest, TurnOffCallsHALGPIOWritePin) {
    // Expect that HAL_GPIO_WritePin is called with GPIO_PIN_RESET
    EXPECT_CALL(*mock_hal, GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET))
        .Times(1);
    
    led_controller->turnOff();
    EXPECT_FALSE(led_controller->isOn());
}

TEST_F(RealLedControllerTest, ToggleCallsHALGPIOTogglePin) {
    // Expect that HAL_GPIO_TogglePin is called
    EXPECT_CALL(*mock_hal, GPIO_TogglePin(GPIOB, GPIO_PIN_3))
        .Times(2); // We'll toggle twice
    
    // Initially off
    EXPECT_FALSE(led_controller->isOn());
    
    // Toggle to on
    led_controller->toggle();
    EXPECT_TRUE(led_controller->isOn());
    
    // Toggle back to off
    led_controller->toggle();
    EXPECT_FALSE(led_controller->isOn());
}

TEST_F(RealLedControllerTest, ReadHardwareStateCallsHALGPIOReadPin) {
    // Setup expectations for HAL_GPIO_ReadPin
    EXPECT_CALL(*mock_hal, GPIO_ReadPin(GPIOB, GPIO_PIN_3))
        .WillOnce(::testing::Return(GPIO_PIN_SET))
        .WillOnce(::testing::Return(GPIO_PIN_RESET));
    
    // Test reading hardware state
    bool state1 = led_controller->readHardwareState();
    bool state2 = led_controller->readHardwareState();
    
    EXPECT_TRUE(state1);   // GPIO_PIN_SET
    EXPECT_FALSE(state2);  // GPIO_PIN_RESET
}

TEST_F(RealLedControllerTest, ComplexScenarioWithMultipleHALCalls) {
    // Complex test that combines multiple HAL operations
    
    // Use InSequence to ensure the calls happen in order
    ::testing::InSequence seq;
    
    // Step 1: Initialize the LED
    EXPECT_CALL(*mock_hal, GPIO_Init(GPIOB, ::testing::_))
        .Times(1);
    
    // Step 2: Turn on the LED
    EXPECT_CALL(*mock_hal, GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET))
        .Times(1);
    
    // Step 3: Read hardware state (simulate hardware confirming LED is on)
    EXPECT_CALL(*mock_hal, GPIO_ReadPin(GPIOB, GPIO_PIN_3))
        .WillOnce(::testing::Return(GPIO_PIN_SET));
    
    // Step 4: Toggle the LED
    EXPECT_CALL(*mock_hal, GPIO_TogglePin(GPIOB, GPIO_PIN_3))
        .Times(1);
    
    // Step 5: Read hardware state again (LED should be off)
    EXPECT_CALL(*mock_hal, GPIO_ReadPin(GPIOB, GPIO_PIN_3))
        .WillOnce(::testing::Return(GPIO_PIN_RESET));
    
    // Execute the scenario
    led_controller->init();
    led_controller->turnOn();
    EXPECT_TRUE(led_controller->isOn());
    EXPECT_TRUE(led_controller->readHardwareState());
    
    led_controller->toggle();
    EXPECT_FALSE(led_controller->isOn());
    EXPECT_FALSE(led_controller->readHardwareState());
}
