#pragma once

namespace kuro
{
    namespace commands
    {
        class BaseCommand
        {
        public:
            virtual void execute(std::vector<std::string>&) = 0;
        };
    }
}