#include "net/TCPServer.hpp"
#include "core/logger.hpp"
#include <thread>

int main() {
    const uint16_t port = 12345;

    kuro::Logger::set_level(kuro::Logger::Level::Debug);

    kuro::TCPServer server(port);

    server.start();

    std::thread io_thread([](){
        kuro::Session::get_instance().ioc_.run();
    });

    kuro::Logger::log(kuro::Logger::Level::Info,
        "Server started on port " + std::to_string(port));
    
    io_thread.join();
    return 0;
}