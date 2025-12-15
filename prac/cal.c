#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>

int main() {
	system("mkdir -p /tmp/codes");
	uint32_t result, val1, val2;
	val1 = ~  1945618903U;
	val2 = 2121226598U;
	result = val1 << val2;

	printf("0x%x\n", result);
	printf("0x%x\n", ~  1945618903U  <<  (  (( 2121226598U )) ));

	printf("0x%x\n", 1 / ( 2-20+18));

	printf("0x%x\n", ( ((  0x2a1b3809U < ~ 636574516U )  | 712431447U )  << ~ 1867326510U  )  ^( ( (  1126749546U  > ! 580538507U )!=~ 641232135U  ^ ! 0x2e4f2313U <=( 0x09968507U  )  ==  ( 1623343393U)  >>  1338652346U | !  0x48655ba4U )  )  -  0x34cb205bU| ~  0x4723e7c7U);

	return 0;
}
