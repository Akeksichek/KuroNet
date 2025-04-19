#pragma once
#include <string>
#include <iostream>
#include <mutex>
#include <iomanip>

#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_RESET      "\033[0m"

namespace kuro {
    namespace detail {
        inline std::string current_time();
    } // namespace detail
    class Logger {
    public:
        enum class Level { Debug, Info, Warning, Error };
        
        static void set_level(Level level);

        static void log(Level level, const std::string& message);

    private:
        static Level current_level_;
        static std::mutex log_mutex_;

};

} // namespace kuro