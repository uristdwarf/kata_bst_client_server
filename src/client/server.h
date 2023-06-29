#pragma once

#include "../lib/socket.h"

#define DATA_SIZE 1000

class server : connection {
	char buffer[DATA_SIZE];
	public:
	server(string address, string port);
	void send(string message);
	string recv();
	// An idea for multi threading: If we wanted the client to do something
	// else while it waited for server messages, we could use threads here.
	// I don't see any reason for it though.
};
