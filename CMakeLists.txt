cmake_minimum_required(VERSION 3.21)

# Toolchain config
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-gcc-toolchain.cmake)

set(PROJECT_NAME stm32l432_firmware)
project(${PROJECT_NAME} C CXX ASM)

# language settings
# CMake 3.21+ is required for C++17 support
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Main directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/app)
set(HAL_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/STM32L4xx_HAL_Driver)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS)
set(STARTUP_DIR ${CMAKE_SOURCE_DIR}/startup)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32L432KCUX_FLASH.ld)


# Flags comuns
set(COMMON_FLAGS
  -mcpu=cortex-m4
  -mthumb
  -mfpu=fpv4-sp-d16
  -mfloat-abi=hard
)

# source files
file(GLOB_RECURSE APP_SOURCES ${SRC_DIR}/*.cpp)
file(GLOB HAL_SOURCES ${HAL_DIR}/Src/*.c)

set(STARTUP_FILE ${STARTUP_DIR}/Src/startup_stm32l432kcux.s)
set(SYSTEM_FILE ${STARTUP_DIR}/Src/system_stm32l4xx.c)
set(SYSCALL_FILES
  ${STARTUP_DIR}/Src/syscalls.c
  ${STARTUP_DIR}/Src/sysmem.c
)

add_executable(${PROJECT_NAME}
  ${APP_SOURCES}
  ${HAL_SOURCES}
  ${SYSTEM_FILE}
  ${STARTUP_FILE}
  ${SYSCALL_FILES}
)

# compilation definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
  STM32L432xx
  USE_HAL_DRIVER
)

# compilation options
target_compile_options(${PROJECT_NAME} PRIVATE
  ${COMMON_FLAGS}
  -Wall
  -Wextra
  -ffunction-sections
  -fdata-sections
)

# linker options
target_link_options(${PROJECT_NAME} PRIVATE
  ${COMMON_FLAGS}
  -T${LINKER_SCRIPT}
  -Wl,--gc-sections
  -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
  ${SRC_DIR}
  ${HAL_DIR}/Inc
  ${CMSIS_DIR}/Include
  ${CMSIS_DIR}/Device/ST/STM32L4xx/Include
  ${STARTUP_DIR}/Inc
)

# disable standard libc (optional, if not used)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

# Post-build: generate bin and hex
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> firmware.bin
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> firmware.hex
)
