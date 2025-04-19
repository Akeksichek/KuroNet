# KuroNet
**Лёгкий асинхронный чат на C++/Boost.Asio**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/github/actions/workflow/status/Akeksichek/KuroNet/build.yml?label=CI)](https://github.com/Akeksichek/KuroNet/actions)
[![CMake](https://img.shields.io/badge/CMake-3.15+-brightgreen.svg)](https://cmake.org)

## Установка зависимостей {#install}
```bash
# Linux
chmod +x install_deps.sh
./install_deps.sh
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