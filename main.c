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

        assembler(*++argv);

        remove("macros-file.txt");
    }
    return 0;
}

void assembler(char *file_name)
{
    FILE *expanded_file_ptr;
    char *expanded_file_name;
    TypeLabel *label_array;
    if ((expanded_file_name = (char *)malloc(sizeof(char) * (strlen(file_name) + 13))) == NULL)
    {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    printf("Pre Assembly process at work...");
    fflush(stdout);
    if (!preAssembler(expanded_file_name, file_name))
    {
        free(expanded_file_name);
        return;
    }
    printf(GRN "OK.\n" NRM);
    printf("===Assembly process for %s.as started===\n", file_name);
    if (!(expanded_file_ptr = fopen(expanded_file_name, "r")))
    {
        fprintf(stderr, "Error: File '%s' open failed.\n", expanded_file_name);
        exit(EXIT_FAILURE);
    }
    printf("Creating labels table...");
    fflush(stdout);
    LABEL_INIT(label_array)
    createLabelsTable(expanded_file_ptr, label_array);
    printf(GRN "OK.\n" NRM);
    printf("Checking for errors in code...");
    fflush(stdout);
    /*if (!errorsCheck(expanded_file_ptr))
    {
        free(expanded_file_name);
        fclose(expanded_file_ptr);
        return;
    }*/
    printf(GRN "OK.\n" NRM);
    fclose(expanded_file_ptr);
    free(label_array);
    free(expanded_file_name);
    printf("===Assembly process for %s.as finshed===\n\n", file_name);
}