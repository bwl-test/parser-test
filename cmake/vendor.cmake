execute_process(
    COMMAND git submodule update --init
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor
)

include(vendor/lexertl14.cmake)
add_subdirectory(vendor/catch2)