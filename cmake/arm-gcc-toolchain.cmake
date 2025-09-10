set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# define toolchain path
if (DEFINED ENV{ARM_GCC_TOOLCHAIN_PATH})
    set(ARM_TOOLCHAIN_PATH $ENV{ARM_GCC_TOOLCHAIN_PATH})
else()
  # default path if environment variable is not set
  message(WARNING "Environment variable ARM_GCC_TOOLCHAIN_PATH is not set. Using default")
  set(ARM_TOOLCHAIN_PATH "/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi")
endif()

message(STATUS "Using ARM toolchain path: ${ARM_TOOLCHAIN_PATH}")

set(CMAKE_C_COMPILER ${ARM_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${ARM_TOOLCHAIN_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${ARM_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc)

set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_PATH}/bin/arm-none-eabi-objcopy CACHE INTERNAL "objcopy tool")

set(CMAKE_C_COMPILER_WORKS TRUE CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER_WORKS TRUE CACHE INTERNAL "")
