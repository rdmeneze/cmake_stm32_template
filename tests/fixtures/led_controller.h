#pragma once

#include <cstdint>

// Simple LED controller class for demonstration
class LedController {
public:
    LedController(uint32_t pin);
    ~LedController() = default;

    void init();
    void toggle();
    void turnOn();
    void turnOff();
    bool isOn() const;

private:
    uint32_t pin_;
    bool state_;
};
