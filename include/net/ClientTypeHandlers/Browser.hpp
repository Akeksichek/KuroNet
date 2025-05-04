#pragma once

#include <net/ClientTypeHandlers/ClientTypeHndl.hpp>

#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/message.hpp>

namespace kuro
{
    namespace detectors {

        /**
         * @class Browser (в разработке)
         * @brief Обработчик HTTP-запросов от веб-браузеров
         *
         * Класс специализируется на:
         * - Определении браузерных запросов (по HTTP-заголовкам)
         * - Форматировании ответов в соответствии со стандартами HTTP
         * - Поддержке основных методов GET/POST/PUT/DELETE
         * - Автоматической обработке CORS и CSRF
         */
        class Browser
        {
        private:


        public:
            Browser();
            ~Browser();

            void request_processing();
            boost::beast::http::response<boost::beast::http::string_body> response_generation();
            void response_generate_and_auto_send();

        };

    } // namespace detectors
} // namespace kuro