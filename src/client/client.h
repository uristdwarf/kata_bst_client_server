#pragma once

#include "../lib/socket.h"
#include <exception>

#define DATA_SIZE 1000

class send_err : public exception {
	virtual const char * what() {
		return "failed sending data";
	}
};

class connection_closed : public exception {
	virtual const char * what() {
		return "remote has closed connection";
	}
};

class recv_err : public exception {
	virtual const char * what() {
		return "remote has closed connection";
	}
};

class client : connection {
	char buffer[DATA_SIZE];
	public:
	client(string address, string port);
	~client();
	void send(string message);
	string recv();
	// An idea for multi threading: If we wanted the client to do something
	// else while it waited for server messages, we could use threads here.
	// I don't see any reason for it though.
};
