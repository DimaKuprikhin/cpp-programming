cmake_minimum_required(VERSION 3.10)

project(long_arithmetic)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-g -Wall -std=c++17)

add_executable(long_arithmetic
    src/expression_evaluator.cpp
    src/long_integer.cpp
    src/main.cpp
)
target_include_directories(long_arithmetic
    PRIVATE include
)

enable_testing()
add_executable(long_integer_test
    src/long_integer.cpp
    tests/long_integer_test.cpp
)
add_executable(expression_evaluator_test
    src/expression_evaluator.cpp
    src/long_integer.cpp
    tests/expression_evaluator_test.cpp
)
target_include_directories(long_integer_test
    PRIVATE
        ${gtest_SOURCE_DIR}
        ${gtest_SOURCE_DIR}/include
        ./include
)
target_include_directories(expression_evaluator_test
    PRIVATE
        ${gtest_SOURCE_DIR}
        ${gtest_SOURCE_DIR}/include
        ./include
)
target_link_libraries(long_integer_test
    gtest
    gtest_main
)
target_link_libraries(expression_evaluator_test
    gtest
    gtest_main
)
add_test(UnitTests expression_evaluator_test)
add_test(UnitTests long_integer_test)