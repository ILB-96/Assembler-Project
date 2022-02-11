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
TypeLabel *labels_array;
const char *registers_array[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
const char *operations_array[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
const char *guiders_array[] = {".data", ".string", ".entry", ".extern"};

int firstPass(FILE *expanded_file_handler)
{
    unsigned int count = 0, address = 100;
    size_t array_size = 1;
    char line[MAX_LINE] = "", word[MAX_LINE] = "";
    char label_name[MAX_LABEL_LENGTH] = "";
    labelInit(0);
    while (fgets(line, MAX_LINE, expanded_file_handler) != NULL)
    {
        firstWord(line, word);
        /*Case for extern labels*/
        if (!strcmp(word, ".extern"))
        {
            nextWord(line, word, nextWordIndex(line, 0));
            if (!isValidLabelName(word))
            {
                fprintf(stderr, RED "FAILED!\n" NRM "Error: %s is an illigal label name\n", word);
                return 0;
            }
            else if (isLabelExist(word) && !isDefinedExternal(word))
            {
                fprintf(stderr, RED "FAILED!\n" NRM "Error: %s label already exists\n", word);
                return 0;
            }
            labelAdd(count++, word, 0, "external", ++array_size);
            nextWord(line, word, nextWordIndex(line, 0));
        }
        /*Case for inner labels*/
        else if (word[strlen(word) - 1] == ':')
        {
            strcpy(label_name, word);
            removeColon(label_name);
            if (!isValidLabelName(label_name))
            {
                fprintf(stderr, RED "FAILED!\n" NRM "Error: %s is an illigal label name\n", word);
                return 0;
            }
            else if (isLabelExist(label_name))
            {
                fprintf(stderr, RED "FAILED!\n" NRM "Error: %s label already exists\n", word);
                return 0;
            }
            nextWord(line, word, nextWordIndex(line, 0));
            if (!strcmp(word, ".data") || !strcmp(word, ".string"))
            {
                labelAdd(count++, label_name, address, "data", ++array_size);
            }
            else
                labelAdd(count++, label_name, address, "code", ++array_size);
        }
        if (!isSpaceLine(line))
            address += countWords(line, word); /*TODO: this function should move the current address to the address of the command*/
        if (address > MAX_ADDRESS)
        {
            fprintf(stderr, RED "FAILED!\n" NRM "Error: Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    printf(GRN "OK.\n" NRM);
    printLabels();
    return 1;
}
int isDefinedExternal(char *word)
{
    unsigned int i = 0;
    while (strcmp(labels_array[i].name, ""))
    {
        if (!strcmp(labels_array[i].name, word) && !strcmp(labels_array[i].attribute, "external"))
            return 1;
    }
    return 0;
}
void labelInit(unsigned int count)
{
    if (count == 0)
        if ((labels_array = calloc(1, sizeof(TypeLabel))) == NULL)
        {
            fprintf(stderr, RED "FAILED!\n" NRM "Error: Out of memory\n");
            exit(EXIT_FAILURE);
        }

    if ((labels_array[count].name = (char *)malloc(MAX_LABEL_LENGTH)) == NULL)
    {
        fprintf(stderr, RED "FAILED!\n" NRM "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    if ((labels_array[count].attribute = (char *)malloc(MAX_LABEL_LENGTH)) == NULL)
    {
        fprintf(stderr, RED "FAILED!\n" NRM "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    strcpy(labels_array[count].name, "");
    strcpy(labels_array[count].attribute, "");
    labels_array[count].address = 0;
    labels_array[count].base_address = 0;
    labels_array[count].offset = 0;
}
void labelAdd(unsigned int count, char *word, unsigned int address, char *attribute, size_t array_size)
{

    strcpy(labels_array[count].name, word);
    strcpy(labels_array[count].attribute, attribute);
    labels_array[count].address = address;
    labels_array[count].base_address = baseAddress(address);
    labels_array[count].offset = (address % BASE_MODE);

    if ((labels_array = realloc(labels_array, array_size * sizeof(TypeLabel))) == NULL)
    {
        fprintf(stderr, RED "FAILED!\n" NRM "Error: Out of memory\n");
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
    for (i = 0; strcmp(labels_array[i].name, ""); i++)
    {
        if (!strcmp(labels_array[i].name, word))
            return 1;
    }
    return 0;
}
void printLabels()
{
    unsigned int i;
    printf("Name\t|\taddress\t|\tbase\t|\toffset\t|\tattributes\n");
    printf("--------|---------------|---------------|---------------|-------------------\n");
    for (i = 0; strcmp(labels_array[i].name, ""); i++)
    {
        printf("%s\t|\t%d\t|\t%d\t|\t%d\t|\t%s\n", labels_array[i].name, labels_array[i].address, labels_array[i].base_address, labels_array[i].offset, labels_array[i].attribute);
    }
}