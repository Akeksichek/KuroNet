#pragma once

#include <net/Session.hpp>
#include <core/logger.hpp>
#include <core/UniqueUtils.hpp>
#include <core/commands/base_command.hpp>

#include <unordered_map>
#include <functional>

namespace kuro
{
    namespace commands
    {
        class ClientsCommand : public BaseCommand
        {
        private:
            std::vector<std::string> command;
            void get_help();
            void approve();

            // в разработке

            // void reject();
            // void get_wait_clients();
            // void get_approved_clients();
            std::unordered_map<std::string, std::function<void()>> operations;

            void init_operations() {
                operations.emplace("help", [this](){ get_help(); });
                operations.emplace("approve", [this](){ approve(); });

                // в разработке

                // operations.emplace("reject", [this](){ reject(); });
                // operations.emplace("wait_clients", [this](){ get_wait_clients(); });
                // operations.emplace("approved_clients", [this](){ get_approved_clients(); });
            }

        public:
            ClientsCommand() { init_operations(); }
            void execute(std::vector<std::string>&) override;
        };
    }
}