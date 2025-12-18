#include <stdio.h>
#include <ctype.h>
#include "dbg.h"

#define MAX_LEN 1024

int print_a_message(const char *msg)
{
    printf("A STRING: %s\n", msg);

    return 0;
}


int uppercase(const char *msg)
{
    check(msg != NULL, "You didn't pass a message over.");
    check(strlen(msg) < MAX_LEN, "Message is too long, max is %d", MAX_LEN);   

    int i = 0;

    // BUG: \0 termination problems
    for(i = 0; msg[i] != '\0'; i++) {
        printf("%c", toupper(msg[i]));
    }
    printf("\n");
    return 0;

error:
    return 1;
}

int lowercase(const char *msg)
{
    check(msg != NULL, "You didn't pass a message over.");
    check(strlen(msg) < MAX_LEN, "Message is too long, max is %d", MAX_LEN);   

    int i = 0;

    // BUG: \0 termination problems
    for(i = 0; msg[i] != '\0'; i++) {
        printf("%c", tolower(msg[i]));
    }
    printf("\n");
    return 0;

error:
    return 1;
}

int fail_on_purpose(const char *msg)
{
    (void)msg;
    return 1;
}
