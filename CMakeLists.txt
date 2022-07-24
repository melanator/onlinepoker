# CMakeList.txt: проект CMake для somegame; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("somegame")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (somegame "src/somegame.cpp" "include/somegame.h")
add_executable (client "src/client/client.cpp" "include/somegame.h")
add_executable (server "src/server/server.cpp" "include/somegame.h")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
