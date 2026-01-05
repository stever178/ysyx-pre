
#include <stdio.h>

int main() {
  int numbers[4] = {0};
  char name[4] = {'a'};

  // first, print them out raw
  printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2],
         numbers[3]);

  printf("name each: %c %c %c %c\n", name[0], name[1], name[2], name[3]);

  printf("name: %s\n", name);

  // setup the numbers
  numbers[0] = '1';
  numbers[1] = '2';
  numbers[2] = '3';
  numbers[3] = '4';
  for (int i = 0; i < 4; i++) {
    printf("numbers[%d] = %c\n", i, numbers[i]);
  }

  // setup the name
  name[0] = 'Z';
  name[1] = 'e';
  name[2] = 'd';
  name[3] = '\0';
  for (int i = 0; i < 4; i++) {
    printf("name[%d] = %d\n", i, name[i]);
  }

  // then print them out initialized
  printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2],
         numbers[3]);

  printf("name each: %c %c %c %c\n", name[0], name[1], name[2], name[3]);

  // print the name like a string
  printf("name: %s\n", name);

  // another way to use name
  char *another = "Zed";

  printf("another: %s\n", another);

  printf("another each: %c %c %c %c\n", another[0], another[1], another[2],
         another[3]);

  char arr[4] = {'A', 'B', 'C', 'D'};
  // int num = 65;
  printf("%x\n", *(int *)(arr + 0));
  printf("%x\n", *(int *)(arr + 1));

  printf("%p\n", (arr + 0));
  printf("%p\n", (arr + 1));

  return 0;
}
