cmake_minimum_required (VERSION 3.8)

project (client)

add_executable (client "src/client.cpp" "include/client.h")
add_executable (game "src/tictactoe.cpp")
if(WIN32 OR MSVC)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()