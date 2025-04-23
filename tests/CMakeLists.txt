include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

set(KURO_TESTS
    logger_test
)

foreach(test_name ${KURO_TESTS})
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE kuro_net kuro_core Catch2::Catch2WithMain)
    add_test(NAME test_name COMMAND test_name)
endforeach()