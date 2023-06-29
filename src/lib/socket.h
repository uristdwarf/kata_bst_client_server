#pragma once

#include <netdb.h>
#include <string>

using namespace std;
// Represents a socket connection. No connection is actually made
// and the server/client are responsible for actually connecting
// and listening/writing
class connection {
	struct addrinfo *info;

	protected:
	// File descriptor
	int fd;
	// Information about remote peer;

	public:
	// Create connection on own host
	connection(string port);
	// Create connection on remote host
	connection(string address, string port);

	addrinfo get_info();

	~connection();
};

