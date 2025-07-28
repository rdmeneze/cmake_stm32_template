#pragma once

#include "stm32l4xx_hal.h"
#include <cstdint>

// Real LED controller that actually uses STM32 HAL functions
class RealLedController {
public:
    RealLedController(GPIO_TypeDef* port, uint16_t pin);
    ~RealLedController() = default;

    void init();
    void toggle();
    void turnOn();
    void turnOff();
    bool isOn() const;
    
    // Method to check GPIO state from hardware
    bool readHardwareState() const;

private:
    GPIO_TypeDef* port_;
    uint16_t pin_;
    bool state_;
};
