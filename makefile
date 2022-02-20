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
