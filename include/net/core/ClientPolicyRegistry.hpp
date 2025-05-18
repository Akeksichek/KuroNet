#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>

#include <core/logger.hpp>
#include <net/Dispatcher/KuroNetClient.hpp>

namespace kuro{
    namespace dispatcher {

        class ClientPolicyRegistry
        {
        public:
            enum class AgentType {
                Unknown,       // Неопознанные (бан по умолчанию)
                KuroWeb,       // Веб-версия KuroNet (особая обработка)
                KuroDesktop,   // Нативное десктоп-приложение
                KuroMobile,    // Нативное мобильное приложение
                CustomClient   // Полностью кастомный клиент, постоенный на основе базового клиента
            };

        private:
            std::mutex agents_mutex_;
            std::mutex protocols_mutex_;

            std::unordered_map<std::string, AgentType> agents;
            std::unordered_map<AgentType, std::shared_ptr<KuroNetClient>> protocols;
            
            void agents_init();
            void protocols_init();

            
            AgentType agent_selecter(const char* agent);
            std::shared_ptr<KuroNetClient> eponymous_protocol(AgentType agent);

            // std::shared_ptr<KuroNetClient> createHandler(AgentType type) {
            //     switch (type) {
            //         case AgentType::Unknown: return std::make_shared<protocol::UnknownHandler>();
            //         case AgentType::KuroDesktop: return std::make_shared<protocol::DesktopHandler>();
            //         case AgentType::KuroMobile: return std::make_shared<protocol::MobileHandler>();
            //         case AgentType::CustomClient: return std::make_shared<protocol::CustomHandler>();
            //         case AgentType::KuroWeb: return std::make_shared<protocol::WebHandler>();
            //         default: throw std::invalid_argument("Unknown");
            //     }
            // }

        public:

            ClientPolicyRegistry() {
                agents_init();
                protocols_init();
            }

            AgentType get_agent(const std::string& message);
            std::shared_ptr<KuroNetClient> create(AgentType agent);

            std::shared_ptr<KuroNetClient> create(const std::string& message);

            ~ClientPolicyRegistry() = default;

        };

    } // namespace dispatcher
} // namespace kuro