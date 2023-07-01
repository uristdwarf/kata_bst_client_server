#include "client.h"

client::client(string ip_address, string port) : connection(ip_address, port) {
		
}

client::~client() {
	
}

void client::send(string message) {

}

string client::recv() {
	return "";
}
