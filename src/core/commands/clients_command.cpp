#include <core/commands/clients_command.hpp>

namespace kuro
{
    namespace commands{

        void ClientsCommand::get_help()
        {
            std::string help = 
            "Type \"help\" for see that message\n" + 
            [this](){
                std::string res = "Clients help\n";
                for(const auto& pair : operations) {
                    res += pair.first + '\n';
                }
                return res;
            }();
            Logger::log(Logger::Level::Command, help);
        }

        void ClientsCommand::approve()
        {
            if(command.size() < 4) {
                Logger::log(Logger::Level::Command, "Command Error: client approve <user id> <token>");
                return;
            }
            if(Session::get_instance().manager.approve_client(command[2], command[3])) {
                Logger::log(Logger::Level::Command, "Client: " + command[2] + ", approved succesfuly");
            }
        }

        void ClientsCommand::execute(std::vector<std::string>& cmd)
        {
            if(cmd.size() < 2) {
                Logger::log(Logger::Level::Error, "Type \"client help\"");
                return;
            }
            this->command = cmd;
            auto it = operations.find(cmd[1]);
            if (it != operations.end()) {
                it->second();
            }
        }

        void ClientsCommand::get_wait_clients() {
            if (command.size() < 2) {
                Logger::log(Logger::Level::Command, "Command error: type client <jurisdiction>");
                return;
            }
        
            auto& manager = Session::get_instance().manager;
            std::vector<std::string> wait_clients_list;
            manager.access_wait_clients([&wait_clients_list](const std::unordered_map<std::string, Client>& clients) {
                for (const auto& [id, client] : clients) {
                    if(!client.socket->is_open()) Logger::log(Logger::Level::Warning, id + ", leave");
                    auto user = id + " | " + client.login;
                    wait_clients_list.push_back(user);
                }
            });
        
            if (wait_clients_list.empty()) {
                Logger::log(Logger::Level::Command, "No clients in queue.");
                return;
            }
        
            std::string users_list_str;
            for (const auto& client : wait_clients_list) {
                users_list_str += client + "\n";
            }
            Logger::log(Logger::Level::Command, "Clients list:\n" + users_list_str);
        }

        void ClientsCommand::get_clients() {
            if (command.size() < 2) {
                Logger::log(Logger::Level::Command, "Command error: type client <jurisdiction>");
                return;
            }
        
            auto& manager = Session::get_instance().manager;
            std::vector<std::string> clients_list;
            manager.access_clients([&clients_list](const std::unordered_map<std::string, Client>& clients) {
                for (const auto& [id, client] : clients) {
                    auto user = id + " | " + client.login;
                    clients_list.push_back(user);
                }
            });
        
            if (clients_list.empty()) {
                Logger::log(Logger::Level::Command, "No approved clients.");
                return;
            }
        
            std::string users_list_str;
            for (const auto& client : clients_list) {
                users_list_str += client + "\n";
            }
            Logger::log(Logger::Level::Command, "Clients list:\n" + users_list_str);
        }
    } // namespace commands
} // namespace kuro