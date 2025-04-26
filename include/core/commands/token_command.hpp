#pragma once

#include <core/logger.hpp>
#include <core/UniqueUtils.hpp>
#include <core/commands/base_command.hpp>

#include <unordered_map>
#include <functional>

namespace kuro
{
    namespace commands
    {
        class TokenCommand : public BaseCommand
        {
        private:
            std::vector<std::string> command;
            void get_help();
            void create_token();
            std::unordered_map<std::string, std::function<void()>> operations;

            void init_operations() {
                operations.emplace("help", [this](){ get_help(); });
                operations.emplace("create", [this](){ create_token(); });
            }

        public:
            TokenCommand() { init_operations(); }
            void execute(std::vector<std::string>&) override;
        };
    }
}