#include "assembler.h"
#define MAX_LABEL_LEN 31
#define MAX_ATTRIBUTE_LEN 16
/*Defining and declaring the symbols table*/
struct TypeLabel
{
    char name[MAX_LABEL_LEN];
    int address;
    int base_address;
    int offset;
    char attribute[MAX_ATTRIBUTE_LEN];
};
/*
*Checks if a given label name already have external attribute
@return 1 if found an external attribute in the label
*/
int is_defined_ext(char *token, TypeLabel *symbols_table)
{
    int i = 0;
    while (strcmp(symbols_table[i].name, ""))
    {
        if (!strcmp(symbols_table[i].name, token) &&
            !strcmp(symbols_table[i].attribute, "external"))
            return 1;
        i++;
    }
    return 0;
}

/*
*Checks if inserted label already have entry attribute
@return 1 if found entry attribute in the label
*/
int is_defined_ent(char *token, TypeLabel *symbols_table)
{
    int i = 0;
    while (strcmp(symbols_table[i].name, ""))
    {
        if (!strcmp(symbols_table[i].name, token) &&
            strstr(symbols_table[i].attribute, "entry"))
            return 1;
        i++;
    }
    return 0;
}

/*
 *Accepts the location in the symbols array to initialize a new symbol
 */
void label_init(int count, TypeLabel **symbols_table)
{
    if (count == 0)
        if ((*symbols_table = calloc(BASE_ARR_SIZE, sizeof(TypeLabel))) == NULL)
        {
            fprintf(stdout, "Error: Out of memory\n");
            exit(EXIT_FAILURE);
        }

    strcpy((*symbols_table)[count].name, "");
    strcpy((*symbols_table)[count].attribute, "");
    (*symbols_table)[count].address = 0;
    (*symbols_table)[count].base_address = 0;
    (*symbols_table)[count].offset = 0;
}

/*
 *Adds new symbol and initialize a new symbol in the next address
 */
void label_add(int count, char *label_name, int address, char *attribute,
               size_t *array_size, TypeLabel **symbols_table)
{
    strcpy((*symbols_table)[count].name, label_name);
    strcpy((*symbols_table)[count].attribute, attribute);
    if (address != BASE_DC && address != BASE_IC)
        (*symbols_table)[count].address = address + 1;
    else
        (*symbols_table)[count].address = address;
    (*symbols_table)[count].base_address = base_address(address);
    (*symbols_table)[count].offset = ((*symbols_table)[count].address % BASE_MODE);
    if (count == *array_size - 1)
    {
        *array_size *= 2;
        if (!(*symbols_table = realloc(*symbols_table, *array_size * sizeof(TypeLabel))))
        {
            fprintf(stdout, "Error: Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    label_init(++count, symbols_table);
}

/*
 *Adds entry attribute to a given valid label.
 */
void add_entry_attribute(char *token, TypeLabel *symbols_table)
{
    int i;
    char entry_attribute[] = ", entry";
    for (i = 0; strcmp(symbols_table[i].name, ""); i++)
        if (!strcmp(symbols_table[i].name, token))
            strcat(symbols_table[i].attribute, entry_attribute);
}

/*
 *Updates the address of the data labels
 *to the end of the the IC address so that they will have a valid address
 */
void update_data_labels_address(int last_address, TypeLabel *symbols_table)
{
    int i = 0;
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

/*
Checks if a given name of label is a valid name
*@token char pointer to the label name.
*@return 1 if label name is legal.
*/
int is_valid_label_name(char *token)
{
    const char *registers_array[] = {"r0", "r1", "r2", "r3", "r4", "r5",
                                     "r6", "r7", "r8", "r9", "r10", "r11",
                                     "r12", "r13", "r14", "r15"};
    const char *operations_array[] = {"mov", "cmp", "add", "sub", "lea", "clr",
                                      "not", "inc", "dec", "jmp", "bne", "jsr",
                                      "red", "prn", "rts", "stop"};

    int i;
    int result = 1;
    if (!isalpha(token[0]) || strlen(token) > MAX_LABEL_LEN || !strcmp(token, "A") ||
        !strcmp(token, "R") || !strcmp(token, "E") || !strcmp(token, "macro") || !strcmp(token, "endm") || !strcmp(token, "data") ||
        !strcmp(token, "string") || !strcmp(token, "entry") || !strcmp(token, "extern"))
        result = 0;
    for (i = 0; result && i < SUM_OPERATIONS; i++)
        if (!strcmp(token, registers_array[i]) || !strcmp(token, operations_array[i]))
            result = 0;
    return result;
}

/*
 *Checks if label is already defined
 *@label_name char pointer to the name of the label.
 *@return 1 if found a label with the given name
 */
int is_label_exists(char *label_name, TypeLabel *symbols_table)
{
    int i;
    for (i = 0; strcmp(symbols_table[i].name, ""); i++)
        if (!strcmp(symbols_table[i].name, label_name))
            return 1;

    return 0;
}

/*
Finds the base and offset values of a given label
*@label_base_val will point to the base value of the label.
*@labe_offset_val will point to the offset value of the label.
*@return 1 if label not found
*/
int get_label_values(char *token, int *label_base_val, int *label_offset_val, int line_number, TypeLabel *symbols_table)
{
    int i;
    for (i = 0; strcmp(symbols_table[i].name, ""); i++)
        if (!strcmp(symbols_table[i].name, token))
        {
            *label_base_val = symbols_table[i].base_address;
            *label_offset_val = symbols_table[i].offset;
            return 0;
        }
    fprintf(stdout, "Error at line %d: '%s' label not found\n",
            line_number, token);
    return 1;
}

/*
 *Checks if a given line contains a label
 *@token will point to the label if found one
 *@return 1 if found a label.
 */
int found_label(char *line, char *token, TypeLabel *symbols_table)
{
    while (!is_empty_line(line))
    {
        remove_signs(token);
        if (!is_valid_label_name(token))
            get_next_token(line, token);
        else
            return TRUE;
    }
    return FALSE;
}

/********************************************************************************************
 * The next functions are used to count and calculate binary words and their
 *address
 ******************************************************************************************/
/*
 *Calculates the base address from a given address
 *@return The base address of the label.
 */
int base_address(int address)
{
    if (address == 0)
        return 0;
    return (address - (address % BASE_MODE));
}