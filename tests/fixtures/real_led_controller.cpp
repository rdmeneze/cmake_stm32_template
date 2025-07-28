#include "real_led_controller.h"

RealLedController::RealLedController(GPIO_TypeDef* port, uint16_t pin) 
    : port_(port), pin_(pin), state_(false) {
}

void RealLedController::init() {
    GPIO_InitTypeDef gpio_init = {};
    gpio_init.Pin = pin_;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    
    // This call will be intercepted by our mock in tests
    HAL_GPIO_Init(port_, &gpio_init);
}

void RealLedController::toggle() {
    state_ = !state_;
    // This call will be intercepted by our mock in tests
    HAL_GPIO_TogglePin(port_, pin_);
}

void RealLedController::turnOn() {
    state_ = true;
    // This call will be intercepted by our mock in tests
    HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET);
}

void RealLedController::turnOff() {
    state_ = false;
    // This call will be intercepted by our mock in tests
    HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET);
}

bool RealLedController::isOn() const {
    return state_;
}

bool RealLedController::readHardwareState() const {
    // This call will be intercepted by our mock in tests
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(port_, pin_);
    return (pin_state == GPIO_PIN_SET);
}
