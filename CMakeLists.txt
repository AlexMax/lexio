cmake_minimum_required(VERSION 3.22)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(FindSanitizers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD "20" CACHE STRING "C++ Standard to use")
set(LEXIO_TEST_NONSTD "OFF" CACHE BOOL "Test using nonstd::span")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(lexio LANGUAGES CXX)

set(LEXIO_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/lexio.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/array.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/bufreader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/container.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/file.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/ref.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/serialize.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/tryserialize.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/vector.h")

# Main LexIO header-only library
add_library(lexio INTERFACE ${LEXIO_HEADERS})
target_include_directories(lexio INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_subdirectory(tests)

message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
if(CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "CLANG_TIDY_EXE: ${CLANG_TIDY_EXE}")
message(STATUS "LEXIO_TEST_NONSTD: ${LEXIO_TEST_NONSTD}")
message(STATUS "SANITIZE_ADDRESS: ${SANITIZE_ADDRESS}")
message(STATUS "SANITIZE_UNDEFINED: ${SANITIZE_UNDEFINED}")
