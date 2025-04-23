#include "core/UniqueUtils.hpp"

std::string kuro::kuro_generator::random_generate_str(int length)
{
    const std::string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, chars.size() - 1);

    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; ++i) {
        result += chars[dist(generator)];
    }

    return result;
}

int kuro::IDGenerator::default_length_ = 16;
        
std::string kuro::IDGenerator::generate(int length)
{
    std::string id = kuro::kuro_generator::random_generate_str(length);
    return id;
}