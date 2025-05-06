#include <core/CommandHandler.hpp>

namespace kuro
{
    void CommandHandler::get_help() {
        std::string help = 
            "\nType \"help\" for see that message\nexit for close KuroNet\n" + 
            [this](){
                std::scoped_lock lock(executors_mtx_);
                std::string res = "Supported operations\n";
                for(const auto& pair : executors) {
                    res += pair.first + '\n';
                }
                return res;
            }();
        Logger::log(Logger::Level::Command, help);
    }

    void CommandHandler::set_nonblocking(bool enable) {
        static struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        if (enable) {
            newt.c_lflag &= ~(ICANON | ECHO);
            newt.c_cc[VMIN] = 0;
            newt.c_cc[VTIME] = 0;
        } else {
            newt.c_lflag |= (ICANON | ECHO);
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    
    void CommandHandler::command_partition(std::string&& command) {
        command_parts.clear();
        
        std::istringstream iss(command);
        std::string part;
        
        while (iss >> part) {
            command_parts.push_back(part);
        }
    }    

    bool CommandHandler::executor_valid()
    {
        auto executor = executors.find(command_parts[0]);
        if(executor != executors.end()) {
            executor->second.get()->execute(command_parts);
            return true;
        }
        Logger::log(Logger::Level::Error, "Неизвестный операнд: " + command_parts[0]);
        return false;
    }
    
    void CommandHandler::run()
    {
        auto cmd_hndl_ptr = &CommandHandler::get_imstance();
        
        
        while(true)
        {
            cmd_hndl_ptr->set_nonblocking(true);
            Logger::_continue();
            if(char c; read(STDIN_FILENO, &c, 1) > 0 && c == '!') {
                Logger::stop();
                std::string command;
                std::cout << ">: ";
                cmd_hndl_ptr->set_nonblocking(false);
                std::getline(std::cin, command, '\n');

                if(command == "exit") {
                    exit(0);
                }
                if(command == "help") {
                    cmd_hndl_ptr->get_help();
                    continue;
                }

                cmd_hndl_ptr->command_partition(std::move(command));
        
                if(!cmd_hndl_ptr->executor_valid())
                {
                    continue;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // перерыв для CPU + время обработки подключений
        }
    }
} // namespace kuro