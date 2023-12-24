include(CheckCompilerFlag)
include(FetchContent)
include(FindSanitizers)

# Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
include(CTest)
include(Catch)

set(TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test_bufreader.cpp"
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
target_link_libraries(test PRIVATE lexio Catch2::Catch2WithMain)
catch_discover_tests(test)

# Only use clang-tidy with Clang.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT MSVC)
    find_program(CLANG_TIDY_EXE clang-tidy)
    if(CLANG_TIDY_EXE)
        set_target_properties(test PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif()

check_compiler_flag(CXX -Wmost W_MOST)
if(W_MOST)
    target_compile_options(test PRIVATE "-Wmost")
endif()

check_compiler_flag(CXX -Werror=c++17-extensions WERROR_CXX17_EXTENSIONS)
if(WERROR_CXX17_EXTENSIONS)
    target_compile_options(test PRIVATE "-Werror=c++17-extensions")
endif()

check_compiler_flag(CXX -Werror=c++20-extensions WERROR_CXX20_EXTENSIONS)
if(WERROR_CXX20_EXTENSIONS)
    target_compile_options(test PRIVATE "-Werror=c++20-extensions")
endif()

add_sanitizers(Catch2)
if(MSVC AND SANITIZE_ADDRESS)
    target_compile_definitions(Catch2 PUBLIC "_DISABLE_VECTOR_ANNOTATION")
    target_compile_definitions(Catch2 PUBLIC "_DISABLE_STRING_ANNOTATION")
endif()

if(MSVC)
    target_compile_options(test PUBLIC /W4 /D_DISABLE_VECTOR_ANNOTATION)
else()
    target_compile_options(test PUBLIC -Wall -Wextra -Wno-unknown-pragmas)
endif()

option(CODE_COVERAGE "Enable code coverage on test suite" OFF)
if(CODE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags_to_target(test)
    setup_target_for_coverage_lcov(
        NAME test_coverage
        EXECUTABLE test
        DEPENDENCIES test)
endif()
