#pragma once
#include <string>
#include <iostream>
#include <mutex>
#include <iomanip>
#include <vector>
#include <atomic>
#include <algorithm>

#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_RESET      "\033[0m"

namespace kuro {
    namespace detail {
        inline std::string current_time();
    } // namespace detail
    /**
     * @brief Класс отвечающий за логирование и маркировку уведомлений
     * 
     * Пример использования:
     * @code
     * Logger::log(Logger::Level::Info, "Some message");
     * @endcode
     */
    class Logger {
    public:
        enum class Level { Debug, Info, Warning, Error, Command };
        
        /**
         * @brief Устанавливает глобальный уровень логирования
         * 
         * Сообщения с уровнем ВЫШЕ установленного будут игнорироваться.
         * По умолчанию: Level::Info (показываются Info, Warning, Error).
         * 
         * @param level Новый уровень фильтрации (Debug|Info|Warning|Error)
         * 
         * @note Пример:
         * @code
         * // В продакшене - только ошибки
         * Logger::set_level(Logger::Level::Error);
         * 
         * // В дебаге - всё
         * Logger::set_level(Logger::Level::Debug);
         * @endcode
         */
        static void set_level(Level level);

        static void log(Level level, const std::string& message);

        static void stop();
        static void _continue();

    private:
        static Level current_level_;
        static std::mutex log_mutex_;

        static std::vector<std::string> log_when_stopped_;
        static std::atomic<bool> stop_flag_;
    };

} // namespace kuro