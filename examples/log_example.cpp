#include "core/logger.hpp"

int main() {
    kuro::Logger::set_level(kuro::Logger::Level::Debug);
    kuro::Logger::log(kuro::Logger::Level::Error, "Тест ошибки (должен быть красным)");
    kuro::Logger::log(kuro::Logger::Level::Warning, "Тест предупреждения");
    return 0;
}