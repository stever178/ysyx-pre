#include <stdlib.h>

int main() {
	int i = 0;
	while (1) {
		malloc(1024);
		i ++;
		printf(" allocate %d bytes \n ", i);
	}

	return 0;
}
