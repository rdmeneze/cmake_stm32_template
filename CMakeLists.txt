cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME stm32l432_firmware)
project(${PROJECT_NAME} C CXX ASM)

# main directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/app)
set(HAL_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/STM32L4xx_HAL_Driver)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS)
set(STARTUP_DIR ${CMAKE_SOURCE_DIR}/startup)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32L432KCUX_FLASH.ld)

# toolchain config
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-gcc-toolchain.cmake)

# toolchain flags
set(COMMON_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

add_definitions(
        -DSTM32L432xx
        -DUSE_HAL_DRIVER
)

# Arquivos fonte
file(GLOB_RECURSE APP_SOURCES
        ${SRC_DIR}/*.cpp
)

file(GLOB HAL_SOURCES
        ${HAL_DIR}/Src/*.c
)

add_executable(${PROJECT_NAME}
        ${APP_SOURCES}
        ${HAL_SOURCES}
        ${SYSTEM_FILE}
        ${STARTUP_FILE}
)

target_compile_options(${PROJECT_NAME} PRIVATE
        ${COMMON_FLAGS}
        -Wall
        -Wextra
        -ffunction-sections
        -fdata-sections
)

target_link_options(${PROJECT_NAME} PRIVATE
        -T${LINKER_SCRIPT}
        -Wl,--gc-sections
        -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
)


set(STARTUP_FILE ${STARTUP_DIR}/Src/startup_stm32l432kcux.s)
set(SYSTEM_FILE ${STARTUP_DIR}/Src/system_stm32l4xx.c)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
        ${SRC_DIR}
        ${HAL_DIR}/Inc
        ${CMSIS_DIR}/Include
        ${CMSIS_DIR}/Device/ST/STM32L4xx/Include
        ${STARTUP_DIR}/Inc
)

# Desabilitar libc padrão (opcional, se não usar)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

# Post build: gerar bin e hex
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf firmware.bin
        COMMAND arm-none-eabi-objcopy -O ihex   ${PROJECT_NAME}.elf firmware.hex
)


