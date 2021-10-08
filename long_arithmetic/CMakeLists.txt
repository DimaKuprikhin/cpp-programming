cmake_minimum_required(VERSION 3.10)

project(long_arithmetic)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-g -Wall -std=c++17)

add_executable(long_arithmetic
    src/main.cpp
    src/long_integer.cpp
)
target_include_directories(long_arithmetic
    PRIVATE include
)

enable_testing()
# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(long_integer_test
    tests/long_integer_test.cpp
    src/long_integer.cpp
)
target_include_directories(long_integer_test
    PRIVATE 
        include
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
)
target_link_libraries(long_integer_test
    gtest
    gtest_main
)
add_test(UnitTests long_integer_test)