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
            [session_ptr, buffer, socket](const boost::system::error_code& ec, size_t bytes_transferred){
                if(ec) {
                    Logger::log(Logger::Level::Error, "Read error: " + ec.message());
                    return;
                }
                std::string message(
                    boost::asio::buffers_begin(buffer->data()),
                    boost::asio::buffers_begin(buffer->data()) + bytes_transferred
                );
                buffer->consume(bytes_transferred);
                message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());


                Logger::log(Logger::Level::Debug, message);

                Client client;
                client.id = IDGenerator::generate();
                client.socket = socket;

                bool auto_approving = false;
                auto agent_type = session_ptr->manager.registry.get_agent(message);
                auto agent_ptr = session_ptr->manager.registry.create(agent_type);

                if(!agent_ptr->valid_request(message)) {
                    boost::asio::write(*socket, boost::asio::buffer("INVALID REQUEST FORMAT\n"));
                    socket->close();
                    return;
                }
                if(agent_type == dispatcher::ClientPolicyRegistry::AgentType::Unknown) {
                    boost::asio::write(*socket, boost::asio::buffer("INVALID AGENT TYPE\n"));
                    socket->close();
                    return;
                }

                agent_ptr->parse_client_metadata(message, client);
                Logger::log(Logger::Level::Debug, client.agent_type);

                if(message.rfind("auto") != std::string::npos) {
                    session_ptr->manager.auto_approve_client(std::move(client));
                    return;
                } else {
                    session_ptr->manager.add_client(std::move(client));
                    return;
                }
            }
        );
    }

    void TCPServer::stop()
    {
        acceptor.close();
    }

} //namespace kuro