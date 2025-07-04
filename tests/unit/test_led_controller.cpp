#include <gtest/gtest.h>
#include "led_controller.h"

// Test fixture for LED controller
class LedControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        led = std::make_unique<LedController>(3); // GPIO pin 3
    }

    void TearDown() override {
        led.reset();
    }

    std::unique_ptr<LedController> led;
};

TEST_F(LedControllerTest, ConstructorSetsInitialState) {
    EXPECT_FALSE(led->isOn());
}

TEST_F(LedControllerTest, TurnOnSetsState) {
    led->turnOn();
    EXPECT_TRUE(led->isOn());
}

TEST_F(LedControllerTest, TurnOffClearsState) {
    led->turnOn();
    EXPECT_TRUE(led->isOn());

    led->turnOff();
    EXPECT_FALSE(led->isOn());
}

TEST_F(LedControllerTest, ToggleChangesState) {
    // Initially off
    EXPECT_FALSE(led->isOn());

    // Toggle to on
    led->toggle();
    EXPECT_TRUE(led->isOn());

    // Toggle back to off
    led->toggle();
    EXPECT_FALSE(led->isOn());
}

TEST_F(LedControllerTest, MultipleToggles) {
    bool expected_state = false;

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(led->isOn(), expected_state);
        led->toggle();
        expected_state = !expected_state;
        EXPECT_EQ(led->isOn(), expected_state);
    }
}
