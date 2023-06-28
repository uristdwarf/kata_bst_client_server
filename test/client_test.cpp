#include <gtest/gtest.h>
#include "../src/client/client_utils.h"

TEST(ClientTests, ParseSimpleHost) {
	string ip_address = "127.0.0.1";
	char *argv[] = {(char*)"client", ip_address.data()};
	// Default port will be 5510
	int port = 5510;
	int argc = sizeof(argv)/sizeof(argv[0]);
	
	auto [host, port_int] = parse_hostname(argc, argv);
	EXPECT_EQ(host, ip_address);
	EXPECT_EQ(port, port_int);
}

TEST(ClientTests, ParseHostWithPort) {
	string ip_address = "127.0.0.1:1234";
	int port = 1234;
	char *argv[] = {(char*)"client", ip_address.data()};
	int argc = sizeof(argv)/sizeof(argv[0]);
	
	auto [host, port_int] = parse_hostname(argc, argv);
	EXPECT_EQ(host, ip_address);
	EXPECT_EQ(port, port_int);
}

TEST(ClientTests, ParseHostAndPort) {
	string ip_address = "127.0.0.1";
	string port = "6789";
	char *argv[] = {(char*)"client", ip_address.data(), port.data()};
	int argc = sizeof(argv)/sizeof(argv[0]);
	
	auto [host, port_int] = parse_hostname(argc, argv);
	EXPECT_EQ(host, ip_address);
	EXPECT_EQ(port_int, stoi(port));
}
