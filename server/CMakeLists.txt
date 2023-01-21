cmake_minimum_required (VERSION 3.15)

project (server)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE "${CMAKE_SOURCE_DIR}/external/asio-1.24.0/include")

add_executable (server "src/server.cpp" "include/server.h")

set(CMAKE_CXX_FLAGS "-DASIO_STANDALONE")
set(CMAKE_CXX_FLAGS "-D_WIN32_WINNT=0x0601")
target_link_libraries(${PROJECT_NAME} asio table_lib poker_lib)

#[[
Link options for old server settings
if(WIN32 OR MSVC)
    target_link_libraries(${PROJECT_NAME} asio table_lib poker_lib)
else()
	target_link_libraries(${PROJECT_NAME} PUBLIC asio table_lib poker_lib)
endif()
]]
