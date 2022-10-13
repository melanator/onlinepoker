cmake_minimum_required (VERSION 3.15)

project (server)

add_executable (server "src/server.cpp" "include/server.h")
if(WIN32 OR MSVC)
    target_link_libraries(${PROJECT_NAME} ws2_32 table_lib poker_lib)
else()
	target_link_libraries(${PROJECT_NAME} PUBLIC table_lib poker_lib)
endif()
