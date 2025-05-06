#include <net/Chat/Chat.hpp>

namespace kuro {
    namespace chat {

        void Chat::do_read(const std::string& client_id, const Client& client)
        {
            auto buffer = std::make_shared<boost::asio::streambuf>();
            auto client_ptr = std::make_shared<Client>(client);

            boost::asio::async_read_until(
                *client_ptr->socket,
                *buffer,
                '\n',
                [this, client_ptr, client_id, buffer](const boost::system::error_code& ec, size_t /*bytes*/){
                    if(ec) {
                        Logger::log(Logger::Level::Error, "Client: " + client_ptr->login + " - read error: " + ec.message());
                        return;
                    }
                    std::istream is(buffer.get());
                    std::string message;
                    std::getline(is, message);

                    do_send(client_id, message);
                }
            );
        }
        void Chat::do_send(const std::string& sender_id, const std::string& message)
        {
            Logger::log(Logger::Level::Info, "Client " + sender_id + " write: \n\t" + message);

            auto& manager = Session::get_instance().manager;
            manager.access_clients([sender_id, message](const std::unordered_map<std::string, Client>& clients){
                for(const auto& [client_id, client] : clients) {
                    if(client_id == sender_id || !client.socket->is_open()) continue;

                    auto client_ptr = std::make_shared<Client>(client);
                    auto write_buffer = std::make_shared<boost::asio::streambuf>();
                    std::ostream os(write_buffer.get());
                    os << message << '\n';
                    boost::asio::async_write(*client_ptr->socket, *write_buffer, [client_ptr, write_buffer](const boost::system::error_code& ec, size_t){
                        if(ec) {
                            Logger::log(Logger::Level::Error, "Client " + client_ptr->login + " - send error" + ec.message());
                            return;
                        }
                    });
                }
            });
        }

        void Chat::work()
        {
            auto& manager = Session::get_instance().manager;

            while(started.load()) {
                manager.access_clients([this](const std::unordered_map<std::string, Client>& clients){
                    for(const auto& [client_id, client] : clients) {
                        do_read(client_id, client);
                    }
                });   
            }
        }

        void Chat::stop()
        {
            if(!started.load()) {
                Logger::log(Logger::Level::Error, "I can't stop something that didn't start");
                return;
            } else started.store(false);
        }

        void Chat::start() {
            if(started.load()) {
                Logger::log(Logger::Level::Error, "I can't run something that's already running");
                return;
            } else {
                started.store(true);
                work();
            }
        }

    } // namespace chat
} // namespace kuro