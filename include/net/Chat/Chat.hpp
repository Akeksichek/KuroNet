#pragma once

#include <core/logger.hpp>
#include <net/Session.hpp>

#include <atomic>
#include <thread>
#include <chrono>

#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>

namespace kuro {
    namespace chat {

        /**
         * @class Chat
         * @brief Chat, говорящее само за себя название клсса
         * 
         * Простой чат, не адаптирован для работы с русским языком, сокеты передают сырые байты без преобразования кодировки
         * ВАЖНО!!! при "вылете" пользователям, возможна утечка памяти
         * 
         */
        class Chat {
            private:
                std::atomic<bool> started{false};
            
                void do_read(const std::string& client_id, const Client& client);
                void do_send(const std::string& sender_id, const std::string& message);

                void work();
            
            public:
                Chat() = default;
                ~Chat() {
                    stop();
                }
                /**
                 * @brief запускает чат - прослушивание и отправку, ТОЛЬКО для принятых хостом пользователей - client approve user_id token
                 */
                void start();

                /**
                 * @brief останавливает работу чата, возможны задержки при работе, из-за выделения как минимум 2 буферов(чтения/записи)
                 * для каждого пользователя по отдельности
                 */
                void stop();
            };

    } // namespace chat
} // namespace kuro