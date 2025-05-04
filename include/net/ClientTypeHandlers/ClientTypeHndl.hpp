#pragma once

#include <string>

namespace kuro
{
    /**
     * @class ClientTypeHndl
     * @brief Объект обработки и распределения запросов на разные типы подключений
     * 
     * Класс ClientTypeHndl занимается определением типа клиента на пример браузер,
     * клиентское приложение KuroNet(пока что можно имитировать с помощью Telnet),
     * Unknown - неизвестное приложение, будет требоваться настройка базового
     * KuroClient(будет реализовано в клиентском приложении) или игнорироваться(ВОЗМОЖНО! с соответствующим уведомлением)
     * 
     */
    class ClientTypeHndl
    {
    private:
        enum class ClientType { KuroClient, Browser, Unknown };

    public:
        ClientType recognition(const std::string& req_body);
        ~ClientTypeHndl() = default;
    };

 } // namespace kuro