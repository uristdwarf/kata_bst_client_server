cmake_minimum_required(VERSION 3.14)

set(SOURCE_FILES
	client_utils.cpp
	client.cpp
)

add_library(client_lib STATIC ${SOURCE_FILES})
add_executable(client main.cpp)

target_link_libraries(client client_lib lib)

