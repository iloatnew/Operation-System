# -= Variablen =-
# Name=Wert oder auch 
# Name+=Wert fuer Konkatenation

CC ?= gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g

TARGETS = 4a.c 4b.c 4c.c bst.c test_4a.c test_4b.c test_4c.c
OBJECTS = ${TARGETS:.c=.o}

all: ${OBJECTS}

include deps.mk

test_4a: test_4a.o 4a.o bst.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

test_4b: test_4b.o 4b.o 4a.o bst.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

test_4c: test_4c.o 4c.o 4b.o 4a.o bst.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

4a: test_4a
	@./test_4a

4b: test_4b
	@./test_4b

4c: test_4c
	@./test_4c

clean:
	rm -f test_4a test_4b test_4c ${OBJECTS}

.PHONY: 4a 4b 4c all clean
