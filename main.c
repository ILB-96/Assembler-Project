#include "assembler.h"


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Error: Not enough arguments, please insert files names.\n");
        return 1;
    }
    while (--argc > 0)
    {
        printf("***Assembly process for %s.as started***\n", *++argv);
        assembler(*argv);
        printf("***Assembly process for %s.as finshed***\n\n", *argv);
        remove("macros-file.txt");
    }
    return 0;
}

void assembler(char *file_name)
{
    FILE *exp_fptr;
    char *expanded_file_name;
    if ((expanded_file_name = (char *)malloc(sizeof(char) * (strlen(file_name) + 13))) == NULL)
    {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    printf("   Pre Assembly process at work...");
    fflush(stdout);
    if (!preAssembler(expanded_file_name, file_name))
    {
        free(expanded_file_name);
        return;
    }
    printf(GRN "OK.\n" NRM);
    if (!(exp_fptr = fopen(expanded_file_name, "r")))
    {
        free(expanded_file_name);
        fprintf(stderr, "   Error: File '%s' open failed.\n", expanded_file_name);
        exit(EXIT_FAILURE);
    }
    /*TODO: add function to check if expanded file doesn't contain errors.*/
    
    fclose(exp_fptr);
    free(expanded_file_name);
}