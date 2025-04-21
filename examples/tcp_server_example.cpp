#include "net/TCPServer.hpp"
#include "core/logger.hpp"
#include <thread>

int main() {
    const uint16_t port = 12345; //обьявление порта

    kuro::Logger::set_level(kuro::Logger::Level::Debug); // фильтрция логов

    kuro::TCPServer server(port); // инициализация сервера

    std::thread io_thread([](){
        kuro::Session::get_instance().ioc_.run(); // запуск контекста в отдельном потоке для boost
    });
    
    server.start(); //запуск сервера

    kuro::Logger::log(kuro::Logger::Level::Info,
        "Server started on port " + std::to_string(port)); // уведолмение о успешном запуске :)
    
    io_thread.join(); // ожидание завершения потока с контекстом
    return 0;
}