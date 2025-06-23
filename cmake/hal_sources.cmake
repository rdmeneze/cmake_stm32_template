file(GLOB HAL_SOURCES ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/STM32L4xx_HAL_Driver/Src/*.c)
add_library(hal_sources OBJECT ${HAL_SOURCES})

target_include_directories(hal_sources PUBLIC
  ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/STM32L4xx_HAL_Driver/Inc
  ${STARTUP_DIR}/Inc
  ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS/Device/ST/STM32L4xx/Include
)

target_link_libraries(hal_sources PRIVATE cmsis_headers)
target_compile_definitions(hal_sources PUBLIC STM32L432xx)
target_compile_options(hal_sources PRIVATE ${COMMON_FLAGS})
