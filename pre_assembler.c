#include "assembler.h"
#define AS ".as"

void expand_macros(FILE *, FILE *);
int is_macro_name(char *, FILE *);
void insert_macro(FILE *, FILE *, char *);
void get_macro_name(char *, char *);

int pre_assembler(char *expanded_name, char *file_name)
{
    FILE *file_handler;
    FILE *expanded_file_handler;
    strcpy(expanded_name, file_name);
    strcat(expanded_name, AS);
    if (!(file_handler = fopen(expanded_name, "r")))
    {
        fprintf(stderr, "FAILED!\nError: File '%s' open failed.\n\n", expanded_name);
        return 0;
    }
    strcpy(expanded_name, "expanded_");
    strcat(expanded_name, file_name);
    strcat(expanded_name, AS);
    if (!(expanded_file_handler = fopen(expanded_name, "w")))
    {
        fprintf(stderr, "FAILED!\nError: File '%s' open failed.\n\n", expanded_name);
        exit(EXIT_FAILURE);
    }

    /*TODO: maybe change macros file to a dynamic array*/
    expand_macros(file_handler, expanded_file_handler);
    fclose(expanded_file_handler);
    fclose(file_handler);
    remove("macros-file.txt");
    printf("OK.\n");
    return 1;
}

void expand_macros(FILE *file_handler, FILE *expanded_file_handler)
{
    FILE *macros_file_handler;
    char line[MAX_LINE] = "", word[MAX_LINE] = "";
    unsigned int is_part_of_macro = 0;
    if (!(macros_file_handler = fopen("macros-file.txt", "w+")))
    {
        fprintf(stderr, "FAILED!\nError: File '%s' open failed.\n\n", "macros-file.txt");
        exit(EXIT_FAILURE);
    }
    while (fgets(line, sizeof(line), file_handler) != NULL)
    {
        get_first_token(line, word);
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
                    get_macro_name(line, word);
                    fprintf(macros_file_handler, "%s\n", word);
                }
                else if (!is_empty_line(line) && !is_comment_line(line))
                    fprintf(macros_file_handler, "%s", line);
            }
        }
        else
        {
            if (is_macro_name(word, macros_file_handler))
            {
                insert_macro(expanded_file_handler, macros_file_handler, word);
            }
            else if (!is_empty_line(line) && !is_comment_line(line))
                fprintf(expanded_file_handler, "%s", line);
            fseek(macros_file_handler, 0, SEEK_END);
        }
    }

    fclose(macros_file_handler);
}

void get_macro_name(char *line, char *word)
{
    unsigned int i = 0, j = 0;
    while (isspace(line[i]) && line[i] != '\0')
    {
        i++;
    }
    i = get_next_token_index(line, i);
    while (!isspace(line[i]) && line[i] != '0')
    {
        word[j++] = line[i++];
    }

    word[j] = '\0';
}

int is_macro_name(char *word, FILE *macros_file_handler)
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

void insert_macro(FILE *expanded_file_handler, FILE *macros_file_handler, char *word)
{
    char line[MAX_LINE] = "", fword[MAX_LINE] = "";
    unsigned int inserted = 0, insert = 0;
    fseek(macros_file_handler, 0, SEEK_SET);
    while (fgets(line, MAX_LINE, macros_file_handler) != NULL && !inserted)
    {
        get_first_token(line, fword);
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