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
    std::atomic<bool> Logger::stop_flag_ = false;
    std::vector<std::string> Logger::log_when_stopped_;

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
            case Level::Command: level_str = "COMMAND"; color = COLOR_BLUE; break;
        }

        std::string log_output = "[" + detail::current_time() + "][KuroNet][" 
            + color + level_str + COLOR_RESET 
            + "]: " + color + message + COLOR_RESET;

        if(stop_flag_.load()) {
            std::lock_guard<std::mutex> lock(log_mutex_);
            if (log_output.find("COMMAND") == std::string::npos) {
                log_output += "\t --history";
            }
            log_when_stopped_.push_back(log_output);
        } else {
            std::lock_guard<std::mutex> lock(log_mutex_);
            if(!log_when_stopped_.empty()) {
                std::for_each(log_when_stopped_.begin(), log_when_stopped_.end(), [](std::string log){
                    std::cout << log << std::endl;
                });
                log_when_stopped_.clear();
            }
            std::cout << log_output << std::endl;
        }
    }

    void Logger::stop()
    {
        stop_flag_.store(true);
    }
    void Logger::_continue()
    {
        stop_flag_.store(false);
    }

} // namespace kuro