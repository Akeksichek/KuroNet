#include <core/CommandHandler.hpp>
#include <net/TCPServer.hpp>

#include <thread>
#include <chrono>

std::chrono::steady_clock::time_point start_prgrm = std::chrono::steady_clock::now();

int main()
{
    kuro::Logger::set_level(kuro::Logger::Level::Error); // Выводим все логи
    
    std::thread cmd_thr([](){
        kuro::CommandHandler::run(); // запускаем ожидание ввода команды(начнется после нажатия "!")
    });

    kuro::TCPServer server(12345);

    std::thread io_thr([](){
        kuro::Session::get_instance().ioc_.run(); // запуск контекста для tcp сервера 
    });

    server.start(); // запуск сервера

    cmd_thr.join();
    io_thr.join();

    return 0;
}