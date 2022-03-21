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
	$(CC) $(CFLAGS) -c command_process.c
data_struct_words.o: data_struct_words.c assembler.h
	$(CC) $(CFLAGS) -c data_struct_words.c

clean:
	rm -f core *.o main test
fclean:
	rm -f core *.ob *.ext *.ent *.am