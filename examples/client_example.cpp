#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

const std::string SERVER_IP = "127.0.0.1";
const int PORT = 12345;
const std::string TEST_LOGIN = "test_user";

void send_some_text(const std::string& message) {

}

void run_example_client() {
    try {
        boost::asio::io_context io;

        boost::asio::ip::tcp::socket socket(io);
        
        socket.connect({
            boost::asio::ip::address::from_string(SERVER_IP),
            PORT
        });

        std::string request = 
            "TEST!\n" /*key word*/
            "LOGIN:" + TEST_LOGIN + "\n"
            "AGENT_TYPE:desktop_app\n"
            "MAX_MESSAGE_SIZE:4096\n"
            "ALLOW_RAW_TCP:1"
            "\n\n";

        std::cout << "Sending:\n" << request << std::endl;
        boost::asio::write(socket, boost::asio::buffer(request));

        std::string response;
        auto read_buffer = std::shared_ptr<boost::asio::streambuf>();
        boost::asio::read(socket, *read_buffer);

        std::istream is(read_buffer.get());
        std::string resp;
        std::getline(is, resp);
        std::cout << "\nServer response:\n" << resp << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    run_example_client();
    return 0;
}