#include "socket.h"

connection::connection(string port) {

}

connection::connection(string address, string port) {

}

connection::~connection() {
	
}

struct addrinfo connection::get_info() {
	return *this->info;
}

int connection::get_fd() {
	return this->fd;
}
