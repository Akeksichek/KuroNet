include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(logger_test logger_test.cpp)
target_link_libraries(logger_test PRIVATE 
    kuro_logger
    Catch2::Catch2WithMain
)

enable_testing()
add_test(NAME logger_test COMMAND logger_test)