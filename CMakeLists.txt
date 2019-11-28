cmake_minimum_required(VERSION 3.12)

project(parser-test LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} cmake")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/vendor.cmake)
add_subdirectory(modules)