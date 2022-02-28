#include "assembler.h"
#define MAX_LABEL_LENGTH 32

/*Inner functions that are meant to use only inside this source*/
int is_valid_label_name(char *);
int is_label_exists(char *);
void get_next_token(char *, char *, unsigned int);
int base_address(int);
void label_init(unsigned int count);
void label_add(unsigned int, char *, unsigned int, char *, size_t);
void update_data_labels_address(int);
void print_labels();
int is_defined_external(char *);

/*Defining and declaring the symbols table*/
struct TypeLabel
{
    char *name;
    int address;
    int base_address;
    int offset;
    char *attribute;
};
TypeLabel *symbols_table;
unsigned int g_error;
/*Instruction and data counter are global variable to use in the Second Pass*/

/*TODO: maybe we need to add more checks to see variables are
not names after saved words?*/

/*Those arrays are saved words, better to have them globally,
so we can always check if a variable is trying to use those name*/

int first_pass(FILE *expanded_file_handler)
{
    /*Variables*/
    unsigned int label_counter = 0;
    unsigned int line_number = 1;
    int is_code = TRUE;
    size_t array_size = 1;
    char line[MAX_LINE];
    char word[MAX_LINE];
    char label_name[MAX_LINE];

    plw head_IC; /*need to be free in the end...*/
    plw prv_IC;
    plw head_DC;
    plw prv_DC;
    initialize_list(&head_DC, &prv_DC, 0);
    initialize_list(&head_IC, &prv_IC, BASE_STOCK);
    /*First initialize the symbols' table dynamic array and error flag*/

    g_error = FALSE;

    label_init(0);
    /*TODO: use error flag to continue looking for errors in the program and
     * stop the program after that*/
    /*This loop will go through the entire expanded file, creating the labels
     * and adding every piece of data to it's correct location in the memory*/
    while (fgets(line, MAX_LINE, expanded_file_handler) != NULL)
    {
        get_first_token(line, word);

        if (!strcmp(word, ".extern")) /*Case for extern labels*/
        {
            is_code = TRUE;
            get_next_token(line, word, get_next_token_index(line, 0));

            if (!is_valid_label_name(word))
            {
                /*raises error flag but continue to search for more errors in
                 * the first pass*/
                g_error = TRUE;
                fprintf(stderr,
                        "Error at line %d: '%s' is an illegal label name\n",
                        line_number, word);
            }
            else if (is_label_exists(word) && !is_defined_external(word))
            {
                /*the file is allowed to define the same
                external more than once without causing an error*/
                g_error = TRUE;
                fprintf(stderr, "Error at line %d: '%s' label already exists\n",
                        line_number, word);
            }
            /*If we reached here the label is a valid external label, and we can
             * add it to the symbols array*/
            label_add(label_counter++, word, 0, "external", ++array_size);
            get_next_token(line, word, get_next_token_index(line, 0));
            if (line[0] != '\0')
            {
                g_error = TRUE;
                fprintf(
                    stderr,
                    "Error at line %d: Extended text after extern variable\n",
                    line_number);
            }
        }
        /*Case for inner labels*/
        else if (word[strlen(word) - 1] == ':')
        {
            strcpy(label_name, word);
            remove_colon(label_name);
            if (!is_valid_label_name(label_name))
            {
                g_error = TRUE;
                fprintf(stderr, "Error at line %d: '%s' is an illegal label name\n",
                        line_number, label_name);
            }
            else if (is_label_exists(label_name))
            {
                g_error = TRUE;
                fprintf(stderr, "Error at line %d: '%s' label already exists\n",
                        line_number, label_name);
            }
            get_next_token(line, word, get_next_token_index(line, 0));
            if (!strcmp(word, ".data") || !strcmp(word, ".string"))
            {
                is_code = FALSE;
                if (get_current_address(prv_DC) != 0)
                    label_add(label_counter++, label_name, get_current_address(prv_DC) + 1, "data", ++array_size);
                else
                    label_add(label_counter++, label_name, get_current_address(prv_DC), "data", ++array_size);
            }
            else if (!strcmp(word, ".entry") || !strcmp(word, ".extern"))
                fprintf(stderr, "Warning at line %d: undefined attribute to label.\n",
                        line_number);
            else if (is_operation_name(word))
                if (get_current_address(prv_IC) != BASE_STOCK)
                    label_add(label_counter++, label_name, get_current_address(prv_IC) + 1, "code", ++array_size);
                else
                    label_add(label_counter++, label_name, get_current_address(prv_IC), "code", ++array_size);
            else
            {
                g_error = TRUE;
                fprintf(stderr, "Error at line %d: undefined operation.\n", line_number);
            }
        }

        /*in this point the line contain only code*/
        if (!is_empty_line(line) && is_code == TRUE && (strcmp(word, ".data") != 0 && strcmp(word, ".string") != 0) && strcmp(word, ".entry") != 0 && strcmp(word, ".extern"))
            command_code_process(&prv_IC, line);
        else if (!is_empty_line(line) && strcmp(word, ".data") == 0)
            command_data_process(&prv_DC, line);
        else if (!is_empty_line(line) && strcmp(word, ".string") == 0)
            command_string_process(&prv_DC, line);

        if (head_IC->stock_index > MAX_ADDRESS)
        { /*checks if we didn't use too much memory*/
            fprintf(stderr, "Error: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        line_number++;
    }

    update_data_labels_address(prv_IC->stock_index);
    update_address(head_DC, prv_IC->stock_index);

    /* TODO: unit test functions for command_data/code/string functions*/
    /* TODO: update global variables into first_pass scope*/

    return g_error;
}

/***********************************************************************
 * The next functions are related to creating, defining and checking
 * validity of symbols
 **********************************************************************/
/*Function that accepts a name and checks if a symbol name is already
  definedand has "external" attribute with the same given name*/
int is_defined_external(char *word)
{
    unsigned int i = 0;
    while (strcmp(symbols_table[i].name, ""))
    {
        if (!strcmp(symbols_table[i].name, word) &&
            !strcmp(symbols_table[i].attribute, "external"))
            return 1;
    }
    return 0;
}

/*Function that accept the location in the symbols array to initialize a new
 * symbol*/
void label_init(unsigned int count)
{
    if (count == 0)
        if ((symbols_table = calloc(1, sizeof(TypeLabel))) == NULL)
        {
            fprintf(stderr, "FAILED!\nError: Out of memory\n");
            exit(EXIT_FAILURE);
        }

    if ((symbols_table[count].name = (char *)malloc(MAX_LABEL_LENGTH)) == NULL)
    {
        fprintf(stderr, "FAILED!\nError: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    if ((symbols_table[count].attribute = (char *)malloc(MAX_LABEL_LENGTH)) ==
        NULL)
    {
        fprintf(stderr, "FAILED!\nError: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    strcpy(symbols_table[count].name, "");
    strcpy(symbols_table[count].attribute, "");
    symbols_table[count].address = 0;
    symbols_table[count].base_address = 0;
    symbols_table[count].offset = 0;
}

/*Function to add new symbol and initialize a new symbol in the next address*/
void label_add(unsigned int count, char *word, unsigned int address,
               char *attribute, size_t array_size)
{
    strcpy(symbols_table[count].name, word);
    strcpy(symbols_table[count].attribute, attribute);
    symbols_table[count].address = address;
    symbols_table[count].base_address = base_address(address);
    symbols_table[count].offset = (address % BASE_MODE);

    if ((symbols_table = realloc(symbols_table, array_size * sizeof(TypeLabel))) == NULL)
    {
        fprintf(stderr, "FAILED!\nError: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    label_init(++count);
}
void update_data_labels_address(int last_address)
{
    unsigned int i = 0;
    while (strcmp(symbols_table[i].name, ""))
    {
        if (!strcmp(symbols_table[i].attribute, "data"))
        {
            symbols_table[i].address += (last_address + 1);
            symbols_table[i].base_address = base_address(symbols_table[i].address);
            symbols_table[i].offset = (symbols_table[i].address % BASE_MODE);
        }
        i++;
    }
}

/*This function frees all the strings inside the symbol array*/
void free_symbols_str()
{
    unsigned int i = 0;
    while (strcmp(symbols_table[i].name, ""))
    {
        free(symbols_table[i].name);
        free(symbols_table[i++].attribute);
    }
    free(symbols_table[i].name);
    free(symbols_table[i].attribute);
}

/*Function that checks if a given name of label is a valid name*/
int is_valid_label_name(char *word)
{
    const char *registers_array[] = {"r0", "r1", "r2", "r3", "r4", "r5",
                                     "r6", "r7", "r8", "r9", "r10", "r11",
                                     "r12", "r13", "r14", "r15"};
    const char *operations_array[] = {"mov", "cmp", "add", "sub", "lea", "clr",
                                      "not", "inc", "dec", "jmp", "bne", "jsr",
                                      "red", "prn", "rts", "stop"};
    unsigned int i;
    unsigned int arrays_length = 16;

    if (!isalpha(word[0]))
        return 0;
    if (strlen(word) > MAX_LABEL_LENGTH)
        return 0;
    for (i = 0; i < arrays_length; i++)
        if (!strcmp(word, registers_array[i]) || !strcmp(word, operations_array[i]))
            return 0;
    if (!strcmp(word, "A") || !strcmp(word, "R") || !strcmp(word, "E"))
        return 0;
    return 1;
}

/*Function that checks if a given name of label is already exist in the symbols
 * array*/
int is_label_exists(char *word)
{
    unsigned int i;
    for (i = 0; strcmp(symbols_table[i].name, ""); i++)
        if (!strcmp(symbols_table[i].name, word))
            return 1;

    return 0;
}

/*Function that prints labels(just for show).*/
void print_labels()
{
    unsigned int i;
    printf("Name\t|\taddress\t|\tbase\t|\toffset\t|\tattributes\n");
    printf(
        "--------|---------------|---------------|---------------|-------------"
        "------\n");
    for (i = 0; strcmp(symbols_table[i].name, ""); i++)
    {
        printf("%s\t|\t%d\t|\t%d\t|\t%d\t|\t%s\n", symbols_table[i].name,
               symbols_table[i].address, symbols_table[i].base_address,
               symbols_table[i].offset, symbols_table[i].attribute);
    }
}

/********************************************************************************************
 * The next functions are used to count and calculate binary words and their
 *address
 ******************************************************************************************/
/*Function to calculate the base address from a given address*/
int base_address(int address)
{
    if (address == 0)
        return 0;
    return (address - (address % BASE_MODE));
}

/*Function to count the number of binary words inside a line*/
int get_binary_words(char *line, char *word) { return 1; }

/*******************************************************************
 * The next functions related to finding and checking commands
 *******************************************************************/
/*Function that checks if a given word is an operation*/
int is_operation_name(char *word)
{
    const char *operations_array[] = {"mov", "cmp", "add", "sub", "lea", "clr",
                                      "not", "inc", "dec", "jmp", "bne", "jsr",
                                      "red", "prn", "rts", "stop"};
    unsigned int i = 0, found = 0;
    unsigned int arrays_length = 16;

    while (i < arrays_length)
        if (!strcmp(word, operations_array[i++]))
            found = 1;

    return found;
}