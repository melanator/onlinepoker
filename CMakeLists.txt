cmake_minimum_required(VERSION 3.15)

project ("somegame")
enable_testing()

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)
add_subdirectory(external/googletest)
