#ifndef ASSEMBLER_DOT_H
#define ASSEMBLER_DOT_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE 81
#define MAX_ADDRESS 8191
#define BASE_MODE 16

/*Defines and store Labels Table*/
typedef struct TypeLabel
{
    char *name;
    int address;
    int base_address;
    int offset;
    char *attribute;
} TypeLabel;

extern unsigned int instruction_counter;
extern unsigned int data_counter;
extern TypeLabel *symbol_table;
/*Functions from pre-assembler.c*/
int preAssembler(char *, char *);
void firstWord(char *, char *);
unsigned int nextWordIndex(char *, int);
int isCommentLine(char *);
int isSpaceLine(char *);
/*Functions from first-pass.c*/
int firstPass(FILE *);
int countWords(char *, char *);
/*Functions from second-pass.c*/
int secondPass(FILE *expanded_file_handler);

#endif