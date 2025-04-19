#include "core/logger.hpp"

namespace kuro
{
    inline std::string detail::current_time() {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S")
            << "." << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }

    Logger::Level Logger::current_level_ = Logger::Level::Info;
    std::mutex Logger::log_mutex_;

    void Logger::set_level(Level level) {
        current_level_ = level;
    }

    void Logger::log(Level level, const std::string& message) {
        if (level < current_level_) return;

        const char* level_str = "";
        const char* color = "";
        switch (level) {
            case Level::Debug: level_str = "DEBUG"; color = COLOR_BLUE; break;
            case Level::Info: level_str = "INFO"; color = COLOR_GREEN; break;
            case Level::Warning: level_str = "WARN"; color = COLOR_YELLOW; break;
            case Level::Error: level_str = "ERROR"; color = COLOR_RED; break;
    }

    std::lock_guard<std::mutex> lock(log_mutex_);
    std::cout << "[" << detail::current_time() << "][KuroNet][" << color << level_str << COLOR_RESET << "]: " << color << message << COLOR_RESET << std::endl;
}

} // namespace kuro