CC = gcc
CFLAGS = -ansi -pedantic -Wall -g

main: main.o pre-assembler.o errors-checker.o
	$(CC) $(CFLAGS) -o main main.o pre-assembler.o errors-checker.o
test: test.o acutest.h
	$(CC) -o test test.o pre-assembler.o errors-checker.o
test.o: test.c
	$(CC) -c test.c
pre-assembler.o: assembler.h pre-assembler.c 
	$(CC) $(CFLAGS) -c pre-assembler.c
errors-checker.o: assembler.h errors-checker.c 
	$(CC) $(CFLAGS) -c errors-checker.c	
clean:
	rm -f core *.o main test