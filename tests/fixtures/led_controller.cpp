#include "led_controller.h"

LedController::LedController(uint32_t pin) : pin_(pin), state_(false) {
}

void LedController::init() {
    // In real implementation, this would call HAL_GPIO_Init
    // For testing, we can mock this
}

void LedController::toggle() {
    state_ = !state_;
    // In real implementation, this would call HAL_GPIO_TogglePin
}

void LedController::turnOn() {
    state_ = true;
    // In real implementation, this would call HAL_GPIO_WritePin
}

void LedController::turnOff() {
    state_ = false;
    // In real implementation, this would call HAL_GPIO_WritePin
}

bool LedController::isOn() const {
    return state_;
}
