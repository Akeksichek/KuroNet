#pragma once

#include <boost/asio/io_context.hpp>
#include <mutex>
#include <vector>
#include <thread>

#include "core/logger.hpp"

namespace kuro {

/**
 * @brief Главный управляющий класс для работы с асинхронными операциями
 * 
 * Содержит общий `io_context`, который используется всеми компонентами KuroNet.
 * Реализован как Singleton, чтобы гарантировать единую точку управления.
 * 
 * Пример использования:
 * @code
 * auto& session = Session::get_instance();
 * session.ioc_.run(); // Запуск обработки асинхронных операций
 * @endcode
 */
class Session {
private:
    Session() = default; // Приватный конструктор (Singleton)

public:
    /**
     * @brief Получить экземпляр Session (Singleton)
     * @return Ссылка на единственный экземпляр
     */
    static Session& get_instance() {
        static Session instance;
        return instance;
    }

    /// Основной контекст ввода-вывода (Boost.Asio)
    boost::asio::io_context ioc_;
};

} // namespace kuro