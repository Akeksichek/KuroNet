#include <core/commands/token_command.hpp>

namespace kuro
{
    namespace commands
    {
        void TokenCommand::get_help() {
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

        void TokenCommand::create_token() {
            TokenHandler::TokenType level;
            if(command.size() <= 2) {
                Logger::log(Logger::Level::Command, R"(Supported token types:
                    -perm: permanent
                    -sess: session
                    -temp: temporary
                    )");
                return;
            } 
            else {
                std::string token_type = command[2];
                
                if(token_type == "-perm") {
                    level = TokenHandler::TokenType::PermanentToken;
                } 
                else if(token_type == "-sess") {
                    level = TokenHandler::TokenType::SessionToken;
                }
                else if(token_type == "-temp") {
                    level = TokenHandler::TokenType::TemporaryToken;
                }
                else {
                    Logger::log(Logger::Level::Error, "Unknown token type: " + token_type);
                    return;
                }
            }
            
            std::string token = TokenHandler::generate(level);
            Logger::log(Logger::Level::Command, "New token created: " + token);
        }

        void TokenCommand::execute(std::vector<std::string>& cmd) {
            if(cmd.size() < 2) {
                Logger::log(Logger::Level::Error, "Type \"token help\"");
                return;
            }
            this->command = cmd;
            auto it = operations.find(cmd[1]);
            if (it != operations.end()) {
                it->second();
            }
        }

    } //namespace commands
} //namespace kuro