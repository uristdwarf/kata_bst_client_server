#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include "../src/lib/socket.h"

TEST(LibTest, CreateRemoteSocket) {
	string ip = "127.0.0.1";
	string port = "5510";
	connection conn(ip, port);

	auto info = conn.get_info();
	
	struct sockaddr_in *info_addr = (struct sockaddr_in*)info.ai_addr;

	auto ip_result = inet_ntoa(info_addr->sin_addr);
	EXPECT_EQ(ip_result, ip);
}

