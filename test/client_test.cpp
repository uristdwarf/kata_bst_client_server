#include <cstddef>
#include <cstdio>
#include <gtest/gtest.h>
#include "../src/client/client_utils.h"
#include "../src/client/server.h"

TEST(ClientTests, ParseSimpleHost) {
	string ip_address = "127.0.0.1";
	char *argv[] = {(char*)"client", ip_address.data()};
	// Default port will be 5510
	string port = "5510";
	int argc = sizeof(argv)/sizeof(argv[0]);
	
	auto [host, port_result] = parse_hostname(argc, argv);
	EXPECT_EQ(host, ip_address);
	EXPECT_EQ(port, port_result);
}

TEST(ClientTests, ParseHostWithPort) {
	string ip_address = "127.0.0.1:1234";
	string port = "1234";
	char *argv[] = {(char*)"client", ip_address.data()};
	int argc = sizeof(argv)/sizeof(argv[0]);
	
	auto [host, port_result] = parse_hostname(argc, argv);
	EXPECT_EQ(host, ip_address);
	EXPECT_EQ(port, port_result);
}

TEST(ClientTests, ParseHostAndPort) {
	string ip_address = "127.0.0.1";
	string port = "6789";
	char *argv[] = {(char*)"client", ip_address.data(), port.data()};
	int argc = sizeof(argv)/sizeof(argv[0]);
	
	auto [host, port_result] = parse_hostname(argc, argv);
	EXPECT_EQ(host, ip_address);
	EXPECT_EQ(port_result, port);
}

// We can do tests for parse_hostname all day, but for right now let's just finish up the client shall we?
// TODO: Add test with invalid port

TEST(ClientTests, ReadFromNetCat) {
	string message = "Hello!";
	const size_t BUFFER_SIZE = sizeof("Hello!");
	char buffer[BUFFER_SIZE];
	auto stream = popen("nc -lv 5510", "r");
	server srv("127.0.0.1", "5510");

	srv.send(message);
	fread(buffer, 1, sizeof(buffer), stream);	
	EXPECT_EQ(buffer, message);
}

TEST(ClientTests, WriteFromNetCat) {	
	auto stream = popen("nc -lv 5510", "w");
	server srv("127.0.0.1", "5510");
	string message = "Hello!";
	
	fwrite(message.data(), sizeof(message[0]), message.length(), stream);
	string res = srv.recv();
	ASSERT_EQ(res, message);
}
