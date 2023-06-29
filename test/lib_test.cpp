#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include "../src/lib/socket.h"

TEST(LibTest, CreateRemoteSocket) {
	string ip = "127.0.0.1";
	string port = "5510";
	connection conn(ip, port);

	auto info = conn.get_info();
	auto fd = conn.get_fd();
	
	char ip_result[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, info.ai_addr->sa_data, ip_result, INET_ADDRSTRLEN);
	EXPECT_EQ(ip_result, ip);
	EXPECT_GT(fd, 0);
}

