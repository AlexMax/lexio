cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
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

add_executable(test "${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp" ${LEXIO_HEADERS})
set_property(TARGET test PROPERTY CXX_STANDARD 20)

if(MSVC)
    target_compile_options(test PUBLIC /W4)
else()
    target_compile_options(test PUBLIC -Wall -Wextra)
endif()
target_include_directories(test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(test PUBLIC Catch2::Catch2WithMain)

# target_compile_options(Catch2 PUBLIC "$<$<CONFIG:Debug>:-fsanitize=address>")
# target_compile_options(Catch2WithMain PUBLIC "$<$<CONFIG:Debug>:-fsanitize=address>")
# target_compile_options(test PUBLIC "$<$<CONFIG:Debug>:-fsanitize=address>")
