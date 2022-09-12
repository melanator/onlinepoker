cmake_minimum_required (VERSION 3.15)

set (This tests)

set(Sources
	test_table.cpp)
	

add_executable(${This} ${Sources})

add_test(
	NAME ${This}
	COMMAND ${This}
)

target_link_libraries(${This} PUBLIC gtest table_lib)


