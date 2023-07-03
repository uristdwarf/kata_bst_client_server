#include "server.h"
#include "../lib/bst.h"
#include <memory>

tuple<string, int> parse_command(string command) {
	return tuple("", 0);
}

shared_ptr<bst> tree;

server::server(string port) : connection("127.0.0.1", port) {
}

void server::listen() {
}
