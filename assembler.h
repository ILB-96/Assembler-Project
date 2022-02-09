#ifndef ASSEMBLER_DOT_H
#define ASSEMBLER_DOT_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE 81
#define NRM "\x1B[0m"
#define GRN "\x1B[32m"
#define RED "\x1B[31m"
#define LABEL_INIT(label_array)                                        \
    if ((label_array = calloc(1, sizeof(TypeLabel))) == NULL)          \
    {                                                                  \
        fprintf(stderr, RED "FAILED!\n" NRM "Error: Out of memory\n"); \
        exit(EXIT_FAILURE);                                            \
    }
typedef struct TypeLabel
{
    char *name;
    int address;
} TypeLabel;

void assembler(char *);
int preAssembler(char *, char *);
void expandMacros(FILE *, FILE *);
int isMacroName(char *, FILE *);
void insertMacro(FILE *, FILE *, char *);
void firstWord(char *, char *);
void macroName(char *, char *);
int nextWordIndex(char *, int);
int isValidMacroName(char *);
int errorsCheck(FILE *);
void createLabelsTable(FILE *, TypeLabel *);
int isValidLabelName(char *);
#endif