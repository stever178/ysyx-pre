
#include <stdio.h>

int main () {
	char arr[4] = {'A', 'B', 'C', 'D'};
	int num = 65;
	printf("%d\n", (int)(arr+0));
	printf("%d\n", (int)(arr+1));

	printf("%d\n", (arr+0));
	printf("%d\n", (arr+1));

	printf("%d\n", (arr));
	return 0;
}
