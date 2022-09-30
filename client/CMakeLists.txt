cmake_minimum_required (VERSION 3.15)

project (client)
set (TABLE_SOURCES "include/table.h" "src/table.cpp")
set (POKER_SOURCES "include/poker.h" "src/poker.cpp")

add_executable (client "src/client.cpp" "include/client.h")
add_executable (game "include/poker.h" "src/poker.cpp" ${TABLE_SOURCES})
add_library(table_lib STATIC ${TABLE_SOURCES})
add_library(poker_lib STATIC ${POKER_SOURCES})

if(WIN32 OR MSVC)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()