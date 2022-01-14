all: pre-assembler assembler
pre-assembler: pre-assembler.c assembler.h
	gcc -pedantic -ansi -Wall -g pre-assembler.c -c -lm
assembler: assembler.c assembler.h
	gcc -pedantic -ansi -Wall -g pre-assembler.o assembler.c -o assembler -lm
clean:
	rm -f pre-assembler
	rm -f assembler
	