#include <core/CommandHandler.hpp>
#include <net/TCPServer.hpp>

#include <thread>
#include <chrono>

std::chrono::steady_clock::time_point start_prgrm = std::chrono::steady_clock::now();

int main()
{
    kuro::Logger::set_level(kuro::Logger::Level::Debug); // Выводим все логи
    
    std::thread cmd_thr([](){
        while(true) {
            kuro::CommandHandler::run(); // запускаем ожидание ввода команды(начнется после нажатия "!")
        }
    });

    kuro::TCPServer server(12345);

    std::thread io_thr([](){
        kuro::Session::get_instance().ioc_.run(); // запуск контекста для tcp сервера 
    });

    server.start(); // запуск сервера
    
    while(true)
    {
        std::chrono::steady_clock::time_point while_thr = std::chrono::steady_clock::now();
        if(while_thr - start_prgrm >= std::chrono::milliseconds(1500)) { // ожидаем 500 миллисекунд не останавливая поток
            kuro::Logger::log(kuro::Logger::Level::Info, "Main thread...");
            start_prgrm = std::chrono::steady_clock::now(); // обновляем время чтобы вызвать логгер еще раз
        }

    }

    cmd_thr.detach();

    return 0;
}