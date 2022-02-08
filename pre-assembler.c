#include "assembler.h"

int preAssembler(char *expanded_name, char *file)
{
    char as[] = ".as";
    FILE *fptr;
    FILE *exp_fptr;
    strcpy(expanded_name, file);
    strcat(expanded_name, as);
    if (!(fptr = fopen(expanded_name, "r")))
    {
        fprintf(stderr, "Error: File '%s' open failed.\n", expanded_name);
        return 0;
    }
    strcpy(expanded_name, "expanded-");
    strcat(expanded_name, file);
    strcat(expanded_name, as);
    if (!(exp_fptr = fopen(expanded_name, "w")))
    {
        fclose(fptr);
        fprintf(stderr, "Error: File '%s' open failed.\n", expanded_name);
        exit(EXIT_FAILURE);
    }
    
    expandMacros(fptr, exp_fptr))
    fclose(exp_fptr);
    fclose(fptr);
    return 1;
}

void expandMacros(FILE *fptr, FILE *exp_fptr)
{
    FILE *macros_fptr;
    char line[MAX_LINE] = "", word[MAX_LINE] = "";
    int is_part_of_macro = 0;
    if (!(macros_fptr = fopen("macros-file.txt", "w+")))
    {
        fprintf(stderr, "Error: File '%s' open failed.\n", "macros-file.txt");
        exit(EXIT_FAILURE);
    }
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        firstWord(line, word);
        if (!strcmp(word, "macro") || is_part_of_macro)
        {
            if (!strcmp(word, "endm"))
            {
                is_part_of_macro = 0;
                fprintf(macros_fptr, "%s\n", word);
            }
            else
            {
                if (!strcmp(word, "macro"))
                {
                    is_part_of_macro = 1;
                    macroName(line, word);
                    fprintf(macros_fptr, "%s\n", word);
                }
                else
                    fprintf(macros_fptr, "%s", line);
            }
        }
        else
        {
            if (isMacroName(word, macros_fptr))
            {
                insertMacro(exp_fptr, macros_fptr, word);
            }
            else
                fprintf(exp_fptr, "%s", line);
            fseek(macros_fptr, 0, SEEK_END);
        }
    }
    fclose(macros_fptr);
    return 1;
}

void firstWord(char *line, char *word)
{
    int i = 0, j = 0;
    while (isspace(line[i]) && line[i] != '\0')
    {
        i++;
    }
    while (!isspace(line[i]) && line[i] != '0')
    {
        word[j++] = line[i++];
    }
    word[j] = '\0';
}
void macroName(char *line, char *word)
{
    int i = 0, j = 0;
    while (isspace(line[i]) && line[i] != '\0')
    {
        i++;
    }
    i = nextWordIndex(line, i);
    while (!isspace(line[i]) && line[i] != '0')
    {
        word[j++] = line[i++];
    }

    word[j] = '\0';
}
int nextWordIndex(char *line, int index)
{
    while (!isspace(line[index]) && line[index] != '\0')
    {
        index++;
    }
    while (isspace(line[index]) && line[index] != '\0')
    {
        index++;
    }
    return index;
}
int isMacroName(char *word, FILE *macros_fptr)
{
    char line[MAX_LINE] = "", *macro_name;
    int next_is_macro = 1;
    fseek(macros_fptr, 0, SEEK_SET);
    while (fgets(line, MAX_LINE, macros_fptr) != NULL)
    {
        macro_name = strtok(line, " \n");
        if (next_is_macro)
        {
            if (!strcmp(macro_name, word))
                return 1;
            else
                next_is_macro = 0;
        }
        if (!strcmp(macro_name, "endm"))
            next_is_macro = 1;
    }
    return 0;
}


void insertMacro(FILE *expanded_macros_fptr, FILE *macros_fptr, char *word)
{
    char line[MAX_LINE] = "", fword[MAX_LINE] = "";
    int inserted = 0, insert = 0;
    fseek(macros_fptr, 0, SEEK_SET);
    while (fgets(line, MAX_LINE, macros_fptr) != NULL && !inserted)
    {
        firstWord(line, fword);
        if (!strcmp(fword, "endm") && insert)
        {
            insert = 0;
            inserted = 1;
        }
        if (insert)
        {
            fprintf(expanded_macros_fptr, "%s", line);
        }
        if (!strcmp(fword, word) && !insert)
            insert = 1;
    }
}