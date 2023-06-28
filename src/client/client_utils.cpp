#include "client_utils.h"
#include <cstdio>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>

tuple<string, int> parse_hostname(int argc, char **argv) {
	string hostname = argv[1];
	int port;
	string delimiter = ":";
	size_t pos = hostname.find(delimiter);

	if (pos == string::npos) {
		if (argc == 2) {
			// Default port to use
			// TODO: Add a #define for this
			port = 5510;
		} else {
			port = stoi(argv[2]);
		}
	} else {
		port = stoi(hostname.substr(pos + delimiter.length()));
	}

	return {hostname, port};
}

