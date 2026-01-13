#include <stdio.h>

int main() {
	int a = -1;
	int b = 4;
	printf("%d %% %d = %d\n", a, b, a % b);
	printf("(%d + b) %% %d = %d\n", a, b, (a + b) % b);
	return 0;
}

