#pragma once

#include <core/logger.hpp>
#include <net/Session.hpp>

#include <atomic>
#include <thread>
#include <chrono>

#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>

namespace kuro {
    namespace chat {

        class Chat {
            private:
                std::atomic<bool> started{false};
            
                void do_read(const std::string& client_id, const Client& client);
                void do_send(const std::string& sender_id, const std::string& message);

                void work();
            
            public:
                Chat() = default;
                ~Chat() {
                    stop();
                }
                void start();
                void stop();
            };

    } // namespace chat
} // namespace kuro