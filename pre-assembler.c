#include "assembler.h"
void expandMacros(FILE *, FILE *);
int isMacroName(char *, FILE *);
void insertMacro(FILE *, FILE *, char *);
void macroName(char *, char *);
int isValidMacroName(char *);

int preAssembler(char *expanded_name, char *file_name)
{
    char as[] = ".as";
    FILE *file_handler;
    FILE *expanded_file_handler;
    strcpy(expanded_name, file_name);
    strcat(expanded_name, as);
    if (!(file_handler = fopen(expanded_name, "r")))
    {
        fprintf(stderr, RED "FAILED!\n" NRM "Error: File '%s' open failed.\n\n", expanded_name);
        return 0;
    }
    strcpy(expanded_name, "expanded-");
    strcat(expanded_name, file_name);
    strcat(expanded_name, as);
    if (!(expanded_file_handler = fopen(expanded_name, "w")))
    {
        fprintf(stderr, RED "FAILED!\n" NRM "Error: File '%s' open failed.\n\n", expanded_name);
        exit(EXIT_FAILURE);
    }

    expandMacros(file_handler, expanded_file_handler);

    fclose(expanded_file_handler);
    fclose(file_handler);
    return 1;
}

void expandMacros(FILE *file_handler, FILE *expanded_file_handler)
{
    FILE *macros_file_handler;
    char line[MAX_LINE] = "", word[MAX_LINE] = "";
    unsigned int is_part_of_macro = 0;
    if (!(macros_file_handler = fopen("macros-file.txt", "w+")))
    {
        fprintf(stderr, RED "FAILED!\n" NRM "Error: File '%s' open failed.\n\n", "macros-file.txt");
        exit(EXIT_FAILURE);
    }
    while (fgets(line, sizeof(line), file_handler) != NULL)
    {
        firstWord(line, word);
        if (!strcmp(word, "macro") || is_part_of_macro)
        {
            if (!strcmp(word, "endm"))
            {
                is_part_of_macro = 0;
                fprintf(macros_file_handler, "%s\n", word);
            }
            else
            {
                if (!strcmp(word, "macro"))
                {
                    is_part_of_macro = 1;
                    macroName(line, word);
                    fprintf(macros_file_handler, "%s\n", word);
                }
                else if (!isSpaceLine(line) && !isCommentLine(line))
                    fprintf(macros_file_handler, "%s", line);
            }
        }
        else
        {
            if (isMacroName(word, macros_file_handler))
            {
                insertMacro(expanded_file_handler, macros_file_handler, word);
            }
            else if (!isSpaceLine(line) && !isCommentLine(line))
                fprintf(expanded_file_handler, "%s", line);
            fseek(macros_file_handler, 0, SEEK_END);
        }
    }
    remove("macros-file.txt");
    fclose(macros_file_handler);
}

int isSpaceLine(char *line)
{
    unsigned int i = 0;
    while (line[i] != '\0')
        if (!isspace(line[i++]))
            return 0;
    return 1;
}
int isCommentLine(char *line)
{
    unsigned int i = 0;
    while (isspace(line[i++]))
        ;
    if (line[--i] == ';')
        return 1;
    return 0;
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
int isMacroName(char *word, FILE *macros_file_handler)
{
    char line[MAX_LINE] = "", *macro_name;
    unsigned int next_is_macro = 1;
    fseek(macros_file_handler, 0, SEEK_SET);
    while (fgets(line, MAX_LINE, macros_file_handler) != NULL)
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

void insertMacro(FILE *expanded_file_handler, FILE *macros_file_handler, char *word)
{
    char line[MAX_LINE] = "", fword[MAX_LINE] = "";
    unsigned int inserted = 0, insert = 0;
    fseek(macros_file_handler, 0, SEEK_SET);
    while (fgets(line, MAX_LINE, macros_file_handler) != NULL && !inserted)
    {
        firstWord(line, fword);
        if (!strcmp(fword, "endm") && insert)
        {
            insert = 0;
            inserted = 1;
        }
        if (insert)
        {
            fprintf(expanded_file_handler, "%s", line);
        }
        if (!strcmp(fword, word) && !insert)
            insert = 1;
    }
}