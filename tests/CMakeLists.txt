cmake_minimum_required(VERSION 3.21)

# Dedicated project for testing
project(stm32l432_tests C CXX)

# Set standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Only build if this is the main project (not a subproject)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(STATUS "Building STM32L432 unit tests...")

    # Download and build Google Test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    # Enable testing
    enable_testing()

    # Create test executable
    add_executable(unit_tests
        unit/test_smoke.cpp
        unit/test_led_controller.cpp
        unit/test_hal_mocking.cpp
        unit/test_real_led_controller.cpp
        unit/test_main_functions.cpp
        fixtures/led_controller.cpp
        fixtures/real_led_controller.cpp
        fixtures/main_functions.cpp
        fixtures/stm32l4xx_hal.cpp
        fixtures/stm32_memory_mock.cpp
        mocks/mock_hal.cpp
        mocks/mock_freertos.cpp
    )

    target_link_libraries(unit_tests
        gtest_main
        gmock_main
    )

    target_compile_definitions(unit_tests PRIVATE
        UNIT_TESTING
    )

    target_include_directories(unit_tests PRIVATE
        fixtures
        mocks
    )

    # Register tests with CTest
    include(GoogleTest)
    gtest_discover_tests(unit_tests)

    message(STATUS "Unit tests configured successfully")
endif()
