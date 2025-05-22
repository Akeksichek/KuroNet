#include <core/System/SystemConfig.hpp>

int main()
{
    kuro::Logger::set_level(kuro::Logger::Level::Debug);

    int port = kuro::System::SystemConfig::get_parameter_value_int("server.port");
    std::string key = kuro::System::SystemConfig::get_parameter_value_str("security.secret_key");

    kuro::Logger::log(kuro::Logger::Level::Debug, std::to_string(port));
    kuro::Logger::log(kuro::Logger::Level::Debug, key);

    return 0;
}