cmake_minimum_required(VERSION 3.21)
set(CMAKE_VERBOSE_MAKEFILE ON)
# Define o nome do projeto e linguagens usadas
project(stm32l432_firmware C CXX ASM)

# Padroniza versões das linguagens
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/STM32CubeL4")
  message(FATAL_ERROR "Submodule STM32CubeL4 did not exist. Please run:
                      git submodule update --init --recursive")
endif()

find_package(Git REQUIRED)

message(STATUS "Using Git executable: ${GIT_EXECUTABLE}")
message(STATUS "Using CMake version: ${CMAKE_VERSION}")
message(STATUS "Current working dir for Git: ${CMAKE_SOURCE_DIR}")
file(GLOB GIT_DIR_CONTENTS ${CMAKE_SOURCE_DIR}/.git)
message(STATUS "Git dir contents: ${GIT_DIR_CONTENTS}")

# Tenta usar git describe com tags começando com "v"
execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags --match "v*"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE GIT_RESULT
    OUTPUT_VARIABLE APP_VERSION
    ERROR_VARIABLE GIT_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Fallback: tenta pegar o hash curto se não achou uma tag válida
if(NOT APP_VERSION OR GIT_RESULT)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE APP_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(APP_VERSION "v0.0.0-${APP_VERSION}")
endif()

message(STATUS "App version set to: ${APP_VERSION}")

message(STATUS "Git describe output: ${APP_VERSION}")
message(STATUS "Git describe result: ${GIT_RESULT}")
message(STATUS "Git describe error: ${GIT_ERROR}")

if (NOT APP_VERSION)
  set(APP_VERSION "v0.0.0-unknown")
  message(WARNING "Failed to determine VERSION from Git tags. Using default version \"${APP_VERSION}\".")
  message(WARNING "Git result: ${GIT_RESULT}")
  message(WARNING "Git error: ${GIT_ERROR}")
endif()

configure_file(
        ${CMAKE_SOURCE_DIR}/version.h.in
        ${CMAKE_BINARY_DIR}/version.h
        @ONLY
)

add_custom_target(generate_version_header DEPENDS ${VERSION_HEADER})


# main directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/app)
set(HAL_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/STM32L4xx_HAL_Driver)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS)
set(STARTUP_DIR ${CMAKE_SOURCE_DIR}/startup)
set(FREERTOS_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Middlewares/Third_Party/FreeRTOS/Source)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32L432KCUX_FLASH.ld)

message(STATUS "SRC_DIR = ${SRC_DIR}")
message(STATUS "HAL_DIR = ${HAL_DIR}")
message(STATUS "CMSIS_DIR = ${CMSIS_DIR}")
message(STATUS "STARTUP_DIR = ${STARTUP_DIR}")
message(STATUS "LINKER_SCRIPT = ${LINKER_SCRIPT}")
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

set(HAL_INCLUDES
        ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/STM32L4xx_HAL_Driver/Inc
        ${STARTUP_DIR}/Inc
        ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS/Device/ST/STM32L4xx/Include
        ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS/Include
        ${CMAKE_BINARY_DIR}
)

# common flags for compilation and linking
set(COMMON_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
)

# add subdirectory for application sources
add_subdirectory(src)

# add source libraries (HAL e CMSIS)
include(cmake/hal_sources.cmake)
include(cmake/cmsis_headers.cmake)
include(cmake/freertos_sources.cmake)

# important files for startup and system initialization
set(STARTUP_FILE
        ${STARTUP_DIR}/Src/startup_stm32l432kcux.s
)
set(SYSTEM_FILE
        ${STARTUP_DIR}/Src/system_stm32l4xx.c
)
set(SYSCALL_FILES
        ${STARTUP_DIR}/Src/syscalls.c
        ${STARTUP_DIR}/Src/sysmem.c
)

# main executable target
add_executable(${PROJECT_NAME}
        $<TARGET_OBJECTS:app_sources>
        $<TARGET_OBJECTS:hal_sources>
        $<TARGET_OBJECTS:freertos_sources>
        ${SYSTEM_FILE}
        ${STARTUP_FILE}
        ${SYSCALL_FILES}
)


add_dependencies(${PROJECT_NAME} generate_version_header)

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

# compilation includes
target_include_directories(${PROJECT_NAME} PRIVATE
        ${SRC_DIR}
        ${HAL_DIR}/Inc
        ${CMSIS_DIR}/Include
        ${CMSIS_DIR}/Device/ST/STM32L4xx/Include
        ${STARTUP_DIR}/Inc
        ${CMAKE_BINARY_DIR}
)

# links CMSIS headers
target_link_libraries(${PROJECT_NAME} PRIVATE cmsis_headers)

# adjust linker language to C
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

# Pos-build: create bin and hex files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> firmware.bin
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> firmware.hex
)
