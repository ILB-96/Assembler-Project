#include "assembler.h"
#define EXPENDED_EXTENTION_LENGTH 13 /*"expanded-.as\0" = 13 chars"*/

/*Private functions meant to be used only inside this source*/
void assembler(char *);
void freeStr(TypeLabel *);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Error: Not enough arguments, please insert files names.\n");
        return 1;
    }
    while (--argc > 0)
    {
        assembler(*++argv); /*assembler process for each file starts here*/
    }
    return 0;
}

/*Primary Function to process all the steps of the assembler*/
void assembler(char *file_name)
{
    /*veriables to handle the expanded file that we will use*/
    FILE *expanded_file_handler;
    char *expanded_file_name;

    /*assigned space for the new name*/
    if ((expanded_file_name = (char *)malloc(sizeof(char) * (strlen(file_name) + EXPENDED_EXTENTION_LENGTH))) == NULL)
    {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }

    printf("Pre Assembly process at work...");
    fflush(stdout);
    if (!preAssembler(expanded_file_name, file_name)) /*go to pre-assembler.c for more info*/
    {
        free(expanded_file_name);
        return;
    }

    printf("===Assembly process for %s.as started===\n", file_name);
    /*Here we open the expanded file completed from the pre assembler*/
    if (!(expanded_file_handler = fopen(expanded_file_name, "r")))
    {
        fprintf(stderr, "Error: File '%s' open failed.\n", expanded_file_name);
        exit(EXIT_FAILURE);
    }

    printf("Assembly First Pass at work...\n");
    if (!firstPass(expanded_file_handler)) /*Go to first-pass.c for more info*/
    {
        fclose(expanded_file_handler);
        free(expanded_file_name);
        freeStr(symbol_table);
        free(symbol_table);
        return;
    }
    printf("Assembly Second Pass at work...\n");
    if (!secondPass(expanded_file_handler)) /*Go to second-pass.c for more info*/
    {
        fclose(expanded_file_handler);
        free(expanded_file_name);
        freeStr(symbol_table);
        free(symbol_table);
        return;
    }

    /*Frees every allocated memory and close the file handler*/
    freeStr(symbol_table);
    free(symbol_table);
    fclose(expanded_file_handler);
    free(expanded_file_name);
    printf("===Assembly process for %s.as finshed===\n\n", file_name);
}

/*This function frees all the strings inside the symbol array*/
void freeStr(TypeLabel *symbol_table)
{
    unsigned int i = 0;
    while (strcmp(symbol_table[i].name, ""))
    {
        free(symbol_table[i].name);
        free(symbol_table[i++].attribute);
    }
    free(symbol_table[i].name);
    free(symbol_table[i].attribute);
}
