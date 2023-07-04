#include "server.h"
#include "../lib/bst.h"
#include <arpa/inet.h>
#include <functional>
#include <map>
#include <memory>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <thread>

// TODO: Put this into a single header file
#define DATA_SIZE 1000

tuple<string, int> parse_command(string command) {
	size_t pos = command.find(' ');
	if (pos == string::npos) {
		throw bad_command("Invalid argument");
	}
	std::string cmd = command.substr(0, pos);
	command.erase(0, pos + 1);
	command.erase(command.find_last_not_of(" \n\r\t")+1);
	int variable;
	try {
		variable = stoi(command);
	} catch (std::invalid_argument) {
		throw bad_command("Invalid argument to command");
	}
	return tuple(cmd, variable);
}

shared_ptr<bst> tree;

string handle_tree(string command, int key) {
	if (!tree && command == "insert") {
		tree.reset(new bst(key));
		return "New tree initialised with key " + to_string(key);
	} else if (!tree) {
		return "ERROR: No tree exists";
	}

	// TODO: Use map functions (somehow?)
	if (command == "insert") {
		tree->insert(key);
	} else if (command == "find") {
		if (tree->find(key)) {
			return "Found";
		} else {
			return "Not found";
		}
	} else if (command == "delete") {
		try {
			tree->del(key);
		} catch (last_node e) {
			tree.reset();
			return "Last key removed; tree uninitialized";
		}
	} else {
		return "ERROR: command is invalid";
	}

	return "OK";
}

server::server(string port) : connection("127.0.0.1", port) {
	int yes=1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
	    throw server_error("failed setting socket options");
        }
	auto info = get_info();
	while (bind(fd, info.ai_addr, info.ai_addrlen) == -1) {
		perror("bind");
		try {
			next_addr();
		} catch (socket_error) {} // socket might not work on this address
		catch (addresses_exhausted e) {
			throw server_error("failed do bind to any available address");
		}
	}
}

void server::listen(std::atomic_bool& run) {
	int e = ::listen(fd, 5);
	if (e == -1) {
		perror("listen:");
		throw server_error("failed do listen");
	}
	while (run) {
		struct sockaddr_storage their_addr;
		socklen_t addr_size = sizeof(their_addr);
		const int remote_fd = accept(fd, (struct sockaddr *)&their_addr, &addr_size);
		server_client *client = new server_client(remote_fd, their_addr);
		clients[remote_fd] = client;
		client->handler = std::thread([this, remote_fd] {handle_client(remote_fd);});
	}
}

void server::handle_client(int fd) {
	auto client = clients[fd];
	while (1) {
		try {
			auto msg = client->read_from_client();
			auto [cmd, var] = parse_command(msg);
			auto result = handle_tree(cmd, var);
			client->write_to_client(result);
		} catch (server_client_closed e) {
			printf("client %s has closed the connection\n", client->str_address.data());
			break;

		} catch (server_client_error e) {
			printf("failed to read/write from client: %s\n", e.what());
			break;
		} catch (exception e) {
			try {
				client->write_to_client("ERROR: " + string(e.what()));
			} catch (server_client_error) {
				printf("failed to write to client %s\n", client->str_address.data());
				break;
			}
		}
	}
	clients.erase(fd);
	delete client;
};

server::~server() {
	for (const auto &[k, v] : clients) {
		close(k);
		v->handler.join();
	};
	close(fd);
}

server_client::server_client(int fd, sockaddr_storage their_addr) {
	this->fd = fd;
	address = their_addr;
	socklen_t addr_size = sizeof(their_addr);
	auto socket_address = (struct sockaddr *)&their_addr;

	// Get peer address as string
	int peer_address = getpeername(fd, socket_address, &addr_size);
	switch(socket_address->sa_family) {
		case AF_INET:
			char buffer_ipv4[INET_ADDRSTRLEN];
			inet_ntop(AF_INET,
				  &(((struct sockaddr_in *)socket_address)->sin_addr),
				  buffer_ipv4,
				  INET_ADDRSTRLEN);
			str_address = string(buffer_ipv4);
			break;
		case AF_INET6:
			char buffer_ipv6[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6,
				  &(((struct sockaddr_in6 *)socket_address)->sin6_addr),
				  buffer_ipv6,
				  INET6_ADDRSTRLEN);
			str_address = string(buffer_ipv6);
		       break;
		default:

            break;
	}
	// Copied from stackoverflow at 3:45 AM
	// https://stackoverflow.com/a/4182564
	// Set timeout
	// struct timeval timeout;
	// timeout.tv_sec = 10;
	// timeout.tv_usec = 0;
	// if (setsockopt (fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0)
	// 	perror("setsockopt failed:");

	// if (setsockopt (fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout) < 0)
	// 	perror("setsockopt failed:");
}

void server_client::write_to_client(string message) {
	int amount_send = send(fd, message.data(), message.length(), 0);
	if (amount_send == -1) {
		perror("server");
		throw (server_client_error("failed do write to client"));
	} else if (amount_send != 0) {
		write_to_client(message.substr(amount_send));
	}
}

string server_client::read_from_client() {
	char buffer[DATA_SIZE];
	int err = recv(fd, buffer, DATA_SIZE, 0);
	if (err == 0) {
		printf("client has closed connection\n");
		throw server_client_closed();
	} else if (err == -1) {
		perror("server");
		throw (server_client_error("failed do read from client"));
	}
	return string(buffer);
}

server_client::server_client(server_client &T) {
	fd = T.fd;
	address = T.address;
	str_address = T.str_address;
}

server_client::~server_client() {
	close(fd);
	handler.join();
}
