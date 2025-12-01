#include <stdio.h>

int main() {
	int sum = 0;
	for (int t = 1; t <= 100; t ++) {
		sum += t;
	}
	printf("Hello World, %5d\n", sum);

	return 0;
}

