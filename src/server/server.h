#include "../lib/socket.h"
#include <map>
#include <stdexcept>
#include <sys/socket.h>
#include <thread>

tuple<string, int> parse_command(string command);

class bad_command : public std::runtime_error {
	public:
	bad_command(string message) : std::runtime_error(message) {};
};

class server_error : public std::runtime_error {
	public:
	server_error(string message) : std::runtime_error(message) {};
};

class server_client_error : public std::runtime_error {
	public:
	server_client_error(string message) : std::runtime_error(message) {};
};

class server_client_closed : public server_client_error {
	public:
	server_client_closed() : server_client_error("client has closed the connection") {};
};

struct server_client {
	int fd;
	thread handler;
	struct sockaddr_storage address;
	string str_address;
	server_client(int fd, sockaddr_storage their_address);
	server_client(server_client &);
	void write_to_client(string message);
	string read_from_client();
	~server_client();
};

class server : public connection {
	map<int, server_client*> clients;
	void handle_client(int remote_fd);
	public:
	server(string p);
	~server();
	void listen(atomic_bool& run);
};
