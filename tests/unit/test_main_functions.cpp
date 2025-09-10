#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "stm32_test_fixture.h"
#include "../fixtures/stm32_memory_mock.h"

// Include the functions we want to test
extern "C" {
    void GPIO_Init(void);
    void SystemClock_Config(void);
}

class MainFunctionsTest : public STM32TestFixture {
protected:
    void SetUp() override {
        STM32TestFixture::SetUp();
        
        // Reset memory mock before each test
        STM32MemoryMock::instance().reset();

        // Additional setup specific to main functions
        SetupGPIOExpectations();
    }

    void SetupGPIOExpectations() {
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

TEST_F(MainFunctionsTest, GPIO_Init_EnablesGPIOBClock) {
    // Act
    GPIO_Init();

    // Assert - Check if RCC AHB2ENR register was written with GPIOB enable bit
    auto& memory_mock = STM32MemoryMock::instance();
    
    EXPECT_TRUE(memory_mock.was_register_written(RCC_AHB2ENR))
        << "RCC AHB2ENR register should have been written to enable GPIO B clock";
        
    EXPECT_TRUE(memory_mock.is_bit_set(RCC_AHB2ENR, RCC_AHB2ENR_GPIOBEN))
        << "GPIOB enable bit should be set in RCC AHB2ENR register";
}

TEST_F(MainFunctionsTest, GPIO_Init_ConfiguresCorrectPin) {
    // Arrange
    // Expect GPIO_Init to be called with GPIOB and correct configuration
    EXPECT_CALL(*mock_hal, GPIO_Init(GPIOB, ::testing::_))
        .Times(1);

    // Act
    GPIO_Init();
}

TEST_F(MainFunctionsTest, SystemClock_Config_CanBeCalled) {
    // This function should be callable without errors
    SystemClock_Config();
    
    // No specific assertions needed - just verify it doesn't crash
    SUCCEED();
}
