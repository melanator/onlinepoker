cmake_minimum_required(VERSION 3.15)

project ("serverpoker")
enable_testing()

add_subdirectory(poker)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)
add_subdirectory(external/googletest)
