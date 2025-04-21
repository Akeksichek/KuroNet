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

                on_client_connected(sock);
                do_accept();
            }
        );
    }
    void TCPServer::on_client_connected(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
    {
        auto session_ptr = &Session::get_instance();
        auto buffer = std::make_shared<boost::asio::streambuf>();

        boost::asio::async_read_until(*socket, *buffer, '\n', 
            [session_ptr, buffer, socket](const boost::system::error_code& ec, size_t /*bytes_transferred*/){
                if(ec) {
                    Logger::log(Logger::Level::Error, "Read error: " + ec.message());
                    return;
                }
                std::istream is(buffer.get());
                std::string message;
                std::getline(is, message);

                Logger::log(Logger::Level::Info, message);

                if(message.rfind("LOGIN:", 0) != 0) {
                    Logger::log(Logger::Level::Warning, "Пользователь " 
                                + socket->remote_endpoint().address().to_string() 
                                + " передал неверные данные");
                    boost::asio::write(*socket, boost::asio::buffer("INVALID_FORMAT\n"));
                    socket->close();
                    return;
                }

                std::string login = message.substr(6);
                
                Client client{
                    socket,
                    "temp_test_id",
                    login,
                };
                
                Session::get_instance().manager.add_client(std::move(client));
                Logger::log(Logger::Level::Info, "User logged in: " + login);
                session_ptr->manager.add_client(std::move(client));
                
                boost::asio::write(*socket, boost::asio::buffer("AUTH_OK\n"));
            }
        );
    }

    void TCPServer::stop()
    {
        acceptor.close();
    }

} //namespace kuro