# Google Test/GMock configuration for unit testing
cmake_minimum_required(VERSION 3.21)

# Only enable testing when not cross-compiling for ARM
if(NOT CMAKE_CROSSCOMPILING)
    message(STATUS "Configuring Google Test for host testing...")

    # Download and build Google Test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0  # Latest stable version
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Disable installation of Google Test
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    # Enable testing
    enable_testing()

    message(STATUS "Google Test configured successfully")
else()
    message(STATUS "Cross-compiling detected - skipping Google Test configuration")
endif()
