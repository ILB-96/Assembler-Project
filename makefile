CC = gcc
CFLAGS = -ansi -pedantic -Wall -g
assembler: command_process.o data_struct_words.o assembler.o pre_assembler.o first_pass.o second_pass.o functions.o label_struct.o macros_struct.o
	$(CC) $(CFLAGS) command_process.o data_struct_words.o  assembler.o pre_assembler.o first_pass.o second_pass.o functions.o label_struct.o macros_struct.o -o assembler
assembler.o: assembler.c assembler.h
	$(CC) $(CFLAGS) -c assembler.c
functions.o: functions.c assembler.h
	$(CC) $(CFLAGS) -c functions.c
label_struct.o: label_struct.c assembler.h
	$(CC) $(CFLAGS) -c label_struct.c
macros_struct.o: macros_struct.c assembler.h
	$(CC) $(CFLAGS) -c macros_struct.c
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
	rm -f core *.o assembler
fclean:
	rm -f core *.ob *.ext *.ent *.am