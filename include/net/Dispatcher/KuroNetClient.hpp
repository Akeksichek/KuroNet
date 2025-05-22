#pragma once

#include <string>
#include <sstream>

#include <net/Client.hpp>
#include <core/logger.hpp>
#include <core/UniqueUtils.hpp>

namespace kuro {
    namespace dispatcher {

        class KuroNetClient
        {
        protected:
            size_t max_message_size = 4096;
            bool allow_raw_tcp = true;
            std::string agent_type = "";

            std::string fill_up_field(const std::string& text, const std::string& key) {
                size_t pos = text.find(key);
                if (pos == std::string::npos) return "";

                size_t value_start = pos + key.size();
                size_t line_end = text.find('\n', value_start);
                
                if (line_end == std::string::npos) {
                    line_end = text.length();
                }

                std::string value = text.substr(value_start, line_end - value_start);
                value.erase(value.find_last_not_of(" \r\n") + 1);
                return value;
            }

            virtual size_t get_max_message_size() {
                return max_message_size;
            };
            virtual bool get_allow_raw_tcp() {
                return allow_raw_tcp;
            };
            virtual std::string get_agent_type() {
                return agent_type;
            };

        public:
            bool valid_request(const std::string& message) {
                std::string agent_type_ = fill_up_field(message, "AGENT_TYPE:");
                std::string max_message_size_ = fill_up_field(message, "MAX_MESSAGE_SIZE:");
                std::string allow_raw_tcp_ = fill_up_field(message, "ALLOW_RAW_TCP:");
                if(agent_type_.size() > 0 && max_message_size_.size() > 0 && allow_raw_tcp_.size() > 0 )
                    return true;
                return false;
            }

            virtual void parse_client_metadata(const std::string& message, Client& client) {
                try {
                    client.login = fill_up_field(message, "LOGIN:");
                    if (client.login.empty()) {
                        client.login = "member_" + kuro::kuro_generator::random_generate_str(5);
                        throw std::invalid_argument("Empty LOGIN");
                    }
                    
                    client.agent_type = get_agent_type();
                    
                    Logger::log(Logger::Level::Debug, 
                        "Parsed client: " + client.login + 
                        ", type: " + client.agent_type);
                        
                } catch (const std::exception& e) {
                    Logger::log(Logger::Level::Error, 
                        "Metadata error: " + std::string(e.what()));
                    throw std::runtime_error("Matadata parse error");
                }
            }

            virtual std::string handle_response(const std::string& message, const kuro::Client& client, const std::string& response) {
                std::ostringstream ss;
                ss << "USER_ID:" << client.id
                   << "\nAGENT_TYPE:" << client.agent_type
                   << "\nCONTENT:" << response << "\r\n\r\n";
                return ss.str();
            }

            KuroNetClient() = default;
            KuroNetClient(const KuroNetClient&) = default;
            KuroNetClient(KuroNetClient&&) = default;
            virtual ~KuroNetClient() = default;
        };

    } // namespace dispatcher
} // namespace kuro