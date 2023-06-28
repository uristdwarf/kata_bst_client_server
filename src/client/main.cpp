#include <stdio.h>

const char *USAGE = 
R""""(Usage:
client [HOSTNAME[:PORT]] [PORT]
)"""";

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("%s", USAGE);
		return 1;
	}
	printf("Hello World!\n");
	return 0;
}
