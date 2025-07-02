add_library(cmsis_headers INTERFACE)
target_include_directories(cmsis_headers INTERFACE
        ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS/Include
        ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS/Core/Include    # <---- ADD ESTA LINHA
        ${CMAKE_SOURCE_DIR}/third_party/STM32CubeL4/Drivers/CMSIS/Device/ST/STM32L4xx/Include
)
