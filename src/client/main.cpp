#include <cstring>
#include <stdio.h>
#include "client_utils.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define DATA_SIZE 1000

const char *USAGE = 
R""""(Usage:
client [HOSTNAME[:PORT]] [PORT]
)"""";
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("%s", USAGE);
		return 1;
	}
	auto [address, port] = parse_hostname(argc, argv);

	struct addrinfo hints, *result, *result_port;
	memset(&hints, 0, sizeof(hints));
	// TODO: Add IPV6 support
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status;
	if ((status = getaddrinfo(address.data(), port.data(), &hints,	&result) != 0)) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		return 254;
	}

	int socket_fd;
	for (result_port = result; result_port != NULL; result_port = result_port->ai_next) {
		if (
		   (socket_fd = socket(
		   	result_port->ai_family,
		   	result_port->ai_socktype,
		   	result_port->ai_protocol)
		   ) == -1)
		{
			perror("client: socket");
			continue;
		}
		if (connect(socket_fd, result_port->ai_addr, result_port->ai_addrlen) == -1) {
			close(socket_fd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (result_port == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 3;
	}
	
	const char *message = "Ping\n";
	size_t length = strlen(message);
	char buffer[DATA_SIZE];

	while (1) {
		int sent = send(socket_fd, message, length, 0);
		printf("client: %s", message);
		if (sent == -1) {
			// TODO: Handle server side closures
			perror("send");
			break;
		} else if (sent != 0) {
			// TODO: Handle large messages
		}
		int received = recv(socket_fd, &buffer, DATA_SIZE, 0);
		printf("server: %s", buffer);
	}
	

	freeaddrinfo(result);
	close(socket_fd);
	return 0;
}
