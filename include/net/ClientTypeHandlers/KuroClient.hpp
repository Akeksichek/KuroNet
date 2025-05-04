#pragma once

#include <functional>
#include <string>
#include <array>

namespace kuro {
    namespace detectors {

        /**
        * @class KuroClient
        * @brief Обработчик кастомного протокола для клиентов KuroNet
        * 
        * ## Основные функции:
        * 1. **Безопасное общение**:
        *    - Автоматическая подпись сообщений (HMAC-SHA256)
        *    - Валидация входящих данных
        *    - Защита от replay-атак
        * 
        * 2. **Эффективный протокол**:
        *    - Бинарная сериализация сообщений
        *    - Поддержка потоковой передачи
        *    - Сжатие данных (LZ4)
        * 
        * @note Клиент НЕ может:
        * - Сгенерировать токен самостоятельно(только уведомить админа/хоста о выдаче нового)
        * - Продлить срок действия токена
        * - Изменить уровень доступа токена
        */
        class KuroClient
        {
        private:
            
            /**
             * @struct KuroClientMessage 
            */
            struct KuroClientMessage {
                std::array<char, 16> client_id;
                std::array<char, 32> session_token;
                
                std::array<char, 16> server_ip;
                u_int16_t server_port;
                
                std::string content;
                u_int32_t payload_size;
                
                std::array<u_int8_t, 32> signature;
                std::array<char, 6> end_marker = {'\\','x','K','E','N','D'};
            };

            bool is_message_valid(const KuroClientMessage& msg) const;

            std::vector<u_int8_t> serialize_message(const KuroClientMessage& msg) const;
            KuroClientMessage deserialize_message(const std::vector<u_int8_t>& raw_data) const;

        public:
            KuroClient();
            ~KuroClient();

            void request_processing();
            KuroClientMessage response_generation();
            void response_generate_and_auto_send();

        };

    } // namespace detectors
} // namespace kuro