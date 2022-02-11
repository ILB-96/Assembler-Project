#include "assembler.h"
void expandMacros(FILE *, FILE *);
int isMacroName(char *, FILE *);
void insertMacro(FILE *, FILE *, char *);
void macroName(char *, char *);
int isValidMacroName(char *);

int preAssembler(char *expanded_name, char *file_name)
{
    char as[] = ".as";
    FILE *file_ptr;
    FILE *expanded_file_ptr;
    strcpy(expanded_name, file_name);
    strcat(expanded_name, as);
    if (!(file_ptr = fopen(expanded_name, "r")))
    {
        fprintf(stderr, RED "FAILED!\n" NRM "Error: File '%s' open failed.\n\n", expanded_name);
        return 0;
    }
    strcpy(expanded_name, "expanded-");
    strcat(expanded_name, file_name);
    strcat(expanded_name, as);
    if (!(expanded_file_ptr = fopen(expanded_name, "w")))
    {
        fprintf(stderr, RED "FAILED!\n" NRM "Error: File '%s' open failed.\n\n", expanded_name);
        exit(EXIT_FAILURE);
    }

    expandMacros(file_ptr, expanded_file_ptr);

    fclose(expanded_file_ptr);
    fclose(file_ptr);
    return 1;
}

void expandMacros(FILE *file_ptr, FILE *expanded_file_ptr)
{
    FILE *macros_file_ptr;
    char line[MAX_LINE] = "", word[MAX_LINE] = "";
    unsigned int is_part_of_macro = 0;
    if (!(macros_file_ptr = fopen("macros-file.txt", "w+")))
    {
        fprintf(stderr, RED "FAILED!\n" NRM "Error: File '%s' open failed.\n\n", "macros-file.txt");
        exit(EXIT_FAILURE);
    }
    while (fgets(line, sizeof(line), file_ptr) != NULL)
    {
        firstWord(line, word);
        if (!strcmp(word, "macro") || is_part_of_macro)
        {
            if (!strcmp(word, "endm"))
            {
                is_part_of_macro = 0;
                fprintf(macros_file_ptr, "%s\n", word);
            }
            else
            {
                if (!strcmp(word, "macro"))
                {
                    is_part_of_macro = 1;
                    macroName(line, word);
                    fprintf(macros_file_ptr, "%s\n", word);
                }
                else
                    fprintf(macros_file_ptr, "%s", line);
            }
        }
        else
        {
            if (isMacroName(word, macros_file_ptr))
            {
                insertMacro(expanded_file_ptr, macros_file_ptr, word);
            }
            else
                fprintf(expanded_file_ptr, "%s", line);
            fseek(macros_file_ptr, 0, SEEK_END);
        }
    }
    remove("macros-file.txt");
    fclose(macros_file_ptr);
}

void firstWord(char *line, char *word)
{
    unsigned int i = 0, j = 0;
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
    unsigned int i = 0, j = 0;
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
int isMacroName(char *word, FILE *macros_file_ptr)
{
    char line[MAX_LINE] = "", *macro_name;
    unsigned int next_is_macro = 1;
    fseek(macros_file_ptr, 0, SEEK_SET);
    while (fgets(line, MAX_LINE, macros_file_ptr) != NULL)
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

void insertMacro(FILE *expanded_file_ptr, FILE *macros_file_ptr, char *word)
{
    char line[MAX_LINE] = "", fword[MAX_LINE] = "";
    unsigned int inserted = 0, insert = 0;
    fseek(macros_file_ptr, 0, SEEK_SET);
    while (fgets(line, MAX_LINE, macros_file_ptr) != NULL && !inserted)
    {
        firstWord(line, fword);
        if (!strcmp(fword, "endm") && insert)
        {
            insert = 0;
            inserted = 1;
        }
        if (insert)
        {
            fprintf(expanded_file_ptr, "%s", line);
        }
        if (!strcmp(fword, word) && !insert)
            insert = 1;
    }
}