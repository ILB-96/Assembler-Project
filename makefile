CC = gcc
CFLAGS = -ansi -pedantic -Wall -g

main: main.o pre-assembler.o
	$(CC) $(CFLAGS) -o main main.o pre-assembler.o
test.o: acutest.h test.c
	$(CC) -c test.c
test: test.o acutest.h
	$(CC) -o test test.o pre-assembler.o
pre-assembler.o: assembler.h pre-assembler.c 
	$(CC) $(CFLAGS) -c pre-assembler.c
clean:
	rm -f core *.o main test