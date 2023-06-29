#include "socket.h"
#include <cstring>
#include <stdexcept>

connection::connection(string port) {

}

connection::connection(string address, string port) {
	struct addrinfo hints, *addresses;
	// Ensure hints is empty
	memset(&hints, 0, sizeof(hints));
	// TODO: Add IPV6 support
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status;
	if ((status = getaddrinfo(address.data(), port.data(), &hints,	&addresses) != 0)) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(254);
	}
	for (this->info = addresses; this->info != NULL; this->info = addresses->ai_next) {
		if (
		   (this->fd = socket(
		   	this->info->ai_family,
		   	this->info->ai_socktype,
		   	this->info->ai_protocol)
		   ) == -1)
		{
			perror("client: socket");
			continue;
		}
		break;
	}

	if (this->info == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		throw runtime_error("failed to connect");
	}
}

connection::~connection() {
	
}

struct addrinfo connection::get_info() {
	return *this->info;
}
