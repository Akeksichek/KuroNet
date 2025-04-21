#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read_until.hpp>

#include <memory>

#include "core/logger.hpp"
#include <net/Session.hpp>

namespace kuro
{

    /**
     * @class TCPServer
     * @brief Асинхронный TCP-сервер для чата
     *  
     * Пока работает только с IPv4, но планируется и IPv6.
     * 
     * ## Как это работает?
     * 1. Сервер создаётся на указанном порту (`::start()`)
     * 2. В фоне начинает принимать подключения (`do_accept()`)
     * 3. Для каждого клиента создаётся сессия (`on_client_connected()`)
     * 4. Если что-то идёт не так — логирует и мягко завершает работу
     * 
     * @warning Пока не умеет в HTTP.
     *          Не потокобезопасен.
     * 
     * @example
     * // Минимальный пример запуска:
     * kuro::TCPServer server(8080);
     * server.start(); // Поехали!
     * 
     * @version 0.3.0 (KuroNet)
     */
    class TCPServer
    {
    private:
        boost::asio::ip::tcp::endpoint endpoint;
        boost::asio::ip::tcp::acceptor acceptor;

        void do_accept();
        void on_client_connected(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

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