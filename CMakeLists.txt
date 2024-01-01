cmake_minimum_required(VERSION 3.22)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD "20" CACHE STRING "C++ Standard to use")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(lexio LANGUAGES C CXX)

set(LEXIO_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/bufreader.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/bufwriter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/core.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/lexio.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/ref.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/serialize.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/stream.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/serialize/float.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/serialize/int.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/serialize/tryfloat.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/serialize/tryint.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/serialize/tryvarint.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/serialize/varint.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/stream/array.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/stream/container.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/stream/file.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/lexio/stream/vector.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${LEXIO_HEADERS})

# Main LexIO header-only library
add_library(lexio INTERFACE ${LEXIO_HEADERS})
target_include_directories(lexio INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_subdirectory(tests)
add_subdirectory(benches)

message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
if(CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "CLANG_TIDY_EXE: ${CLANG_TIDY_EXE}")
message(STATUS "SANITIZE_ADDRESS: ${SANITIZE_ADDRESS}")
message(STATUS "SANITIZE_UNDEFINED: ${SANITIZE_UNDEFINED}")

### Documentation ###
find_package(Doxygen)
if(TARGET Doxygen::doxygen)
    doxygen_add_docs(docs ${LEXIO_HEADERS})
endif()
