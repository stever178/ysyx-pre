#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<errno.h>

int main(int argc, char* argv[]) {
	printf("%d\n", 1 % 0);
	return 0;
    FILE* fp = popen("./a.out", "r");
    assert(fp != NULL);

    int ret, result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    if (ret == EOF) {
		printf("invalid, errno is %d \n", errno);
    }
	//printf("%u\n", 1 / 0);
	return 0;
}
