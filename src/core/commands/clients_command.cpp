#include <core/commands/clients_command.hpp>

namespace kuro
{
    namespace commands{

        void ClientsCommand::get_help()
        {
            std::string help = 
            "Type \"help\" for see that message\n" + 
            [this](){
                std::string res = "Token help\n";
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
            if(cmd.size() <= 2) {
                Logger::log(Logger::Level::Error, "Type \"client help\"");
                return;
            }
            this->command = cmd;
            auto it = operations.find(cmd[1]);
            if (it != operations.end()) {
                it->second();
            }
        }

    } // namespace commands
} // namespace kuro