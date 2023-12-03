include(CheckCXXCompilerFlag)
include(FetchContent)

# Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0)
FetchContent_MakeAvailable(Catch2)

set(LEXIO_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

# Test suite.
add_executable(test
    test_bytes.cpp
    test_core.cpp
    test_container.cpp
    test_file.cpp
    test_float.cpp
    test_int.cpp
    test_ref.cpp
    test_varint.cpp
    test.h)
target_link_libraries(test lexio Catch2::Catch2WithMain)

if(LEXIO_TEST_NONSTD)
    target_compile_definitions(test PUBLIC LEXIO_TEST_NONSTD)
endif()

# Only use clang-tidy with Clang.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    find_program(CLANG_TIDY_EXE clang-tidy)
    if(CLANG_TIDY_EXE)
        set_target_properties(test PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif()

set(LEXIO_EXTRA_FLAGS "")
check_cxx_compiler_flag(-Wmost W_MOST) # clang
if(W_MOST)
    list(APPEND LEXIO_EXTRA_FLAGS "-Wmost")
endif()

if(MSVC)
    target_compile_options(test PUBLIC ${LEXIO_EXTRA_FLAGS} /W4 /D_DISABLE_VECTOR_ANNOTATION)
else()
    target_compile_options(test PUBLIC ${LEXIO_EXTRA_FLAGS} -Wall -Wextra -Wno-unknown-pragmas)
endif()

find_package(Sanitizers)
add_sanitizers(test)

if(NOT LEXIO_TEST_NONSTD)
    set(LEXIO_TEST_NONSTD OFF)
endif()
