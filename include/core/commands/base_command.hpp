#pragma once

namespace kuro
{
    namespace commands
    {
        class BaseCommand
        {
        public:
            virtual void execute() = 0;
        };
    }
}