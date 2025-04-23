#include "net/Session.hpp"

namespace kuro
{
    void ClientSessionManager::add_client(Client&& client)
    {
        std::lock_guard<std::mutex> lock(clients_mutex_);

        if (clients_.find(client.id) != clients_.end()) {
            Logger::log(Logger::Level::Warning, "Client with id " + client.id + " already exists");
            return;
        }

        try {
            Logger::log(Logger::Level::Info, "Client " + client.id + " added successfully");
            clients_.emplace(client.id, std::move(client));
        } catch (const std::exception& e) {
            Logger::log(Logger::Level::Error, "Failed to add client: " + std::string(e.what()));
        }
    }
    void ClientSessionManager::remove_client(const std::string& id) {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        
        auto iter = clients_.find(id);
        if (iter != clients_.end()) {
            try {
                if (iter->second.socket->is_open()) {
                    boost::system::error_code ec;
                    iter->second.socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
                    iter->second.socket->close(ec);
                    if (ec) {
                        Logger::log(Logger::Level::Warning, 
                                    "Error closing socket for client " + id + ": " + ec.message());
                    }
                }
                clients_.erase(iter);
                Logger::log(Logger::Level::Info, "Client " + id + " removed successfully");
            } catch (const std::exception& e) {
                Logger::log(Logger::Level::Error, 
                            "Exception while removing client " + id + ": " + std::string(e.what()));
            }
        } else {
            Logger::log(Logger::Level::Warning, "Client " + id + " not found for removal");
        }
    }


} //namesapce kuro