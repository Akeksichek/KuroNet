#pragma once

#include <string>
#include <memory>
#include <boost/asio/ip/tcp.hpp>

namespace kuro {

    /**
     * @brief Структура данных клиента
     */
    struct Client
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket;
        std::string id;
        std::string login;
        std::string token = "";
        
        // Policy handling
        std::string agent_type = "";
    };

} // namespace kuro