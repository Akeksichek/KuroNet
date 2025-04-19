#Ubuntu/Debian
if [ -f /etc/debian_version ]; then
    sudo apt update
    sudo apt install -y libboost-all-dev cmake g++
    exit 0
fi

#Arch Linux
if [ -f /etc/arch-release ]; then
    sudo pacman -Sy boost cmake gcc
    exit 0
fi

echo "Ваша ОС не поддерживается автоматической установкой. Установите Boost и CMake вручную."
exit 1