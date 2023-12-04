include(CheckCXXCompilerFlag)
include(FetchContent)

option(SANITIZE_ADDRESS "Enable AddressSanitizer for sanitized targets." OFF)

# Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
include(CTest)
include(Catch)

set(LEXIO_EXTRA_FLAGS "")
check_cxx_compiler_flag(-Wmost W_MOST) # clang
if(W_MOST)
    list(APPEND LEXIO_EXTRA_FLAGS "-Wmost")
endif()
if(SANITIZE_ADDRESS)
    check_cxx_compiler_flag(-fsanitize=address F_SANITIZE_ADDRESS)
    if(F_SANITIZE_ADDRESS)
        if(MSVC)
            target_compile_options(Catch2 PUBLIC "/fsanitize=address")
            target_compile_definitions(Catch2 PUBLIC "_DISABLE_VECTOR_ANNOTATION")
            target_compile_definitions(Catch2 PUBLIC "_DISABLE_STRING_ANNOTATION")
        else()
            target_compile_options(Catch2 PUBLIC "-fsanitize=address")
        endif()
    endif()
endif()

set(TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test_bytes.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_container.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_file.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_float.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_int.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_ref.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_varint.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.h"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

# Test suite.
add_executable(test ${TEST_SOURCES})
target_include_directories(test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(test lexio Catch2::Catch2WithMain)
catch_discover_tests(test)

# Only use clang-tidy with Clang.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT MSVC)
    find_program(CLANG_TIDY_EXE clang-tidy)
    if(CLANG_TIDY_EXE)
        set_target_properties(test PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif()

if(MSVC)
    target_compile_options(test PUBLIC ${LEXIO_EXTRA_FLAGS} /W4 /D_DISABLE_VECTOR_ANNOTATION)
else()
    target_compile_options(test PUBLIC ${LEXIO_EXTRA_FLAGS} -Wall -Wextra -Wno-unknown-pragmas)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(test PUBLIC -stdlib=libc++)
    endif()
endif()

if(NOT LEXIO_TEST_NONSTD)
    set(LEXIO_TEST_NONSTD OFF)
endif()
