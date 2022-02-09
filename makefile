CC = gcc
CFLAGS = -ansi -pedantic -Wall -g

main: main.o pre-assembler.o labels-table.o
	$(CC) $(CFLAGS) -o main main.o pre-assembler.o labels-table.o
test: test.o acutest.h
	$(CC) -o test test.o pre-assembler.o labels-table.o
test.o: test.c
	$(CC) -c test.c
pre-assembler.o: assembler.h pre-assembler.c 
	$(CC) $(CFLAGS) -c pre-assembler.c
labels-table.o: assembler.h labels-table.c
	$(CC) $(CFLAGS) -c labels-table.c
clean:
	rm -f core *.o main test