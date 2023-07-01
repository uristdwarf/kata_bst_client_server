#include "socket.h"
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <stdexcept>
#include <unistd.h>

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
	for (info = addresses; info != NULL; info = addresses->ai_next) {
		if (
		   (fd = socket(
		   	info->ai_family,
		   	info->ai_socktype,
		   	info->ai_protocol)
		   ) == -1)
		{
			perror("client: socket");
			continue;
		}
		break;
	}

	if (info == NULL) {
		fprintf(stderr, "client: failed to open socket\n");
		throw socket_error();
	}
}

connection::~connection() {
	freeaddrinfo(info);
	close(fd);
}

struct addrinfo connection::get_info() {
	return *info;
}

struct addrinfo connection::next_addr() {
	info = info->ai_next;
	if (info == NULL) {
		throw runtime_error("no more addrinfo available");
	}
	close(fd);
	if ((fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) == -1) {
		perror("client: socket");
		throw socket_error();
	}
	return *info;
}
