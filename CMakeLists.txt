cmake_minimum_required(VERSION 3.22)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(FindSanitizers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD "20" CACHE INTERNAL "")
set(LEXIO_TEST_NONSTD "OFF" CACHE BOOL "Test using nonstd::span")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(USE_FOLDERS ON)

project(lexio LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.3.1)
FetchContent_MakeAvailable(Catch2)

set(LEXIO_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio_core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio_serialize.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio_std.h")

add_executable(test
    "${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_nonstd.h"
     ${LEXIO_HEADERS})

if(LEXIO_TEST_NONSTD)
    target_compile_definitions(test PUBLIC LEXIO_TEST_NONSTD)
endif()

find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    set_target_properties(test PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
endif()

if(MSVC)
    target_compile_options(test PUBLIC /W4)
else()
    target_compile_options(test PUBLIC -Wall -Wextra -Wno-unknown-pragmas)
endif()

find_package(Sanitizers)
add_sanitizers(test)

target_include_directories(test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(test PUBLIC Catch2::Catch2WithMain)

if(NOT LEXIO_TEST_NONSTD)
    set(LEXIO_TEST_NONSTD OFF)
endif()

message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
if(CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "LEXIO_TEST_NONSTD: ${LEXIO_TEST_NONSTD}")
message(STATUS "SANITIZE_ADDRESS: ${SANITIZE_ADDRESS}")
message(STATUS "SANITIZE_UNDEFINED: ${SANITIZE_UNDEFINED}")
