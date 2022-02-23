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
/*Symbols Tables is a global veriable to use in the Second Pass*/
TypeLabel *symbol_table;
/*Instruction and data counter are global veriable to use in the Second Pass*/
unsigned int instruction_counter = 100;
unsigned int data_counter = 0;
/*This arrays are saved words, better to have them globaly so we can always check if a variable is trying to use those name*/
/*TODO: maybe we need to add more checks to see variables are not names after saved words?*/
const char *registers_array[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
const char *operations_array[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
const char *guiders_array[] = {".data", ".string", ".entry", ".extern"};

int firstPass(FILE *expanded_file_handler)
{
    /*Variables*/
    unsigned int label_counter = 0, ok = 1; /*error flag*/
    size_t array_size = 1;
    char line[MAX_LINE] = "", word[MAX_LINE] = "";
    char label_name[MAX_LINE] = "";
    /*First initialize the symbols table dynamic array*/
    labelInit(0);

    plw head_IC; /*need to be free in the end...*/
    plw prv_IC;
    plw head_DC;
    plw prv_DC;
    initialize_list(&head_DC, &prv_DC,0);
    initialize_list( &head_IC, &prv_IC,BASE_STOCK);

    /*TODO: use error flag to continue looking for errors in the program and stop the program after that*/
    /*This loop will go through the entire expanded file, creating the labels and adding every piece of data to it's correct location in the memory*/
    while (fgets(line, MAX_LINE, expanded_file_handler) != NULL)
    {
        int iscode = 1;
        
        firstWord(line, word); /*TODO:change to strtok*/
        
        if (!strcmp(word, ".extern")) /*Case for extern labels*/
        {
            iscode = 0;
            nextWord(line, word, nextWordIndex(line, 0)); /*Moves the line and the word to the next word inside the line(if there is any)*/
            if (!isValidLabelName(word))                  /*Checks if label name is valid according to the instructions*/
            {
                /*raises error flag but continue to search for more errors in the first pass*/
                ok = 0;
                fprintf(stderr, "FAILED!\nError: '%s' is an illigal label name\n", word);
                /*TODO: maybe add the line number where the error happened in the file?*/
            }
            else if (isLabelExist(word) && !isDefinedExternal(word)) /*the file is allowed to define the same external more than once without causing an error*/
            {
                ok = 0;
                fprintf(stderr, "FAILED!\nError: '%s' label already exists\n", word);
            }
            /*If we reached here the label is a valid external label and we can add it to the symbols array*/
            labelAdd(label_counter++, word, 0, "external", ++array_size);
            nextWord(line, word, nextWordIndex(line, 0));
            /*TODO: after this nextWord call external should be an empty line or it's an error*/
        }
        /*Case for inner labels*/
        else if (word[strlen(word) - 1] == ':')
        {
            strcpy(label_name, word);
            removeColon(label_name);
            if (!isValidLabelName(label_name))
            {
                fprintf(stderr, "FAILED!\nError: '%s' at '%s' an illigal label name\n", label_name, line);
                ok = 0;
            }
            else if (isLabelExist(label_name))
            {
                fprintf(stderr, "FAILED!\nError: '%s' label from  '%s' already exists\n", label_name, line);
                ok = 0;
            }
            nextWord(line, word, nextWordIndex(line, 0));
            if (!strcmp(word, ".data") || !strcmp(word, ".string"))
            {
                iscode = 0;
                labelAdd(label_counter++, label_name, instruction_counter, "data", ++array_size);
            }
            else
            {
                
                labelAdd(label_counter++, label_name, instruction_counter, "code", ++array_size);
            }
        }

        /*in this point the line contain only code*/
        if(iscode == 1 && (strcmp(word, ".data") != 0 && strcmp(word, ".string") != 0) && strcmp(word, ".entry") != 0 && strcmp(word, ".extern"))
        {
            ok = command_code_pross(&prv_IC,line);
        }
        else if(strcmp(word, ".data") == 0)
        {
            command_data_pross(&prv_DC,line);
        }
        else if(strcmp(word, ".string") == 0)
        {
            
            command_string_pross(&prv_DC,line);
        }

        if (!isEmptyLine(line))
            /*if line is not empty we need to count the words and add them to the words list*/
            instruction_counter += countWords(line, word); /*TODO: this function should move the current address to the address of the command*/
        if (instruction_counter > MAX_ADDRESS)             /*checks if we didn't used too much memory*/
        {
            fprintf(stderr, "FAILED!\nError: Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    
    if (ok)
        printf("OK.\n");

    puts("");    
   /* printLabels();*/
   
    update_address(head_DC,prv_IC->stock_index);
    print_listNode(head_DC);
    
    
    
    free_list(head_IC);
    printf("\n");
    return ok;
}

/**********************************************************************************************
 * The next functions are related to creating, defining and checking validity of symbols
 **********************************************************************************************/
/*Function that accepts a name and checks if a symbol name is already defined and has "external" attribute with the same given name*/
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
/*Function that accept the location in the symbols array to initialize a new symbol*/
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
/*Function to add new symbol and initialize a new symbol in the next address*/
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
/*Function that checks if a given name of label is a valid name*/
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
/*Function that checks if a given name of label is already exist in the symbols array*/
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
/*Function that prints labels(just for show).*/
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
/********************************************************************************************
 * The next functions are used to change and alternate the words and the lines from the file to meet our needs.
 * *****************************************************************************************/
/*Function that moves the line to the next word and insert the next word inside a variable(word)*/
/*TODO: change the variable name "word" to not be confused with a binary word*/
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
/*Function that removes the ":" sign from a word*/
void removeColon(char *word)
{
    unsigned int i = 0;
    while (word[i++] != ':')
        ;
    word[--i] = '\0';
}
/********************************************************************************************
 * The next functions are used to count and calculate binary words and their address
 * *****************************************************************************************/
/*Function to calculate the base address from a given adress*/
int baseAddress(int address)
{
    if (address == 0)
        return 0;
    return (address - (address % BASE_MODE));
}
/*Function to count the number of binary words inside a line*/
int countWords(char *line, char *word)
{
    return 1;
}
