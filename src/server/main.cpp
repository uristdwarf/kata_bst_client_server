#include "server.h"

int main() {
	// TODO: Add option for port
	server server("5510");
	std::atomic_bool run_server = true;
	server.listen(run_server);
	return 0;
}
