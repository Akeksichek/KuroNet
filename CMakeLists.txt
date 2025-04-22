cmake_minimum_required(VERSION 3.5)
project(MyProject VERSION 0.4.0)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

add_library(kuro_core STATIC 
    src/core/logger.cpp
    src/core/UniqueUtils.cpp
)
target_include_directories(kuro_core PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

add_library(kuro_net STATIC
    src/net/TCPServer.cpp
    src/net/Session.cpp
)
target_include_directories(kuro_net PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(kuro_net PRIVATE
    kuro_core
)

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()