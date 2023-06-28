#include <gtest/gtest.h>
#include "../src/client/client_utils.h"

TEST(ClientTests, ParseSimpleHost) {
	string ip_address = "127.0.0.1";
	char *argv[] = {(char*)"client", ip_address.data()};
	int argc = sizeof(argv);
	
	auto result = parse_hostname(argc, argv);
	EXPECT_EQ(result, ip_address + ":5510");
}
