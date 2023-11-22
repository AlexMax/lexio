include(CheckCXXCompilerFlag)

set(LEXIO_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(DOCTEST_INCLUDE "${CMAKE_CURRENT_BINARY_DIR}/include")
set(DOCTEST_ARCHIVE "${CMAKE_CURRENT_SOURCE_DIR}/doctest.h.zip")

file(MAKE_DIRECTORY "${DOCTEST_INCLUDE}")
add_custom_command(OUTPUT "${DOCTEST_INCLUDE}/doctest.h"
    COMMAND "${CMAKE_COMMAND}" -E tar xf "${DOCTEST_ARCHIVE}" --format=zip
    WORKING_DIRECTORY "${DOCTEST_INCLUDE}"
    VERBATIM)
add_custom_target(doctest_header DEPENDS "${DOCTEST_INCLUDE}/doctest.h")

add_library(doctest INTERFACE "${DOCTEST_INCLUDE}/doctest.h")
add_dependencies(doctest doctest_header)
target_include_directories(doctest INTERFACE "${DOCTEST_INCLUDE}")

# Test suite.
add_executable(test
    test_core.cpp
    test_container.cpp
    test_file.cpp
    test_main.cpp
    test_ref.cpp
    test_serialize.cpp
    test.h)
target_link_libraries(test lexio doctest)

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
