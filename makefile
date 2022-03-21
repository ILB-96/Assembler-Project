CC = gcc
CFLAGS = -ansi -pedantic -Wall -g
assembler: command_process.o data_struct_words.o assembler.o pre_assembler.o first_pass.o second_pass.o functions.o label_struct.o macros_struct.o
	$(CC) $(CFLAGS) command_process.o data_struct_words.o  assembler.o pre_assembler.o first_pass.o second_pass.o functions.o label_struct.o macros_struct.o -o assembler
assembler.o: assembler.c assembler.h
	$(CC) $(CFLAGS) -c assembler.c -o assembler.o
functions.o: functions.c assembler.h
	$(CC) $(CFLAGS) -c functions.c -o functions.o
label_struct.o: label_struct.c assembler.h
	$(CC) $(CFLAGS) -c label_struct.c -o label_struct.o
macros_struct.o: macros_struct.c assembler.h
	$(CC) $(CFLAGS) -c macros_struct.c -o macros_struct.o
pre_assembler.o: pre_assembler.c assembler.h
	$(CC) $(CFLAGS) -c pre_assembler.c -o pre_assembler.o
first_pass.o: first_pass.c assembler.h 
	$(CC) $(CFLAGS) -c first_pass.c -o first_pass.o
second_pass.o: second_pass.c assembler.h 
	$(CC) $(CFLAGS) -c second_pass.c -o second_pass.o
command_process.o: command_process.c assembler.h
	$(CC) $(CFLAGS) -c command_process.c -o command_process.o
data_struct_words.o: data_struct_words.c assembler.h
	$(CC) $(CFLAGS) -c data_struct_words.c -o data_struct_words.o

clean:
	rm -f core *.o
fclean:
	rm -f core *.ob *.ext *.ent *.am