#include "net/TCPServer.hpp"

namespace kuro
{
    void TCPServer::do_accept()
    {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(Session::get_instance().ioc_);

        acceptor.async_accept(*socket.get(),
            [this, sock = socket](const boost::system::error_code& ec){
                if(ec) {
                    Logger::log(Logger::Level::Error, ec.message());
                } else {
                    try {
                        auto user_addr = sock->remote_endpoint();
                        Logger::log(Logger::Level::Info, "New client: " + user_addr.address().to_string());
                    } catch (const boost::system::system_error& e) {
                        Logger::log(Logger::Level::Error, "Can't get client address: " + std::string(e.what()));
                    }
                }
                do_accept();
            }
        );
    }
    void TCPServer::stop()
    {
        acceptor.close();
    }

} //namespace kuro