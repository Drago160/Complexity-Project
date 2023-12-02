cmake_minimum_required(VERSION 3.6)
project(gtest)

enable_testing()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all -Wall -Werror")


include_directories(
     ${CMAKE_SOURCE_DIR}/include
)

set(TESTING_FILES
     basic_tests.cpp
)

add_executable(testing
     ${TESTING_FILES}
     ${CMAKE_SOURCE_DIR}/src/graph_info.cpp
     ${CMAKE_SOURCE_DIR}/src/utils_impl.cpp
     ${CMAKE_SOURCE_DIR}/src/genetic_algo.cpp
     ${CMAKE_SOURCE_DIR}/src/random.cpp
     )

target_link_libraries(testing gtest gtest_main)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)
