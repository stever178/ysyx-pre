#include <lcthw/dbg.h>
#include <lcthw/utils.h>

char *gen_random_string(int length) {
  char *str = malloc(sizeof(char) * length);
  
  const int num = 126 - 33 + 1;
  int i = 0;
  for (; i < length - 1; i += 2) {
    str[i] = str[i + 1] = 33 + (rand() % num);
  }

  // if (i == length - 1) {
  //   str[i] = 'a' + (rand() % 26);
  // }
  str[length - 1] = '\0';

  return str;
}

void free_string(char *str) {
  assert(str != NULL);
  free(str);
}
