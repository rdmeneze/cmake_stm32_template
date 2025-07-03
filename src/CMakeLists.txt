cmake_minimum_required(VERSION 3.21)
file(GLOB_RECURSE
  APP_SOURCES app/*.cpp app/*.c
)
add_library(app_sources OBJECT ${APP_SOURCES})
target_include_directories(app_sources PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/app
        ${HAL_INCLUDES}
        ${FREERTOS_DIR}/include
        ${FREERTOS_DIR}/portable/GCC/ARM_CM4F
)
target_compile_definitions(app_sources PUBLIC STM32L432xx)
target_compile_options(app_sources PRIVATE ${COMMON_FLAGS})

