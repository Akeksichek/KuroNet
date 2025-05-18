#include <net/core/ClientPolicyRegistry.hpp>

#include <net/Dispatcher/handlers/CustomHandler.hpp>
#include <net/Dispatcher/handlers/DesktopHandler.hpp>
#include <net/Dispatcher/handlers/MobileHandler.hpp>
#include <net/Dispatcher/handlers/WebHandler.hpp>
#include <net/Dispatcher/handlers/UnknownHandler.hpp>

namespace kuro {
    namespace dispatcher {

        void ClientPolicyRegistry::agents_init() {
            Logger::log(Logger::Level::Debug, "Start agents initialization");
            {
            std::lock_guard<std::mutex> lock(agents_mutex_);
            agents.emplace("unknown", AgentType::Unknown);
            agents.emplace("web_client", AgentType::KuroWeb);
            agents.emplace("desktop_app", AgentType::KuroDesktop);
            agents.emplace("mobile_app", AgentType::KuroMobile);
            agents.emplace("custom_client", AgentType::CustomClient);
            }
            Logger::log(Logger::Level::Debug, "End agents initialization");
        }
        void ClientPolicyRegistry::protocols_init()
        {
            Logger::log(Logger::Level::Debug, "Start protocols initialization");
            {
            std::lock_guard<std::mutex> lock(protocols_mutex_);
            protocols.emplace(AgentType::Unknown, std::make_shared<protocol::UnknownHandler>());
            protocols.emplace(AgentType::KuroDesktop, std::make_shared<protocol::DesktopHandler>());
            protocols.emplace(AgentType::KuroMobile, std::make_shared<protocol::MobileHandler>());
            protocols.emplace(AgentType::CustomClient, std::make_shared<protocol::CustomHandler>());
            protocols.emplace(AgentType::KuroWeb, std::make_shared<protocol::WebHandler>());
            }
            Logger::log(Logger::Level::Debug, "End protocols initialization");
        }

        std::shared_ptr<KuroNetClient> ClientPolicyRegistry::eponymous_protocol(AgentType agent)
        {
            std::lock_guard<std::mutex> lock(protocols_mutex_);
            auto it = protocols.find(agent);
            if (it != protocols.end()) {
                return it->second;
            }
            return protocols.at(AgentType::Unknown);
        }

        ClientPolicyRegistry::AgentType ClientPolicyRegistry::agent_selecter(const char* agent)
        {
            std::lock_guard<std::mutex> lock(agents_mutex_);
            auto it = agents.find(agent);
            return (it != agents.end() ? it->second : AgentType::Unknown);
        }

        std::shared_ptr<KuroNetClient> ClientPolicyRegistry::create(AgentType agent)
        {
            return eponymous_protocol(agent);
        }
        std::shared_ptr<KuroNetClient> ClientPolicyRegistry::create(const std::string& message)
        {
            auto agent = get_agent(message);
            return eponymous_protocol(agent);
        }

        ClientPolicyRegistry::AgentType ClientPolicyRegistry::get_agent(const std::string& message)
        {
            std::string key = "AGENT_TYPE:";
            size_t pos = message.rfind(key);
            if (pos == std::string::npos) {
                return AgentType::Unknown;
            }

            size_t start = pos + key.size();
            size_t end = message.find('\n', start);

            std::string agent_str = (end == std::string::npos)
                ? message.substr(start)
                : message.substr(start, end - start);

            return agent_selecter(agent_str.c_str());
        }

} // namespace dispatcher
} // namespace kuro