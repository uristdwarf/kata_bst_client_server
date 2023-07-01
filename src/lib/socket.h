#pragma once

#include <exception>
#include <netdb.h>
#include <string>


using namespace std;

class socket_error : exception {
	virtual const char* what() {
		return "failed to open socket";
	}
};
// Represents a socket connection. No connection is actually made
// and the server/client are responsible for actually connecting
// and listening/writing
class connection {
	// Information about remote peer;
	struct addrinfo *info;

	protected:
	// File descriptor
	int fd;
	// Close socket, get next address and open socket for that.
	// Throws exception if next addrinfo is NULL.
	addrinfo next_addr();

	public:
	// Create connection on own host
	connection(string port);
	// Create connection on remote host
	connection(string address, string port);

	addrinfo get_info();

	~connection();
};

