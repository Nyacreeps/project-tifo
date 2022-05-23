cmake_minimum_required (VERSION 3.14)
project (OIL_FILTER CUDA CXX)

include(FetchContent)

FetchContent_Declare(
        GoogleBenchmark
        URL https://github.com/google/benchmark/archive/v1.5.2.tar.gz
)

FetchContent_Declare(
        CLI11
        URL https://github.com/CLIUtils/CLI11/archive/v1.9.1.tar.gz
)

FetchContent_Declare(
        spdlog
        URL https://github.com/gabime/spdlog/archive/v1.9.2.tar.gz
)


if (NOT GoogleBenchmark_POPULATED)
    FetchContent_Populate(GoogleBenchmark)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "From Gtest")
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "From Gtest")
    add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR})
endif()

FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(CLI11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR} )
find_package(PNG REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic -std=c++20 -O0 -g -fopenmp")

add_library(oil-filter)
target_compile_features(oil-filter PUBLIC)
target_link_libraries(oil-filter PRIVATE spdlog::spdlog)
target_sources(oil-filter PRIVATE src/filter.cu)
target_include_directories(oil-filter PRIVATE src)
set_target_properties(oil-filter PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

add_executable(render)
target_sources(render PRIVATE src/render.cc src/io_png.cc src/video.cc)
target_include_directories(render PRIVATE src)
target_compile_features(render PUBLIC)
target_link_libraries(render PRIVATE PNG::PNG CLI11::CLI11 spdlog::spdlog oil-filter)

add_executable(bench)
target_sources(bench PRIVATE src/bench.cc src/io_png.cc src/video.cc)
target_include_directories(bench PRIVATE src)
target_compile_features(bench PUBLIC)
target_link_libraries(bench PRIVATE PNG::PNG CLI11::CLI11 spdlog::spdlog benchmark oil-filter)
