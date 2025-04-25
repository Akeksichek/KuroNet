#pragma once

#include <core/logger.hpp>
#include <core/commands/base_command.hpp>

namespace kuro
{
    namespace commands
    {
        class TokenCommand : public BaseCommand
        {
        private:
        public:
            void execute() override;
        };
    }
}