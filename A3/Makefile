CC = gcc

all: deadlock 

deadlock: vorgabe3.o aufgabe3_main.o aufgabe3_semaphores.o aufgabe3_memory.o aufgabe3_threads.o aufgabe3_deadlock.o aufgabe3_sanity.o
	${CC} -Wall $^ -o deadlock -pthread

%.o: %.c vorgabe3.h
	${CC} -Wall $< -c -o $@

clean:
	rm -f *.o
	rm -f deadlock
