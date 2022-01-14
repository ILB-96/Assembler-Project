#ifndef ASSEMBLER_DOT_H
#define ASSEMBLER_DOT_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE 80

void preAssembler(char *file);
void findMacros(FILE *fptr);
int isMacroName(char *word, FILE *macros_fptr);
void insertMacro(FILE *expanded_macros_fptr, FILE * macros_fptr, char *word);
char *firstWord(char *line);
char *macroName(char *line);
int nextWordIndex(char *line, int index);
#endif