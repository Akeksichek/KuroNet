# Опциональная сборка примеров
option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_EXAMPLES)
    add_executable(log_example log_example.cpp)
    target_link_libraries(log_example PRIVATE kuro_logger)
    target_include_directories(log_example PRIVATE ${CMAKE_SOURCE_DIR}/include)
endif()