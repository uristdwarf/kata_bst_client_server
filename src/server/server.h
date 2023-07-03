#include "../lib/socket.h"
#include <stdexcept>

tuple<string, int> parse_command(string command);

class bad_command : public std::runtime_error {
	bad_command(string message) : std::runtime_error(message) {};
};

class server_client {
	int fd;
	string remote_addr;
	server_client();
	~server_client();
};

class server : public connection {
	public:
	server(string p);
	void listen();
};
