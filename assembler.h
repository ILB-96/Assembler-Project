#ifndef ASSEMBLER_DOT_H
#define ASSEMBLER_DOT_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE 80

void process(char *file);
void findMacros(FILE *fptr);
char *firstWord(char *line);
char *macroName(char *line);
int nextWordIndex(char *line, int index);
#endif