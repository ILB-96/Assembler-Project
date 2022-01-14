#include "assembler.h"

void preAssembler(char *file)
{
    char as[] = ".as";
    FILE *fptr;
    strcat(file, as);
    if (!(fptr = fopen(file, "r")))
    {
        fprintf(stderr, "error: file '%s' open failed.\n", file);
        exit(1);
    }
    findMacros(fptr);
    fclose(fptr);
}

void findMacros(FILE *fptr)
{
    FILE *expanded_macros_fptr;
    FILE *macros_fptr;
    int macros_counter = 0;
    char line[MAX_LINE] = "", *word;
    int is_part_of_macro = 0;
    if (!(macros_fptr = fopen("macros-file.txt", "w+")))
    {
        fprintf(stderr, "error: file 'macros-file.txt' open failed.\n");
        exit(1);
    }
    if (!(expanded_macros_fptr = fopen("expanded-macros.as", "w")))
    {
        fprintf(stderr, "error: file 'expanded-macros.as' open failed.\n");
        exit(1);
    }
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        word = firstWord(line);
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
                    macros_counter++;
                    word = macroName(line);
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
                insertMacro(expanded_macros_fptr, macros_fptr, word);
                printf("good\n");
            }
            else
                fprintf(expanded_macros_fptr, "%s", line);
            fseek(macros_fptr, 0, SEEK_END);
        }
    }
    fclose(macros_fptr);
    fclose(expanded_macros_fptr);
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
void insertMacro(FILE *expanded_macros_fptr, FILE * macros_fptr, char *word){
    char line[MAX_LINE] = "";
    int inserted = 0, insert = 0;
    fseek(macros_fptr, 0, SEEK_SET);
    while(fgets(line, MAX_LINE, macros_fptr) != NULL && !inserted){

        if(!strcmp(firstWord(line), "endm") && insert){
            insert = 0;
            inserted = 1;
        }
        if(insert){
            fprintf(expanded_macros_fptr, "%s", line);
        }
        if(!strcmp(firstWord(line), word) && !insert)
            insert = 1;
    }
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
    while (isspace(line[i]) && line[i] != '\0')
    {
        i++;
    }
    while (!isspace(line[i]) && line[i] != '0')
    {
        word[j++] = line[i++];
    }
    word[j] = '\0';
    return &word[0];
}

char *macroName(char *line)
{
    int i = 0;
    int j = 0;
    char *word;
    if ((word = (char *)malloc(strlen(line) * sizeof(char))) == NULL)
    {
        printf("ERROR: Out of Memory\n");
        exit(1);
    }
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
    return &word[0];
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