#pragma once

#include <string>
#include <functional>
#include <random>
#include <algorithm>

#include <core/logger.hpp>

namespace kuro
{
    namespace kuro_generator {
        std::string random_generate_str(int length);
    }

    class IDGenerator
    {
    private:
        static int default_length_;

    public:
        /**
         * @brief Метод генерирующий токен опеределенной длинны
         * 
         * Генерирует токен из типа, рандомных символов и подписи(хэша)
         * Не проверяет существует ли похожий ключ!
         * 
         * @param length - длинна генерируемой строки без учета типа и хэша
         */
        static std::string generate(int length = IDGenerator::default_length_);

    };

    class TokenHandler
    {        
    public:
        enum class TokenType { PermanentToken, SessionToken, TemporaryToken };
        static std::string generate(const TokenHandler::TokenType& token);
    };
}