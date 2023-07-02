#include <cstring>
#include <exception>
#include <iostream>
#include <stdio.h>
#include <memory>
#include "client.h"
#include <signal.h>
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
	// Ignore sigpipe errors
	signal(SIGPIPE, SIG_IGN);
	try {
		auto [address, port] = parse_hostname(argc, argv);
		client server = client(address, port);
		cout << "Connected to " << address << " on port " << port << endl;
		cout << "Welcome! Please enter 'help' to see available commands (NOT YET IMPLEMENTED)" << endl;
		cout << "Enter 'exit' to quit" << endl;
		while (1) {
			cout << "> ";
			string command = "";
			cin >> command;
			if (command == "exit") {
				break;
			}
			server.send(command);
			string msg = server.recv();
			cout << "remote: " << msg << endl;
		}
	} catch (connection_err& e) {
		cout << e.what() << endl;
		return 2;
	} catch (client_err& e) {
		cout << "failed communicating with server: " << e.what() << endl;

	}
	return 0;
}
