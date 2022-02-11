CC = gcc
CFLAGS = -ansi -pedantic -Wall -g
TFLAGS =
main: main.o pre-assembler.o first-pass.o second-pass.o
	$(CC) $(CFLAGS) -o main main.o pre-assembler.o first-pass.o second-pass.o
main.o: main.c assembler.h
	$(CC) $(CFLAGS) -c main.c
pre-assembler.o: pre-assembler.c assembler.h
	$(CC) $(CFLAGS) -c pre-assembler.c
first-pass.o: first-pass.c assembler.h
	$(CC) $(CFLAGS) -c first-pass.c
second-pass.o: second-pass.c assembler.h
	$(CC) $(CFLAGS) -c second-pass.c

test: test.o  pre-assembler.o first-pass.o second-pass.o
	$(CC) $(TFLAGS) -o test test.o pre-assembler.o first-pass.o second-pass.o
test.o: test.c assembler.h
	$(CC) $(TFLAGS) -c test.c
clean:
	rm -f core *.o main test