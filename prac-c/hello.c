#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include <inttypes.h>

int main() {
	// printf("test[%s]\n");
	static char *line_read = NULL;

	int sum = 0;
	for (int t = 1; t <= 100; t ++) {
		sum += t;
	}
	errno = 0;
	char *nptr = "0x8000'0000U";
	char *end = NULL;
	uint64_t num = strtoull(nptr, &end, 0);
	printf("Hello World, %5lu, errno: %d\n", num, errno);

	line_read = readline("(test) ");
	printf("read result: %s\n", line_read);
	printf("length of line_read : %5zd[end]\n", strlen(line_read));

	char *cmd = strtok(line_read, " ");
	printf("line_read after strtok: %s[end]\n", line_read);
	__lsan_do_recoverable_leak_check();
	// free(line_read);

	// char *args = cmd + strlen(cmd) + 1;
	// printf("cmd is %s\nargs is %s[end]\n", cmd, args);
	// printf("length of cmd, args: %3zd, %3zd[end]\n", strlen(cmd), strlen(args));
	// printf("*args == 0 is %1d\n", (*args == 0));

	return 0;
}

