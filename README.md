# KuroNet
**Лёгкий асинхронный чат на C++/Boost.Asio**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/github/actions/workflow/status/Akeksichek/KuroNet/build.yml?label=CI)](https://github.com/Akeksichek/KuroNet)
[![CMake](https://img.shields.io/badge/CMake-3.15+-brightgreen.svg)](https://cmake.org)

## Установка зависимостей {#install}
```bash
# Linux
chmod +x install_deps.sh
./install_deps.sh
```
```markdown
Автоустановщик поддерживает только:
- Ubuntu 20.04+
- Debian 11+
- Arch Linux (последние версии)

```

## Сборка проекта {#build}
```bash
mkdir build && cd build
cmake .. && make
```

## Опции сборки {#options}
```bash
-DBUILD_EXAMPLES=ON   # Сборка примеров
-DBUILD_TESTS=ON      # Сборка тестов
```

## Пример запуска {#run}
```bash
./tests/logger_test    # Тесты
./examples/log_example # Примеры
```

## История изменений

### v0.3.0 (текущая) - 2024-04-21
- **Добавлено**
  - Методы `add_client()` и `remove_client()` для управления подключениями
  - Потокобезопасность через `std::mutex` (клиенты не "теряются" в многопоточной среде)
- **Изменено**:
  - Сессии клиентов вынесены в отдельный класс (`ClientSessionManager`)
  - Обновлена документация
- **Исправлено**
  - Возможные race condition при параллельном доступе к списку клиентов

### v0.2.0
- **Добавлено**:
  - Асинхронный TCP-сервер на Boost.Asio
  - Базовая обработка подключений клиентов
  - Улучшенная система логгирования сессий
- **Изменено**:
  - Полностью переработана документация
  - Оптимизирована система сборки
- **Исправлено**:
  - Потенциальные утечки памяти в логгере

### v0.1.0
- Первоначальная версия проекта
- Базовая система логгирования
- CMake-инфраструктура