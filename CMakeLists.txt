cmake_minimum_required(VERSION 3.5)
project(KuroNet VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)

add_library(kuro_logger STATIC src/core/logger.cpp)
target_include_directories(kuro_logger PUBLIC include)

add_subdirectory(examples)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()