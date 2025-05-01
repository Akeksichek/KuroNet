#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <chrono>

#include <core/logger.hpp>
#include <core/commands/base_command.hpp>
#include <core/commands/commands_bundle.hpp>

namespace kuro
{
    /**
     * @class CommandHandler
     * @brief Обработчик команд для системы. Потокобезопасен, поддерживает историю выполнения.
     * 
     * Основные функции:
     * - Принимает команды вида "[действие] [объект] [параметры]"
     * - Проверяет валидность действия и объекта
     * - Запускает соответствующую команду в отдельном потоке
     * - Поддерживает историю выполненных команд
     * 
     * @note Для работы требует предварительной регистрации команд через exec_init().
     * @warning Метод run() должен запускаться в отдельном потоке для асинхронной работы.
     */
    class CommandHandler
    {
    protected:
        

    private:
        std::mutex executors_mtx_;    

        std::vector<std::string> command_parts;

        std::unordered_map<std::string, std::unique_ptr<commands::BaseCommand>> executors;
        void exec_init() {
            executors.emplace(
                std::string("token"),  // Явно создаём string
                std::unique_ptr<commands::BaseCommand>(new commands::TokenCommand())  // Явное приведение
            );
            executors.emplace(
                std::string("client"),
                std::unique_ptr<commands::BaseCommand>(new commands::ClientsCommand())
            );
        }    

        void set_nonblocking(bool);
        bool executor_valid();

        void command_partition(std::string&&);

        void get_help();

        static CommandHandler& get_imstance() {
            static CommandHandler instance;
            return instance;
        }

    public:
        CommandHandler() { exec_init(); }
        virtual ~CommandHandler() = default;

        /**
         * @brief Главный цикл обработки команд.
         * 
         * Запускать в бесконечном цикле нужно вручную иначе прослушивать нажатия клавиш он не будет:
         * 1. Ожидает ввода команды (активируется по символу '!')
         * 2. Парсит введённую строку
         * 3. Проверяет валидность действия и объекта
         * 4. Выполняет команду через зарегистрированный обработчик
         * 
         * @note Потокобезопасен: блокировка логгера ис сохранение истории логов осуществляется автоматически.
         * @warning Не блокирует основной поток сервера - асинхронные подключения продолжают обрабатываться.
         * 
         * Пример использования:
         * @code
         * std::thread cmd_thread(&CommandHandler::run);
         * cmd_thread.detach();
         * @endcode
         */
        static void run();
    };

}