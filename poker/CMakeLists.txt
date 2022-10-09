cmake_minimum_required (VERSION 3.15)

set (TABLE_SOURCES "include/table.h" "src/table.cpp")
set (POKER_SOURCES "include/poker.h" "src/poker.cpp")

#add_executable (game ${POKER_SOURCES} ${TABLE_SOURCES})
add_library(table_lib STATIC ${TABLE_SOURCES})
add_library(poker_lib STATIC ${POKER_SOURCES})
