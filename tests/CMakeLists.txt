include(CheckCompilerFlag)
include(FetchContent)
include(FindSanitizers)

# Catch2
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test_bufreader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_bufwriter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_container.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_file.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_float.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_int.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_ref.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_varint.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_vector.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.h"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

# Test suite.
add_executable(lexio_test ${TEST_SOURCES})
target_include_directories(lexio_test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(lexio_test PRIVATE lexio GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lexio_test)

# Only use clang-tidy with Clang.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT MSVC)
    find_program(CLANG_TIDY_EXE clang-tidy)
    if(CLANG_TIDY_EXE)
        set_target_properties(lexio_test PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif()

check_compiler_flag(CXX -Wmost W_MOST)
if(W_MOST)
    target_compile_options(lexio_test PRIVATE "-Wmost")
endif()

check_compiler_flag(CXX -Werror=c++17-extensions WERROR_CXX17_EXTENSIONS)
if(WERROR_CXX17_EXTENSIONS)
    target_compile_options(lexio_test PRIVATE "-Werror=c++17-extensions")
endif()

check_compiler_flag(CXX -Werror=c++20-extensions WERROR_CXX20_EXTENSIONS)
if(WERROR_CXX20_EXTENSIONS)
    target_compile_options(lexio_test PRIVATE "-Werror=c++20-extensions")
endif()

if(MSVC)
    target_compile_options(lexio_test PUBLIC /W4 /permissive-)
else()
    target_compile_options(lexio_test PUBLIC -Wall -Wextra -Wno-unknown-pragmas)
endif()

option(CODE_COVERAGE "Enable code coverage on test suite" OFF)
if(CODE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        include(CodeCoverage)
        append_coverage_compiler_flags_to_target(lexio_test)
        setup_target_for_coverage_lcov(
            NAME lexio_test_cov
            EXECUTABLE lexio_test
            DEPENDENCIES lexio_test
            EXCLUDE "build/*" "tests" "/usr/include/*"
            LCOV_ARGS --ignore-errors mismatch)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        include(LLVMCov)
        append_llvmcov_compiler_flags_to_target(lexio_test)
        setup_target_for_coverage_llvmcov(
            NAME lexio_test_cov
            EXECUTABLE lexio_test
            DEPENDENCIES lexio_test
            SOURCES ${LEXIO_HEADERS})
    else()
        message(FATAL_ERROR "CODE_COVERAGE not supported by compiler")
    endif()
endif()
