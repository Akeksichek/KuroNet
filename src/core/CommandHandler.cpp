#include <core/CommandHandler.hpp>

namespace kuro
{
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
    
    void CommandHandler::command_partition(std::string&& command)
    {
        if(!command_parts.empty()) command_parts.clear();
        std::string part;
        for(auto elem : command) {
            if(elem == ' ') {
                command_parts.push_back(part);
                part.clear();
            } else {
                part += elem;
            }
        }
        command_parts.push_back(part);
    }
    
    bool CommandHandler::action_valid()
    {
        for(auto elem : supported_actions) {
            if(elem == command_parts[0]) {
                return true;
            }
        }
        Logger::log(Logger::Level::Error, "Неизвестная команда: " + command_parts[0]);
        return false;
    }
    bool CommandHandler::executor_valid()
    {
        if(command_parts[1].empty() || command_parts[1] == " ") {
            for(auto action : supported_actions) {
                if(action == command_parts[0]) {
                    Logger::log(Logger::Level::Error, "Операнд для " + command_parts[0] + ", не может быть пустым");
                    return false;
                }
            }
        }
        auto executor = executors.find(command_parts[1]);
        if(executor != executors.end()) {
            executor->second.get()->execute();
            return true;
        }
        Logger::log(Logger::Level::Error, "Неизвестный операнд: " + command_parts[1]);
        return false;
    }
    
    void CommandHandler::run()
    {
        auto cmd_hndl_ptr = &CommandHandler::get_imstance();
        
        cmd_hndl_ptr->set_nonblocking(true);
        if(char c; read(STDIN_FILENO, &c, 1) > 0 && c == '!') {
            Logger::stop();
            std::string command;
            std::cout << ">: ";
            cmd_hndl_ptr->set_nonblocking(false);
            std::getline(std::cin, command, '\n');
            cmd_hndl_ptr->command_partition(std::move(command));
    
            if(!cmd_hndl_ptr->action_valid() || !cmd_hndl_ptr->executor_valid())
            {
                Logger::_continue();
                return;
            }
            Logger::_continue();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // перерыв для CPU + время обработки подключений
    }
} // namespace kuro