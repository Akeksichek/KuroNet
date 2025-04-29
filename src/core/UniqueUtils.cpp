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

    int TokenHandler::token_time = 0;
    void TokenHandler::set_time_token(const std::string& unit, const std::string& time) {
        int value = std::stoi(time);
        if(unit == "-minute") token_time = value * 60;
        else if(unit == "-hour") token_time = value * 60 * 60;
        else if(unit == "-day") token_time = value * 60 * 60 * 24;
        else throw std::invalid_argument("Unknown time unit");
    }
    std::string TokenHandler::generate(const TokenHandler::TokenType& token)
    {
        std::string res_token;
        std::string type_token;
        std::string tt_time;
        switch (token)
        {
            case TokenType::PermanentToken :
                type_token = "perm";
                    break;
            case TokenType::SessionToken :
                type_token = "sess";
                    break;
            case TokenType::TemporaryToken :
                auto now = std::chrono::steady_clock::now();
                auto expiry_time = now + std::chrono::seconds(token_time);
                int expiry_timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                    expiry_time.time_since_epoch()).count();

                tt_time = std::to_string(expiry_timestamp);
                type_token = "temp";
                    break;
        // default:
        //     break;
        }

        std::string random_part = kuro_generator::random_generate_str(10);
        size_t hash = std::hash<std::string>{}(random_part);
        std::string signature = std::to_string(hash);

        res_token = type_token + "." + (type_token == "temp" ? tt_time + "." : "") + random_part + "." + signature;

        Logger::log(Logger::Level::Info, "New token was created send this token your teammate: " + res_token);

        return res_token;
    }

}