#pragma once

#include <string>
#include <functional>
#include <random>
#include <algorithm>

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
         * @brief Метод генерирующий строку опеределенной длинны
         * 
         * Генерирует строку рандомных символов, длинна строки по умолчанию 16
         * Не проверяет существует ли похожий ключ!
         * 
         * @param length - длинна генерируемой строки
         */
        static std::string generate(int length = IDGenerator::default_length_);

    };
}