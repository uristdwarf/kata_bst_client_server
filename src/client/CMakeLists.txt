cmake_minimum_required(VERSION 3.14)

set(SOURCE_FILES
	main.cpp
)

add_executable(client ${SOURCE_FILES})
target_include_directories(client INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

