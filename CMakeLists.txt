cmake_minimum_required(VERSION 3.21)

# Define o nome do projeto e linguagens usadas
project(stm32l432_firmware C CXX ASM)

# Padroniza versões das linguagens
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Diretórios principais
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/app)
set(HAL_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/STM32L4xx_HAL_Driver)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS)
set(STARTUP_DIR ${CMAKE_SOURCE_DIR}/startup)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32L432KCUX_FLASH.ld)

message(STATUS "SRC_DIR = ${SRC_DIR}")
message(STATUS "HAL_DIR = ${HAL_DIR}")
message(STATUS "CMSIS_DIR = ${CMSIS_DIR}")
message(STATUS "STARTUP_DIR = ${STARTUP_DIR}")
message(STATUS "LINKER_SCRIPT = ${LINKER_SCRIPT}")

set(HAL_INCLUDES
        ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/STM32L4xx_HAL_Driver/Inc
        ${STARTUP_DIR}/Inc
        ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS/Device/ST/STM32L4xx/Include
        ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS/Include
)


# Flags comuns para compilação e link
set(COMMON_FLAGS
  -mcpu=cortex-m4
  -mthumb
  -mfpu=fpv4-sp-d16
  -mfloat-abi=hard
)

# Incluir subdiretório para app (src)
add_subdirectory(src)

# Incluir bibliotecas de sources (HAL e CMSIS)
include(cmake/hal_sources.cmake)
include(cmake/cmsis_headers.cmake)

# Arquivos importantes
set(STARTUP_FILE ${STARTUP_DIR}/Src/startup_stm32l432kcux.s)
set(SYSTEM_FILE ${STARTUP_DIR}/Src/system_stm32l4xx.c)
set(SYSCALL_FILES
  ${STARTUP_DIR}/Src/syscalls.c
  ${STARTUP_DIR}/Src/sysmem.c
)

# Executável principal
add_executable(${PROJECT_NAME}
  $<TARGET_OBJECTS:app_sources>
  $<TARGET_OBJECTS:hal_sources>
  ${SYSTEM_FILE}
  ${STARTUP_FILE}
  ${SYSCALL_FILES}
)

# Definições de compilação
target_compile_definitions(${PROJECT_NAME} PRIVATE
  STM32L432xx
  USE_HAL_DRIVER
)

# Opções de compilação
target_compile_options(${PROJECT_NAME} PRIVATE
  ${COMMON_FLAGS}
  -Wall
  -Wextra
  -ffunction-sections
  -fdata-sections
)

# Opções do linker
target_link_options(${PROJECT_NAME} PRIVATE
  ${COMMON_FLAGS}
  -T${LINKER_SCRIPT}
  -Wl,--gc-sections
  -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
)

# Includes para compilação
target_include_directories(${PROJECT_NAME} PRIVATE
  ${SRC_DIR}
  ${HAL_DIR}/Inc
  ${CMSIS_DIR}/Include
  ${CMSIS_DIR}/Device/ST/STM32L4xx/Include
  ${STARTUP_DIR}/Inc
)

# Linka a interface de headers CMSIS
target_link_libraries(${PROJECT_NAME} PRIVATE cmsis_headers)

# Ajusta linguagem do linker para C
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

# Pós-build: gera arquivos bin e hex
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> firmware.bin
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> firmware.hex
)
