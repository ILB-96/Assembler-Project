#include "assembler.h"


void process(char *file)
{
    char as[] = ".as";
    FILE *fptr;
    char line[MAX_LINE] = "";
    strcat(file, as);
    fptr = fopen(file, "r+");
    if (!fptr)
    {
        fprintf(stderr, "error: file open failed '%s'.\n", file);
        exit(1);
    }
    findMacros(fptr);
    fclose(fptr);
    fptr = fopen("macros-file.txt", "r");
    while (fgets(line, MAX_LINE, fptr)){
        printf("%s", line);
    }
    fclose(fptr);
}

void findMacros(FILE *fptr)
{
    FILE *distributed_macros_fptr;
    FILE *macros;
    int macros_counter = 0;
    char line[MAX_LINE] = "", *word;
    int is_part_of_macro = 0;
    macros = fopen("macros-file.txt", "w");
    distributed_macros_fptr = fopen("distributed-macros.as", "w");
    while (fgets(line, sizeof(line), fptr))
    {
        word = firstWord(line);
        if (!strcmp(word, "macro") || is_part_of_macro)
        {
            if (!strcmp(word, "endm"))
            {
                is_part_of_macro = 0;
                fprintf(macros, "%s\n", word);
            }
            else
            {
                if(!strcmp(word, "macro")){
                    is_part_of_macro = 1;
                    macros_counter++;
                    word = macroName(line);
                    fprintf(macros, "%s\n", word);
                }
                else
                    fprintf(macros, "%s", line);
            }
        }
        else{
                fprintf(distributed_macros_fptr, "%s", line);
        }
    }
    fclose(macros);
    fclose(distributed_macros_fptr);
}

char *firstWord(char *line)
{
    int i = 0;
    int j = 0;
    char *word;
    if ((word = (char *)malloc(strlen(line) * sizeof(char))) == NULL)
    {
        printf("ERROR: Out of Memory\n");
        exit(1);
    }
    while (isspace(line[i]) && line[i] != '\0'){
        i++;
    }
    while (!isspace(line[i]) && line[i] != '0'){
        word[j++] = line[i++];
    }
    word[j] = '\0';
    return &word[0];
}

char *macroName(char *line){
    int i = 0;
    int j = 0;
    char *word;
    if ((word = (char *)malloc(strlen(line) * sizeof(char))) == NULL)
    {
        printf("ERROR: Out of Memory\n");
        exit(1);
    }
    while (isspace(line[i]) && line[i] != '\0'){
        i++;
    }
    i = nextWordIndex(line, i);
    while (!isspace(line[i]) && line[i] != '0'){
        word[j++] = line[i++];
    }

    word[j] = '\0';
    return &word[0];
}
int nextWordIndex(char *line, int index){
    while(!isspace(line[index]) && line[index] != '\0'){
        index++;
    }
    while (isspace(line[index]) && line[index] != '\0'){
        index++;
    }
    return index;
}