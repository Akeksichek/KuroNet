#pragma once

#include <net/Dispatcher/KuroNetClient.hpp>

namespace kuro {
    namespace protocol {

        class WebHandler : public kuro::dispatcher::KuroNetClient
        {
        private:
            size_t max_message_size = 1024*8;
            bool allow_raw_tcp = false;
            std::string agent_type = "web_client";

            size_t get_max_message_size() override {
                return max_message_size;
            }
            bool get_allow_raw_tcp() override {
                return  allow_raw_tcp;
            }
            std::string get_agent_type() override {
                return  agent_type;
            }

        public:
            WebHandler() {
                Logger::log(Logger::Level::Debug, "Web handler ready");
            }
        };

    } // namespace protocol
} // namespace kuro