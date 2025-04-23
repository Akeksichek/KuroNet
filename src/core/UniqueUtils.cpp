#include "core/UniqueUtils.hpp"

namespace kuro
{
    
    std::string kuro_generator::random_generate_str(int length)
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

    int IDGenerator::default_length_ = 16;
            
    std::string IDGenerator::generate(int length)
    {
        std::string id = kuro_generator::random_generate_str(length);
        return id;
    }

    // enum class TokenType { PermanentToken, SessionToken, TemporaryToken };
    std::string TokenHandler::generate(const TokenHandler::TokenType& token)
    {
        std::string res_token;
        std::string type_token;
        switch (token)
        {
            case TokenType::PermanentToken :
                type_token = "perm_";
                    break;
            case TokenType::SessionToken :
                type_token = "sess_";
                    break;
            case TokenType::TemporaryToken :
                type_token = "temp_";
                    break;
        default:
            break;
        }

        std::string random_part = kuro_generator::random_generate_str(10);
        size_t hash = std::hash<std::string>{}(random_part);
        std::string signature = std::to_string(hash);

        res_token = type_token + "." + random_part + "." + signature;

        Logger::log(Logger::Level::Info, "New token was created send this token your teammate: " + res_token);

        return res_token;
    }

}