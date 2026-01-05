#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = 0;
  int max = 4;

  if (argc == 1) {
    printf("You didn't input any argument. You suck.\n");
  } else if (argc > 1 && argc < max) {
    printf("Here's your arguments:\n");

    for (i = 1; i < argc; i++) {
      printf("%s ", argv[i]);
    }
    printf("\n");
  } else {
    printf("You have too many arguments, expect less than %d. You suck.\n",
           max);
  }

  return 0;
}
