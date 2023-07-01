#include <arpa/inet.h>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <gtest/gtest.h>
#include <thread>
#include <unistd.h>
#include "../src/client/client_utils.h"
#include "../src/client/client.h"

class ArgumentParsingTest : public ::testing::Test {
	protected:
	string ip_address = "127.0.0.1";
};

TEST_F(ArgumentParsingTest, ParseSimpleHost) {
	char *argv[] = {(char*)"client", ip_address.data()};
	// Default port will be 5510
	string port = "5510";
	int argc = sizeof(argv)/sizeof(argv[0]);
	
	auto [host, port_result] = parse_hostname(argc, argv);
	EXPECT_EQ(host, ip_address);
	EXPECT_EQ(port, port_result);
}

TEST_F(ArgumentParsingTest, ParseHostWithPort) {
	string port = "1234";
	string address_port = ip_address + ":" + port;
	char *argv[] = {(char*)"client", address_port.data()};
	int argc = sizeof(argv)/sizeof(argv[0]);
	
	auto [host, port_result] = parse_hostname(argc, argv);
	EXPECT_EQ(host, address_port);
	EXPECT_EQ(port, port_result);
}

TEST_F(ArgumentParsingTest, ParseHostAndPort) {
	string port = "6789";
	char *argv[] = {(char*)"client", ip_address.data(), port.data()};
	int argc = sizeof(argv)/sizeof(argv[0]);
	
	auto [host, port_result] = parse_hostname(argc, argv);
	EXPECT_EQ(host, ip_address);
	EXPECT_EQ(port_result, port);
}

// We can do tests for parse_hostname all day, but for right now let's just finish up the client shall we?
// TODO: Add test with invalid port


class ClientTests : public ::testing::Test {
	protected:
	// I spent 3 hours trying to get a netcat listening proccess to work with piping,
	// ended up being a waste of time so I am copying the whole of Beej's Guide To 
	// Network Programming A Simple Stream Server section with few modifications.
	// Could have also Boost to be honest.
	// TODO: Organise this
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    	struct addrinfo hints, *servinfo, *p;
    	struct sockaddr_storage their_addr; // connector's address information
    	socklen_t sin_size;
    	int yes=1;
    	char s[INET6_ADDRSTRLEN];
    	int rv;
	thread server;
	char buffer[256];
	string server_message = "Let's play ping pong!";
	
	
	// get sockaddr, IPv4 or IPv6:
	void *get_in_addr(struct sockaddr *sa)
	{
	    if (sa->sa_family == AF_INET) {
	        return &(((struct sockaddr_in*)sa)->sin_addr);
	    }
	
	    return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}

	void SetUp() override {
    		memset(&hints, 0, sizeof hints);
    		hints.ai_family = AF_UNSPEC;
    		hints.ai_socktype = SOCK_STREAM;
    		hints.ai_flags = AI_PASSIVE; // use my IP

    		if ((rv = getaddrinfo(NULL, "5510", &hints, &servinfo)) != 0) {
    		    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		    FAIL();
    		}

    		// loop through all the results and bind to the first we can
    		for(p = servinfo; p != NULL; p = p->ai_next) {
    		    if ((sockfd = socket(p->ai_family, p->ai_socktype,
    		            p->ai_protocol)) == -1) {
    		        perror("server: socket");
    		        continue;
    		    }

    		    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
    		            sizeof(int)) == -1) {
    		        perror("setsockopt");
    		        FAIL();
    		    }

    		    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
    		        close(sockfd);
    		        perror("server: bind");
    		        continue;
    		    }

    		    break;
    		}

    		freeaddrinfo(servinfo); // all done with this structure

    		if (p == NULL)  {
    		    fprintf(stderr, "server: failed to bind\n");
		    FAIL();
    		}

    		if (listen(sockfd, 1) == -1) {
    		    perror("listen");
    		    FAIL();
    		}

    		server = thread([&] {
			sin_size = sizeof their_addr;
    			new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    			if (new_fd == -1) {
    			    perror("accept");
			    return;
    			}
    			inet_ntop(their_addr.ss_family,
    			    get_in_addr((struct sockaddr *)&their_addr),
    			    s, sizeof s);


    			if (send(new_fd, server_message.data(), 256, 0) == -1) {
			    perror("send");
			}

			if (recv(new_fd, &buffer, 256, 0) == -1) {
			    perror("recv");
			}

    			if (send(new_fd, buffer, 256, 0) == -1) {
			    perror("send");
			}

    			close(sockfd);
    			close(new_fd); 
		});
	}
	void TearDown() override {
	    server.join();
	}
};

TEST_F(ClientTests, ReadFromServer) {
	string message = "Hello!";
	const size_t BUFFER_SIZE = sizeof("Hello!");
	client server("127.0.0.1", "5510");
	
	// server won't read our message until we receive their initial message
	server.recv();
	server.send(message);
	EXPECT_EQ(message, string(buffer));
}

TEST_F(ClientTests, WriteFromServer) {	
	client server("127.0.0.1", "5510");
	string res = server.recv();
	EXPECT_EQ(res, server_message);
}
