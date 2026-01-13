#include <stdio.h>
#include <inttypes.h>

#define BITMASK(bits) ((1ull << (bits)) - 1)

int main() {
	printf("%10.4s\n", "afsdfsdfasdfaf");
	double value = 100.21;
	long int_part = (long)value;
	double frac_part = value - int_part;
	printf("value: %.0lf\n", value);
	printf("int_part: %lu, frac_part: %lf\n", int_part, frac_part);
	return 0;

	uint32_t a = 0x000000ff;
	printf("8bit: %-d, 32bit: %d\n", (int8_t)a, (int32_t)a);
	printf("SEXT: %d\n", (uint32_t)(int8_t)a);

	printf("test 64bits: 0x%llx\n", BITMASK(64));
	return 0;
}

