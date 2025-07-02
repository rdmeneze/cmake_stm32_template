cmake_minimum_required(VERSION 3.21)
file(GLOB_RECURSE
  APP_SOURCES app/*.cpp
)
add_library(app_sources OBJECT ${APP_SOURCES})
target_include_directories(app_sources PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${HAL_INCLUDES}
)
target_compile_definitions(app_sources PUBLIC STM32L432xx)
target_compile_options(app_sources PRIVATE ${COMMON_FLAGS})

