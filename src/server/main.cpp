#include "server.h"

int main() {
	// TODO: Add option for port
	server server("5510");
	server.listen();
	return 0;
}
