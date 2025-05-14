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
        
        // Policy handling
        size_t max_message_size = 4096;
        bool allow_raw_tcp = false;
        std::string auto_approve_key;
        std::string agent_type;
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

    private:
    bool is_client_really_dead(boost::asio::ip::tcp::socket& socket) {
        if (!socket.is_open()) return true;
    
        boost::system::error_code ec;
        socket.remote_endpoint(ec);
        
        if (ec == boost::asio::error::not_connected ||
            ec == boost::asio::error::connection_reset ||
            ec == boost::asio::error::broken_pipe) {
            return true;
        }
    
        char buf;
        socket.non_blocking(true);
        size_t bytes = socket.read_some(boost::asio::buffer(&buf, 1), ec);
        
        if (ec == boost::asio::error::eof) return true;
        
        if (ec == boost::asio::error::would_block) return false;
        
        return true;
    }

    public:
        /**
         * @brief Автоматически принимает пользователя
         * 
         * Занимается автоматическим допуском клиента и выдает сессионный
         * токен(сессионный токен активен, пока пользователь не отключится)
         * 
         * @param bool key - ключ
         * @param Client&& client - обьект Client поддреживающий move-семантику, для передачи контроля клиента сессии
         */
        void auto_approve_client(Client&& client);
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
            
            for(auto it = wait_clients_.begin(); it != wait_clients_.end(); ) {
                if(is_client_really_dead(*it->second.socket)) {
                    Logger::log(Logger::Level::Warning, 
                        "Removing dead client: " + it->first);
                    
                    boost::system::error_code ec;
                    it->second.socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
                    it->second.socket->close(ec);
                    
                    it = wait_clients_.erase(it);
                } else {
                    ++it;
                }
            }

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