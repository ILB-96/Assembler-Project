#ifndef ASSEMBLER_DOT_H
#define ASSEMBLER_DOT_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE 81
#define MAX_ADDRESS 8191
#define NRM "\x1B[0m"
#define GRN "\x1B[32m"
#define RED "\x1B[31m"

/*Defines and store Labels Table*/
typedef struct TypeLabel
{
    char *name;
    int address;
    int base_address;
    int offset;
    char *attribute;
} TypeLabel;
extern TypeLabel *labels_array;

/*Functions from pre-assembler.c*/
int preAssembler(char *, char *);
void firstWord(char *, char *);
int nextWordIndex(char *, int);
int isCommentLine(char *);
int isSpaceLine(char *);
/*Functions from first-pass.c*/
int firstPass(FILE *);
int countWords(char *);
/*Functions from second-pass.c*/

#endif