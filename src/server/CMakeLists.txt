cmake_minimum_required(VERSION 3.14)

set(SOURCE_FILES
	server.cpp
)

add_library(server_lib STATIC ${SOURCE_FILES})
add_executable(server main.cpp)

target_link_libraries(server server_lib lib)

