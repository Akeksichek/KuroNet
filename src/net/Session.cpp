#include "net/Session.hpp"

namespace kuro
{
    void ClientSessionManager::add_client(Client&& client)
    {
        std::lock_guard<std::mutex> lock(clients_mutex_);

        if (wait_clients_.find(client.id) != wait_clients_.end()) {
            Logger::log(Logger::Level::Warning, "Client with id " + client.id + " already exists");
            return;
        }

        try {
            Logger::log(Logger::Level::Info, "New client " + client.id + " trying connect");
            wait_clients_.emplace(client.id, std::move(client));
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

    void ClientSessionManager::create_token(std::string& token)
    {
        std::lock_guard<std::mutex> lock(tokens_mtx_);
        active_tokens_.push_back(token);
    }

    bool ClientSessionManager::approve_client(const std::string& user_id, const std::string& token)
    {
        if(bool check_token = [this, &token](){
            std::lock_guard<std::mutex> lock(tokens_mtx_);
            for(auto token_ : active_tokens_) {
                if(token == token_) return true;
            }
            Logger::log(Logger::Level::Error, "This token: " + token + " - not found");
            return false;
        }(); !check_token) return false;

        std::lock_guard<std::mutex> lock(clients_mutex_);
        auto wait_client_iter = wait_clients_.find(user_id);
        if(wait_client_iter == wait_clients_.end()) {
            Logger::log(Logger::Level::Error, "User " + user_id + " not found");
            return false;
        }
        wait_client_iter->second.token = token;

        auto node = wait_clients_.extract(wait_client_iter);

        boost::asio::write(*wait_client_iter->second.socket, boost::asio::buffer("You are approved\n"));

        clients_.insert(std::move(node));

        return true;
    }

    void ClientSessionManager::auto_approve_client(Client&& client)
    {
        boost::asio::write(*client.socket, boost::asio::buffer("You are auto approved"));
        std::lock_guard<std::mutex> lock(clients_mutex_);
        clients_.emplace(client.id, std::move(client));
    }

} //namesapce kuro