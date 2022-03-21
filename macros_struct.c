#include "assembler.h"
#define MAX_MACRO_LENGTH 31
#define MAX_MACRO_LINES 6
struct TypeMacro
{
    char name[MAX_MACRO_LENGTH];
    char expand[MAX_MACRO_LINES][MAX_LINE];
};

/*
 *Checks if we found a macro name in a line
 *return the location of the macro in the macros table
 */
int is_macro_name(char *token, TypeMacro *macros_table)
{
    int i = 0;
    while (strcmp(macros_table[i].name, ""))
    {
        if (!strcmp(macros_table[i].name, token))
            return i;
        i++;
    }
    return -1;
}

/*
 *Adds macro content to expanded file in the correct location
 */
void insert_macro(FILE *exp_file_handler, TypeMacro *macros_table, char *token, int i)
{
    int j = 0;
    while (j <= MAX_MACRO_LINES && strcmp(macros_table[i].expand[j], ""))
        fprintf(exp_file_handler, "%s", macros_table[i].expand[j++]);
}

/*
 *Initializes the macro array with 3 array members
 */
void macros_init(int count, TypeMacro **macros_table)
{
    if (count == 0)
        if ((*macros_table = calloc(BASE_ARR_SIZE, sizeof(TypeMacro))) == NULL)
        {
            fprintf(stdout, "Error: Out of memory\n");
            exit(EXIT_FAILURE);
        }

    strcpy((*macros_table)[count].name, "");
    strcpy((*macros_table)[count].expand[0], "");
}

/*
 *adds a macro name and its content to the macros table
 */
void macro_add(char *line, char *token, int count, size_t *array_size, FILE *file_handler, TypeMacro **macros_table)
{
    int j = 0;
    int end_of_macro = 0;
    strcpy((*macros_table)[count].name, token);
    while (!end_of_macro && fgets(line, MAX_LINE, file_handler))
    {
        get_first_token(line, token);
        if (strcmp(token, "endm"))
        {
            if (j >= MAX_MACRO_LINES)
            {
                fprintf(stdout, "Error: According to instruction macro max number of lines is 6\n");
                exit(EXIT_FAILURE);
            }
            else if (!is_empty_line(line) && !is_comment_line(line))
                strcpy((*macros_table)[count].expand[j++], line);
        }
        else
            end_of_macro = 1;
    }
    if (count == *array_size - 1)
    {
        *array_size *= 2;
        if (!(*macros_table = realloc(*macros_table, *array_size * sizeof(TypeMacro))))
        {
            fprintf(stdout, "Error: Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    macros_init(++count, macros_table);
}
