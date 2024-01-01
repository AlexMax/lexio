include(FetchContent)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

set(BENCH_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/bench.cpp")

# Test suite.
add_executable(lexio_bench ${BENCH_SOURCES})
target_link_libraries(lexio_bench PRIVATE lexio benchmark::benchmark)
