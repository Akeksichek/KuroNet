#include <boost/asio/ip/tcp.hpp>

#include <memory>

#include "core/logger.hpp"
#include <net/Session.hpp>

namespace kuro
{
    /**
     * @brief Асинхронный TCP-сервер на Boost.Asio
     * 
     * Основные возможности:
     * - Приём подключений с логированием
     * - Остановка/перезапуск по требованию
     * 
     * @warning Не потокобезопасен (используйте мьютекс для вызовов из разных потоков)
     */
    class TCPServer
    {
    private:
        boost::asio::ip::tcp::endpoint endpoint;
        boost::asio::ip::tcp::acceptor acceptor;

        void do_accept();

    public:
        /**
         * @brief Создаёт сервер (но не запускает автоматически!)
         * @param port Порт для прослушивания (1-65535)
         */
        explicit TCPServer(uint16_t port)
            : endpoint(boost::asio::ip::address_v4::any(), port),
            acceptor(Session::get_instance().ioc_, endpoint)
        {}
        
        /**
         * @brief Запускает приём подключений
         * @throws boost::system::system_error Если порт занят
         */
        void start() { 
            acceptor.listen();
            do_accept();
        }

        /**
         * @brief Останавливает сервер (закрывает сокеты)
         */
        void stop();

    };// TCPServer

} //namespace kuro