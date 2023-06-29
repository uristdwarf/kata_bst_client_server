cmake_minimum_required(VERSION 3.14)

set(SOURCE_FILES
	socket.cpp
)

add_library(lib STATIC ${SOURCE_FILES})
