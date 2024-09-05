
CC=gcc
CFLAGS=-Wall -g

SOURCES := $(wildcard ex*.c)
OBJECTS := $(SOURCES:.c=.o)
OUT := ex1 ex3

all: ${OUT}
	gcc ex1.c -o ex1
	gcc ex3.c -o ex3

clean:
	@rm -f ex1
	find . -type f -executable -delete
