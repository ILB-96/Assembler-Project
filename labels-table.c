#include "assembler.h"

#define LABEL_ADD(label_array, i, label_name, address_val, size)                \
    label_array[i].name = label_name;                                           \
    label_array[i].address = address_val;                                       \
    if ((label_array = realloc(label_array, size * sizeof(TypeLabel))) == NULL) \
    {                                                                           \
        fprintf(stderr, RED "FAILED!\n" NRM "Error: Out of memory\n");          \
        exit(EXIT_FAILURE);                                                     \
    }

void createLabelsTable(FILE *expanded_file_ptr, TypeLabel *label_array)
{
    int count = 0, address = 100;
    size_t array_size = 1;
    char line[MAX_LINE] = "", word[MAX_LINE] = "";

    while (fgets(line, sizeof(line), expanded_file_ptr) != NULL)
    {
        firstWord(line, word);
        if (strstr(word, ":") && isValidLabelName(word))
        {
            LABEL_ADD(label_array, count++, word, address, ++array_size)
        }
    }
}

int isValidLabelName(char *word)
{
    return 0;
}