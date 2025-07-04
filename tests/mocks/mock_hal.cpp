#include "mock_hal.h"
#include <stdexcept>

// Global mock instance
MockHALImpl* g_mock_hal = nullptr;

void SetMockHAL(MockHALImpl* mock) {
    g_mock_hal = mock;
}

void ClearMockHAL() {
    g_mock_hal = nullptr;
}

// C function implementations that delegate to the mock
extern "C" {
    void HAL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init) {
        if (g_mock_hal) {
            g_mock_hal->GPIO_Init(GPIOx, GPIO_Init);
        } else {
            throw std::runtime_error("Mock HAL not set");
        }
    }
    
    void HAL_GPIO_DeInit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin) {
        if (g_mock_hal) {
            g_mock_hal->GPIO_DeInit(GPIOx, GPIO_Pin);
        } else {
            throw std::runtime_error("Mock HAL not set");
        }
    }
    
    GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
        if (g_mock_hal) {
            return g_mock_hal->GPIO_ReadPin(GPIOx, GPIO_Pin);
        } else {
            throw std::runtime_error("Mock HAL not set");
        }
    }
    
    void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
        if (g_mock_hal) {
            g_mock_hal->GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
        } else {
            throw std::runtime_error("Mock HAL not set");
        }
    }
    
    void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
        if (g_mock_hal) {
            g_mock_hal->GPIO_TogglePin(GPIOx, GPIO_Pin);
        } else {
            throw std::runtime_error("Mock HAL not set");
        }
    }
    
    HAL_StatusTypeDef HAL_Init(void) {
        if (g_mock_hal) {
            return g_mock_hal->HAL_Init();
        } else {
            throw std::runtime_error("Mock HAL not set");
        }
    }
    
    void HAL_Delay(uint32_t Delay) {
        if (g_mock_hal) {
            g_mock_hal->HAL_Delay(Delay);
        } else {
            throw std::runtime_error("Mock HAL not set");
        }
    }
    
    uint32_t HAL_GetTick(void) {
        if (g_mock_hal) {
            return g_mock_hal->HAL_GetTick();
        } else {
            throw std::runtime_error("Mock HAL not set");
        }
    }
    
    void HAL_IncTick(void) {
        if (g_mock_hal) {
            g_mock_hal->HAL_IncTick();
        } else {
            throw std::runtime_error("Mock HAL not set");
        }
    }
}
