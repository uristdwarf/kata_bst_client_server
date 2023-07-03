#include "gtest/gtest.h"
#include "../src/server/server.h"
#include <cstdio>
#include <netdb.h>
#include <sys/socket.h>

TEST(ParseCommandTests, ParseInsert) {
    auto [command, var] = parse_command("insert 10");
    EXPECT_EQ(command, "insert");
    EXPECT_EQ(var, 10);
}

TEST(ParseCommandTests, ParseFind) {
    auto [command, var] = parse_command("del 15");
    EXPECT_EQ(command, "del");
    EXPECT_EQ(var, 15);
}

TEST(ParseCommandTests, ParseDelete) {
    auto [command, var] = parse_command("find 55");
    EXPECT_EQ(command, "find");
    EXPECT_EQ(var, 55);
}

TEST(ParseCommandTests, ParseRubbish) {
    EXPECT_THROW({parse_command("bla bla lb");}, bad_command);
    EXPECT_THROW({parse_command("man");}, bad_command);
    EXPECT_THROW({parse_command("heyo adfs 43-150");}, bad_command);
    EXPECT_THROW({parse_command("dele 15");}, bad_command);
}

class ServerConnectionTests : public ::testing::Test {
    protected:
    struct addrinfo hints, *servinfo;
    int fd;
    server serv = server("5510");

    void SetUp() override {
	signal(SIGPIPE, SIG_IGN); // Prevent SIGPIPE
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	memset(&hints, 0, sizeof(hints));
	int return_value;
	if ((return_value = getaddrinfo("127.0.0.1", "5510", &hints, &servinfo) != 0)) {
	    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(return_value));
	    exit(254);
	}
	fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (fd == -1) {
	    perror("client: socket");
	    exit(254);
	}
    };
    void connect() {
	if (::connect(fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
	    perror("client: connect");
	    FAIL();
	}
    }
    void write(std::string message) {
	if (send(fd, message.data(), message.length(), 0) == -1) {
	    perror("send");
	    FAIL();
	}
    }
    std::string read() {
	char buffer[255] = "";
	if ((recv(fd, &buffer, 255, 0)) == -1) {
	    perror("recv");
	}
	return std::string(buffer);
    }
    void disconnect() {
	close(fd);
    }
    void TearDown() override {
	freeaddrinfo(servinfo);
    };
};

TEST_F(ServerConnectionTests, ConnectTest) {
    connect();
}

TEST_F(ServerConnectionTests, WriteTest) {
    connect();
    write("Hello!");
}

TEST_F(ServerConnectionTests, ReadTest) {
    connect();
    write("hello");
    auto str = read();
    ASSERT_NE(str, "");
}
