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

        boost::asio::async_read_until(*socket, *buffer, "\r\n\r\n",
            [session_ptr, buffer, socket](const boost::system::error_code& ec, size_t /*bytes_transferred*/){
                if(ec) {
                    Logger::log(Logger::Level::Error, "Read error: " + ec.message());
                    return;
                }
                std::istream is(buffer.get());
                std::string message;
                std::getline(is, message);

                Logger::log(Logger::Level::Info, message);

                std::string login;
                bool auto_approving = false;
                if(message.rfind("TEST!") != std::string::npos) {
                    if(auto login_pos = message.rfind("LOGIN:"); login_pos != std::string::npos) {
                        login = message.substr(login_pos);
                        auto_approving = true;
                    } else {
                        login = "TEST_" + kuro_generator::random_generate_str(5);
                    }
                } else {
                    boost::asio::write(*socket, boost::asio::buffer("INVALID_FORMAT\n"));
                    socket->close();
                    return;
                }
                
                Client client{
                    socket,
                    IDGenerator::generate(),
                    login,
                };

                if(auto_approving) {
                    session_ptr->manager.auto_approve_client(std::move(client));
                    return;
                }

                session_ptr->manager.add_client(std::move(client));
                boost::asio::write(*socket, boost::asio::buffer("Wait for host approval you\n"));
            }
        );
    }

    void TCPServer::stop()
    {
        acceptor.close();
    }

} //namespace kuro