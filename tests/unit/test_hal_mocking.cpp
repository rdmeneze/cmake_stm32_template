#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "stm32_test_fixture.h"

// Example of how to properly test code that uses STM32 HAL functions
class HALMockingTest : public STM32TestFixture {
protected:
    void SetUp() override {
        STM32TestFixture::SetUp();
        
        // Setup specific expectations for this test class
        SetupGPIOExpectations();
    }
    
    void SetupGPIOExpectations() {
        // Default GPIO expectations that can be overridden in individual tests
        EXPECT_CALL(*mock_hal, GPIO_Init(::testing::_, ::testing::_))
            .WillRepeatedly(::testing::Return());
            
        EXPECT_CALL(*mock_hal, GPIO_WritePin(::testing::_, ::testing::_, ::testing::_))
            .WillRepeatedly(::testing::Return());
            
        EXPECT_CALL(*mock_hal, GPIO_TogglePin(::testing::_, ::testing::_))
            .WillRepeatedly(::testing::Return());
    }
};

TEST_F(HALMockingTest, GPIOInitialization) {
    // Test specific expectations
    EXPECT_CALL(*mock_hal, GPIO_Init(::testing::NotNull(), ::testing::NotNull()))
        .Times(1);
    
    // Simulate calling HAL_GPIO_Init
    GPIO_InitTypeDef gpio_init = {};
    gpio_init.Pin = GPIO_PIN_3;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    
    // This would be called in your actual code
    HAL_GPIO_Init(GPIOB, &gpio_init);
}

TEST_F(HALMockingTest, GPIOWritePin) {
    // Test that GPIO_WritePin is called with correct parameters
    EXPECT_CALL(*mock_hal, GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET))
        .Times(1);
    
    // This would be called in your actual code
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
}

TEST_F(HALMockingTest, GPIOTogglePin) {
    // Test that GPIO_TogglePin is called with correct parameters
    EXPECT_CALL(*mock_hal, GPIO_TogglePin(GPIOB, GPIO_PIN_3))
        .Times(1);
    
    // This would be called in your actual code
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
}

TEST_F(HALMockingTest, HALDelayFunctionality) {
    // Test HAL_Delay functionality
    EXPECT_CALL(*mock_hal, HAL_Delay(500))
        .Times(1);
    
    // This would be called in your actual code
    HAL_Delay(500);
}

TEST_F(HALMockingTest, HALTickFunctionality) {
    // Setup return value for HAL_GetTick
    EXPECT_CALL(*mock_hal, HAL_GetTick())
        .WillOnce(::testing::Return(1000))
        .WillOnce(::testing::Return(1500));
    
    // Test the tick functionality
    uint32_t tick1 = HAL_GetTick();
    uint32_t tick2 = HAL_GetTick();
    
    EXPECT_EQ(tick1, 1000);
    EXPECT_EQ(tick2, 1500);
    EXPECT_GT(tick2, tick1);
}
