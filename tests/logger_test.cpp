#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "../include/core/logger.hpp"

TEST_CASE("Logger level filtering", "[logger]") {
    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    kuro::Logger::set_level(kuro::Logger::Level::Warning);
    kuro::Logger::log(kuro::Logger::Level::Info, "INFO test");
    REQUIRE(oss.str().empty());

    kuro::Logger::log(kuro::Logger::Level::Error, "ERROR test");
    REQUIRE(oss.str().find("ERROR") != std::string::npos);

    std::cout.rdbuf(old_cout);
    }

    TEST_CASE("Logger colors", "[logger]") {
}