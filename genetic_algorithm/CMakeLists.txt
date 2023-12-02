cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 20)
project(tsp_genetic_algo VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake)

enable_testing()

include_directories(
    include
)

add_executable(run
    src/graph_info.cpp
    src/utils_impl.cpp
    src/genetic_algo.cpp
    src/random.cpp
    src/graph_generator.cpp
    src/solver.cpp
    main.cpp
    )

add_subdirectory(tests)
