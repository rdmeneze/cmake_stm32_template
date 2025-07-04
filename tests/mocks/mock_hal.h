#pragma once

#include <gmock/gmock.h>
#include "stm32l4xx_hal.h"

// Mock interface for STM32 HAL GPIO functions
class MockHAL {
public:
    virtual ~MockHAL() = default;

    // GPIO functions
    virtual void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init) = 0;
    virtual void GPIO_DeInit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin) = 0;
    virtual GPIO_PinState GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) = 0;
    virtual void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) = 0;
    virtual void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) = 0;

    // System functions
    virtual HAL_StatusTypeDef HAL_Init(void) = 0;
    virtual void HAL_Delay(uint32_t Delay) = 0;
    virtual uint32_t HAL_GetTick(void) = 0;
    virtual void HAL_IncTick(void) = 0;
};

// GMock implementation
class MockHALImpl : public MockHAL {
public:
    MOCK_METHOD(void, GPIO_Init, (GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init), (override));
    MOCK_METHOD(void, GPIO_DeInit, (GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin), (override));
    MOCK_METHOD(GPIO_PinState, GPIO_ReadPin, (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin), (override));
    MOCK_METHOD(void, GPIO_WritePin, (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState), (override));
    MOCK_METHOD(void, GPIO_TogglePin, (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin), (override));

    MOCK_METHOD(HAL_StatusTypeDef, HAL_Init, (), (override));
    MOCK_METHOD(void, HAL_Delay, (uint32_t Delay), (override));
    MOCK_METHOD(uint32_t, HAL_GetTick, (), (override));
    MOCK_METHOD(void, HAL_IncTick, (), (override));
};

// Global mock instance (will be defined in mock_hal.cpp)
extern MockHALImpl* g_mock_hal;

// Function to set the global mock instance
void SetMockHAL(MockHALImpl* mock);

// Function to clear the global mock instance
void ClearMockHAL();
