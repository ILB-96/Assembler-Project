<<<<<<< HEAD
CC = gcc
CFLAGS = -ansi -pedantic -Wall -g
assembler: command_process.o data_struct_words.o main.o pre_assembler.o first_pass.o second_pass.o functions.o
	$(CC) $(CFLAGS) command_process.o data_struct_words.o  main.o pre_assembler.o first_pass.o second_pass.o functions.o -o main
main.o: main.c assembler.h
	$(CC) $(CFLAGS) -c main.c
functions.o: functions.c assembler.h
	$(CC) $(CFLAGS) -c functions.c
pre_assembler.o: pre_assembler.c assembler.h
	$(CC) $(CFLAGS) -c pre_assembler.c
first-pass.o: first_pass.c assembler.h
	$(CC) $(CFLAGS) -c first_pass.c
second-pass.o: second_pass.c assembler.h
	$(CC) $(CFLAGS) -c second_pass.c
command_process.o: command_process.c assembler.h
	gcc -c command_process.c -o command_process.o
data_struct_words.o: data_struct_words.c assembler.h
	gcc -c data_struct_words.c -o data_struct_words.o

test: test.o functions.o pre_assembler.o first_pass.o second_pass.o
	$(CC) $(TFLAGS) -o test test.o functions.o pre_assembler.o first_pass.o second_pass.o
test.o: test.c assembler.h
	$(CC) $(TFLAGS) -c test.c
clean:
	rm -f core *.o main test
=======
assembler: command_process.o data_struct_words.o main.o pre-assembler.o first-pass.o second-pass.o
	gcc -g -Wall -ansi -pedantic command_process.o data_struct_words.o  main.o pre-assembler.o first-pass.o -o main
command_process.o: command_process.c assembler.h
	gcc -c command_process.c -o command_process.o
data_struct_words.o: data_struct_words.c assembler.h
	gcc -c data_struct_words.c -o data_struct_words.o
main.o: main.c assembler.h
	gcc -c main.c -o main.o
pre-assembler.o: pre-assembler.c assembler.h
	gcc -c pre-assembler.c -o assembler.o
first-pass.o: first-pass.c assembler.h
	gcc -c first-pass.c -o first-pass.o
second-pass.o: second-pass.c assembler.h
	gcc -c second-pass.c -o second-pass.o
>>>>>>> 4c1522b3209ce145f637e1a320c288e717cd4f7e
