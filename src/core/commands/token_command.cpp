#include <core/commands/token_command.hpp>

namespace kuro
{
    namespace commands
    {
        
        void TokenCommand::execute()
        {
            Logger::log(Logger::Level::Command, "Token command was execute");
        }

    } //namespace commands
} //namespace kuro