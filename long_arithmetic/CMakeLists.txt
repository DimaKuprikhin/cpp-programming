cmake_minimum_required(VERSION 3.10)

project(long_arithmetic)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-g -Wall -std=c++17)

add_executable(long_arithmetic
    main.cpp
    long_integer.cpp
)

add_subdirectory(./googletest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(
    long_integer_test
    long_integer_test.cpp
    long_integer.cpp
)

target_link_libraries(
    long_integer_test
    gtest gtest_main
)

add_test(UnitTests long_integer_test)
