#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>

#include "core/logger.hpp"
#include <core/UniqueUtils.hpp>

namespace kuro {

    /**
     * @brief Структура данных клиента
     */
    struct Client
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket;
        std::string id;
        std::string login;
        std::string token = "";
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
        std::unordered_map<std::string, Client> wait_clients_;
        std::mutex clients_mutex_;

        std::vector<std::string> active_tokens_;
        std::mutex tokens_mtx_;
        
    public:
        bool approve_client(const std::string& user_id, const std::string& token);
        void add_client(Client&& client);

        template <typename F>
        void access_clients(F&& callback) {
            std::lock_guard<std::mutex> lock(clients_mutex_);
            callback(clients_);
        }

        template <typename F>
        void access_wait_clients(F&& callback) {
            std::lock_guard<std::mutex> lock(clients_mutex_);
            callback(wait_clients_);
        }

        void remove_client(const std::string& id);

        void create_token(std::string& token);
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