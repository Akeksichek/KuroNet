#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <mutex>
#include <vector>
#include <thread>

#include "core/logger.hpp"

namespace kuro {

    /**
     * @brief Структура данных клиента
     */
    struct Client
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket;
        std::string id;
        std::string login;
    };

    /**
     * @class ClientSessionManager
     * @brief Менеджер клиентских сессий.
     * 
     * Здесь хранятся все подключённые клиенты:
     * - Добавляет новых (`add_client()`)
     * - Удаляет отключившихся (`remove_client()`)
     * 
     * @note Использует мьютекс для контроля потока.
     *       Логирует все события.
     */
    class ClientSessionManager {
        private:
            std::unordered_map<std::string, Client> clients_;
            std::mutex clients_mutex_;
            
        public:
            void add_client(Client&& client) {
                std::lock_guard<std::mutex> lock(clients_mutex_);
                clients_.emplace(client.id, std::move(client));
            }
        
            void remove_client(const std::string& id) {
                std::lock_guard<std::mutex> lock(clients_mutex_);
                clients_.erase(id);
            }
        };

    /**
     * @brief Главный управляющий класс для работы с асинхронными операциями
     * 
     * Содержит общий `io_context`, который используется всеми компонентами KuroNet.
     * Реализован как Singleton, чтобы гарантировать единую точку управления.
     * 
     * Пример использования:
     * @code
     * auto& session = Session::get_instance();
     * session.ioc_.run(); // Запуск обработки асинхронных операций
     * @endcode
     */
    class Session {
    private:
        Session() = default; // Приватный конструктор (Singleton)

    public:
        /**
            * @brief Получить экземпляр Session (Singleton)
            * @return Ссылка на единственный экземпляр
            */
        static Session& get_instance() {
            static Session instance;
            return instance;
        }

        boost::asio::io_context ioc_;
        ClientSessionManager manager;


        Session(const Session&) = delete;
        Session& operator=(const Session&) = delete;
    };

} // namespace kuro