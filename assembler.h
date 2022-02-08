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

typedef struct label
{
    char name[32];
    struct label *next;
    struct label *prev;
} label;

typedef struct list
{
    label *head;
} list;

void assembler(char *file_name);
int preAssembler(char *expanded_name, char *file);
void expandMacros(FILE *fptr, FILE *exp_fptr);
int isMacroName(char *word, FILE *macros_fptr);
void insertMacro(FILE *expanded_macros_fptr, FILE *macros_fptr, char *word);
void firstWord(char *line, char *word);
void macroName(char *line, char *word);
int nextWordIndex(char *line, int index);
int isValidMacroName(char *word);
int errorsCheck(FILE *fptr);
void init(char *label_name, list *l);
void add(list *l, char *label_name);
void freeLabels(list *l);
#endif
