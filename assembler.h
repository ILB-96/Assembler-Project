#ifndef ASSEMBLER_DOT_H
#define ASSEMBLER_DOT_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE 81

void assembler(char *file_name);
int preAssembler(char *expanded_name, char *file);
void expandMacros(FILE *fptr, FILE *exp_fptr);
int isMacroName(char *word, FILE *macros_fptr);
void insertMacro(FILE *expanded_macros_fptr, FILE *macros_fptr, char *word);
void firstWord(char *line, char *word);
void macroName(char *line, char *word);
int nextWordIndex(char *line, int index);
int isValidMacroName(char *word);
#endif
