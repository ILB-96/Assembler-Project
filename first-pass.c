#include "assembler.h"
#define MAX_LABEL_LENGTH 32

/*Inner functions that are ment to use only inside this source*/
void removeColon(char *);
int isValidLabelName(char *);
int isLabelExist(char *);
void nextWord(char *, char *, unsigned int);
int baseAddress(int);
void labelInit(unsigned int count);
void labelAdd(unsigned int, char *, unsigned int, char *, size_t);
void printLabels();
int isDefinedExternal(char *);
/*Global Veriabls*/
TypeLabel *symbol_table;
unsigned int instruction_counter = 100;
unsigned int data_counter = 0;
const char *registers_array[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
const char *operations_array[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
const char *guiders_array[] = {".data", ".string", ".entry", ".extern"};

int firstPass(FILE *expanded_file_handler)
{
    unsigned int label_counter = 0, error = 1;
    size_t array_size = 1;
    char line[MAX_LINE] = "", word[MAX_LINE] = "";
    char label_name[MAX_LINE] = "";

    labelInit(0);

    /*TODO: use error flag to continue looking for errors in the program and stop the program after that*/
    while (fgets(line, MAX_LINE, expanded_file_handler) != NULL)
    {
        firstWord(line, word);
        /*Case for extern labels*/
        if (!strcmp(word, ".extern"))
        {
            nextWord(line, word, nextWordIndex(line, 0));
            if (!isValidLabelName(word))
            {
                error = 0;
                fprintf(stderr, "FAILED!\nError: '%s' is an illigal label name\n", word);
                return 0; /*TODO: add the line of the error in the file?*/
            }
            else if (isLabelExist(word) && !isDefinedExternal(word))
            {
                error = 0;
                fprintf(stderr, "FAILED!\nError: '%s' label already exists\n", word);
                return 0;
            }
            labelAdd(label_counter++, word, 0, "external", ++array_size);
            nextWord(line, word, nextWordIndex(line, 0));
        }
        /*Case for inner labels*/
        else if (word[strlen(word) - 1] == ':')
        {
            strcpy(label_name, word);
            removeColon(label_name);
            if (!isValidLabelName(label_name))
            {
                fprintf(stderr, "FAILED!\nError: '%s' at '%s' an illigal label name\n", label_name, line);
                error = 0;
                return 0;
            }
            else if (isLabelExist(label_name))
            {
                fprintf(stderr, "FAILED!\nError: '%s' label from  '%s' already exists\n", label_name, line);
                error = 0;
                return 0;
            }
            nextWord(line, word, nextWordIndex(line, 0));
            if (!strcmp(word, ".data") || !strcmp(word, ".string"))
            {
                labelAdd(label_counter++, label_name, instruction_counter, "data", ++array_size);
            }
            else
                labelAdd(label_counter++, label_name, instruction_counter, "code", ++array_size);
        }
        if (!isSpaceLine(line))                            /*check to empty*/
            instruction_counter += countWords(line, word); /*TODO: this function should move the current address to the address of the command*/
        if (instruction_counter > MAX_ADDRESS)
        {
            fprintf(stderr, "FAILED!\nError: Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    if (error == 1)
        printf("OK.\n");
    printLabels();
    return error;
}
int isDefinedExternal(char *word)
{
    unsigned int i = 0;
    while (strcmp(symbol_table[i].name, ""))
    {
        if (!strcmp(symbol_table[i].name, word) && !strcmp(symbol_table[i].attribute, "external"))
            return 1;
    }
    return 0;
}
void labelInit(unsigned int count)
{
    if (count == 0)
        if ((symbol_table = calloc(1, sizeof(TypeLabel))) == NULL)
        {
            fprintf(stderr, "FAILED!\nError: Out of memory\n");
            exit(EXIT_FAILURE);
        }

    if ((symbol_table[count].name = (char *)malloc(MAX_LABEL_LENGTH)) == NULL)
    {
        fprintf(stderr, "FAILED!\nError: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    if ((symbol_table[count].attribute = (char *)malloc(MAX_LABEL_LENGTH)) == NULL)
    {
        fprintf(stderr, "FAILED!\nError: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    strcpy(symbol_table[count].name, "");
    strcpy(symbol_table[count].attribute, "");
    symbol_table[count].address = 0;
    symbol_table[count].base_address = 0;
    symbol_table[count].offset = 0;
}
void labelAdd(unsigned int count, char *word, unsigned int address, char *attribute, size_t array_size)
{

    strcpy(symbol_table[count].name, word);
    strcpy(symbol_table[count].attribute, attribute);
    symbol_table[count].address = address;
    symbol_table[count].base_address = baseAddress(address);
    symbol_table[count].offset = (address % BASE_MODE);

    if ((symbol_table = realloc(symbol_table, array_size * sizeof(TypeLabel))) == NULL)
    {
        fprintf(stderr, "FAILED!\nError: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    labelInit(++count);
}
void nextWord(char *line, char *word, unsigned int i)
{
    unsigned int j = 0;
    while (line[i] != '\0')
    {
        line[j++] = line[i++];
    }

    line[j] = '\0';
    j = 0;
    i = 0;
    while (!isspace(line[i]))
    {
        word[j++] = line[i++];
    }
    word[j] = '\0';
}
int baseAddress(int address)
{

    if (address == 0)
        return 0;
    return (address - (address % BASE_MODE));
}
int countWords(char *line, char *word)
{
    return 1;
}

void removeColon(char *word)
{
    unsigned int i = 0;
    while (word[i++] != ':')
        ;
    word[--i] = '\0';
}
int isValidLabelName(char *word)
{
    unsigned int i;
    unsigned int arrays_length = 16;

    if (!isalpha(word[0]))
        return 0;
    if (strlen(word) > MAX_LABEL_LENGTH)
        return 0;
    for (i = 0; i < arrays_length; i++)
    {
        if (!strcmp(word, registers_array[i]) || !strcmp(word, operations_array[i]))
            return 0;
    }
    if (!strcmp(word, "A") || !strcmp(word, "R") || !strcmp(word, "E"))
        return 0;
    return 1;
}

int isLabelExist(char *word)
{
    unsigned int i;
    for (i = 0; strcmp(symbol_table[i].name, ""); i++)
    {
        if (!strcmp(symbol_table[i].name, word))
            return 1;
    }
    return 0;
}
void printLabels()
{
    unsigned int i;
    printf("Name\t|\taddress\t|\tbase\t|\toffset\t|\tattributes\n");
    printf("--------|---------------|---------------|---------------|-------------------\n");
    for (i = 0; strcmp(symbol_table[i].name, ""); i++)
    {
        printf("%s\t|\t%d\t|\t%d\t|\t%d\t|\t%s\n", symbol_table[i].name, symbol_table[i].address, symbol_table[i].base_address, symbol_table[i].offset, symbol_table[i].attribute);
    }
}