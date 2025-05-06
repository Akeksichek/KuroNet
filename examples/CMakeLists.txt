set(KURO_EXAMPLES
    tcp_server_example
    log_example
    commands_example
    chat_example
)

foreach(example_name ${KURO_EXAMPLES})
    add_executable(${example_name} ${example_name}.cpp)
    target_link_libraries(${example_name} PRIVATE kuro_net kuro_core)
endforeach()