#include <gtest/gtest.h>
#include "../src/client/client_utils.h"

TEST(ClientTests, ParseSimpleHost) {
	string ip_address = "127.0.0.1";
	char *argv[] = {(char*)"client", ip_address.data()};
	int argc = sizeof(argv);
	
	// Default port will be 5510
	auto result = parse_hostname(argc, argv);
	EXPECT_EQ(result, ip_address + ":5510");
}

TEST(ClientTests, ParseHostWithPort) {
	string ip_address = "127.0.0.1:1234";
	char *argv[] = {(char*)"client", ip_address.data()};
	int argc = sizeof(argv);
	
	auto result = parse_hostname(argc, argv);
	EXPECT_EQ(result, ip_address);
}

TEST(ClientTests, ParseHostAndPort) {
	string ip_address = "127.0.0.1";
	string port = "6789";
	char *argv[] = {(char*)"client", ip_address.data(), port.data()};
	int argc = sizeof(argv);
	
	auto result = parse_hostname(argc, argv);
	EXPECT_EQ(result, ip_address + ":" + port);
}
