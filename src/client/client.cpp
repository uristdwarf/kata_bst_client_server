#include "client.h"
#include <sys/socket.h>

#define DATA_SIZE 1000

client::client(string ip_address, string port) : connection(ip_address, port) {
	auto info = get_info();
	while (connect(fd, info.ai_addr, info.ai_addrlen) == -1) {
		perror("connect");
		try {
			next_addr();
		} catch (socket_error) {} // socket might not work on this address
	}
}

client::~client() {
	
}

void client::send(string message) {
	int sent = ::send(fd, message.data(), message.length(), 0);
	// printf("client: %s", message.data());
	if (sent == -1) {
		// TODO: Handle server side closures
		throw send_err();
	} else if (sent != 0) {
		send(message.substr(sent));
	}
}

string client::recv() {
	int received = ::recv(fd, &buffer, DATA_SIZE, 0);
	if (received == 0) {
		throw connection_closed();
	} else if (received == -1) {
		throw recv_err();
	}
	return string(buffer);
}
