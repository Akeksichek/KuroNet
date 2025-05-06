cmake_minimum_required(VERSION 3.5)
project(MyProject VERSION 0.6.0)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

# Core library
add_library(kuro_core STATIC 
    src/core/logger.cpp
    src/core/UniqueUtils.cpp
    src/core/CommandHandler.cpp
)
target_include_directories(kuro_core PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

# Net library
add_library(kuro_net STATIC
    src/net/TCPServer.cpp
    src/net/Session.cpp
    src/net/Chat/Chat.cpp
)
target_include_directories(kuro_net PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Commands library
add_library(kuro_commands STATIC
    src/core/commands/token_command.cpp
    src/core/commands/clients_command.cpp
)
target_include_directories(kuro_commands PUBLIC
    ${CMAKE_SOURCE_DIR}/include/commands
)


target_link_libraries(kuro_commands PRIVATE kuro_core)
target_link_libraries(kuro_net PRIVATE kuro_core kuro_commands)

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()