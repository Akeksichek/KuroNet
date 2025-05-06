#include <core/CommandHandler.hpp>
#include <net/TCPServer.hpp>
#include <net/Chat/Chat.hpp>

#include <thread>
#include <chrono>

std::chrono::steady_clock::time_point start_prgrm = std::chrono::steady_clock::now();

int main()
{
    kuro::Logger::set_level(kuro::Logger::Level::Info); // Фильтрация логов
    
    std::thread cmd_thr([](){
        kuro::CommandHandler::run();
    });

    // инициализация и запуск сервера
    kuro::TCPServer server(12345);
    std::thread io_thr([](){
        kuro::Session::get_instance().ioc_.run();
    });
    server.start();

    kuro::chat::Chat chat;
    
    std::thread chat_thr([&chat](){ chat.start(); });

    // ожидание завершение потоков
    cmd_thr.join();
    chat_thr.join();
    io_thr.join();

    return 0;
}