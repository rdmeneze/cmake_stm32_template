# Unit Tests for STM32L432 Project

This directory contains unit tests for the STM32L432 firmware project using Google Test and Google Mock.

## Structure

```
tests/
├── CMakeLists.txt          # Test build configuration
├── unit/                   # Unit test files
│   ├── test_smoke.cpp      # Basic smoke tests
│   └── test_main_functions.cpp  # Tests for main application functions
├── mocks/                  # Mock implementations
│   ├── mock_hal.h/.cpp     # HAL function mocks
│   └── mock_freertos.h/.cpp # FreeRTOS function mocks
└── fixtures/               # Test fixtures and utilities
    └── stm32_test_fixture.h # Common test setup
```

## Dependencies

- **Google Test v1.14.0** - Testing framework
- **Google Mock** - Mocking framework (included with Google Test)
- **CMake 3.21+** - Build system

## Building and Running Tests

### Quick Start
```bash
# From project root directory
./run_tests.sh
```

### Manual Build
```bash
# Create build directory for tests
mkdir build-tests
cd build-tests

# Configure for host testing (not cross-compiling)
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build tests
make unit_tests

# Run tests
./tests/unit_tests
```

### Running Specific Tests
```bash
# Run only smoke tests
./tests/unit_tests --gtest_filter="SmokeTest.*"

# Run with verbose output
./tests/unit_tests --gtest_verbose

# List available tests
./tests/unit_tests --gtest_list_tests
```

## Writing Tests

### Basic Test Structure
```cpp
#include <gtest/gtest.h>
#include "stm32_test_fixture.h"

class MyModuleTest : public STM32TestFixture {
protected:
    void SetUp() override {
        STM32TestFixture::SetUp();
        // Additional setup
    }
};

TEST_F(MyModuleTest, TestDescription) {
    // Arrange
    EXPECT_CALL(*mock_hal, GPIO_TogglePin(_, _));

    // Act
    my_function_under_test();

    // Assert (handled by expectations)
}
```

### Available Mocks

#### HAL Functions
- `GPIO_Init`, `GPIO_DeInit`
- `GPIO_ReadPin`, `GPIO_WritePin`, `GPIO_TogglePin`
- `HAL_Init`, `HAL_Delay`, `HAL_GetTick`, `HAL_IncTick`

#### FreeRTOS Functions
- `xTaskCreate`, `vTaskDelete`
- `vTaskDelay`, `vTaskStartScheduler`
- `xTaskGetTickCount`

### Adding New Mocks

1. Add method to the mock interface in `mocks/mock_*.h`
2. Add `MOCK_METHOD` to the implementation class
3. Add C wrapper function in `mocks/mock_*.cpp`
4. Update test fixtures if needed

## Integration with CI/CD

The tests can be easily integrated into CI/CD pipelines:

```yaml
# Example GitHub Actions step
- name: Run Unit Tests
  run: |
    ./run_tests.sh
```

## Debugging Tests

### In VS Code
1. Set breakpoints in test files
2. Use "Debug Test" from the test explorer
3. Or run with debugger: `gdb ./tests/unit_tests`

### Test Output
- Use `--gtest_verbose` for detailed output
- Use `--gtest_repeat=N` to run tests multiple times
- Use `--gtest_shuffle` to randomize test order

## Best Practices

1. **Mock External Dependencies**: Always mock HAL, FreeRTOS, and other external functions
2. **Test One Thing**: Each test should verify one specific behavior
3. **Descriptive Names**: Use clear, descriptive test names
4. **Setup/Teardown**: Use fixtures for common setup and cleanup
5. **Arrange-Act-Assert**: Structure tests clearly with these three phases

## Troubleshooting

### Common Issues

1. **"Mock not set" errors**: Ensure you're using `STM32TestFixture` base class
2. **Linker errors**: Check that all mocked functions are properly wrapped
3. **Cross-compilation conflicts**: Tests should only run with host compiler

### Getting Help

- Check Google Test documentation: https://google.github.io/googletest/
- Review existing test examples in the `unit/` directory
- Ensure mock expectations match actual function calls
