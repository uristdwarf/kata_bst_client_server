#include <stdio.h>
#include "client_utils.h"

const char *USAGE = 
R""""(Usage:
client [HOSTNAME[:PORT]] [PORT]
)"""";
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("%s", USAGE);
		return 1;
	}
	parse_hostname(argc, argv);
	printf("Hello World!\n");
	return 0;
}
