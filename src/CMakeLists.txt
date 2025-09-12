cmake_minimum_required(VERSION 3.21)

# Collect application sources
file(GLOB_RECURSE APP_SOURCES app/*.cpp app/*.c)

# Create app sources library
add_library(app_sources OBJECT ${APP_SOURCES})
target_include_directories(app_sources PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${HAL_INCLUDES}
    ${FREERTOS_INCLUDES}
)
target_compile_definitions(app_sources PUBLIC 
    ${MCU_DEFINES}
    ${FREERTOS_DEFINITIONS}
)
target_compile_options(app_sources PRIVATE ${COMMON_FLAGS})

