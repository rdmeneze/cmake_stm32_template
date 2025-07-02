execute_process(
        COMMAND "${GIT_EXECUTABLE}" describe --tags --always --dirty
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(APP_VERSION "${GIT_VERSION}")

message(STATUS "Generating version header with APP_VERSION=${APP_VERSION}")

configure_file(
        "${SRC}" "${DST}"
        @ONLY
)




# if (GIT_EXECUTABLE)
#   get_filename_component(SRC_DIR "${SRC}" DIRECTORY)
#
#   # Generate a git-describe version string from Git repository tags
#   execute_process(
#     COMMAND ${GIT_EXECUTABLE} describe --tags --long --dirty --match="v*"
#     WORKING_DIRECTORY ${SRC_DIR}
#     OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
#     RESULT_VARIABLE GIT_DESCRIBE_ERROR_CODE
#     OUTPUT_STRIP_TRAILING_WHITESPACE
#   )
#
#   message(STATUS "Git describe version= ${GIT_DESCRIBE_VERSION}")
#   message(STATUS "Git describe error code= ${GIT_DESCRIBE_ERROR_CODE}")
#
#   if (NOT GIT_DESCRIBE_ERROR_CODE)
#     set(APP_VERSION ${GIT_DESCRIBE_VERSION})
#   endif()
# endif()
#
# if(NOT DEFINED APP_VERSION)
#   set(APP_VERSION v0.0.0-unknown)
#   message(WARNING "Failed to determine VERSION from Git tags. Using default version \"${APP_VERSION}\".")
# endif()
#
#
# file(READ "${SRC}" VERSION_TEMPLATE)
# string(REPLACE "@GIT_TAG@" "${APP_VERSION}" VERSION_CONTENT "${VERSION_TEMPLATE}")
# file(WRITE "${DST}" "${VERSION_CONTENT}")
#
# message(STATUS "Generated version header at ${DST}")
