#include<inttypes.h>
#include<stdlib.h>
#include<stdio.h>

int actual_calc(int a, int b){
	int c;
	c=a/b;
	return 0;
}

int calc(){
	int a;
	int b;
	a=13;
	b=0;
	actual_calc(a, b);
	return 0;
}

int main(){
	// calc();
	int a[2] = {1, 2};
	uint64_t num = strtoull("a", NULL, 10);
	printf("%2" PRIu64 "\n", num);
	return 0;
}

