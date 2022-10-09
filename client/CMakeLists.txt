cmake_minimum_required (VERSION 3.15)

project (client)

add_executable (client "src/client.cpp" "include/client.h")

if(WIN32 OR MSVC)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()