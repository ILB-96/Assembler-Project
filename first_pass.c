#include "assembler.h"
#define MAX_LABEL_LENGTH 32

/*Inner functions that are meant to use only inside this source*/
int is_valid_label_name(char *);
int base_address(int);
void label_init(int count);
void label_add(int, char *, int, char *, size_t);
void update_data_labels_address(int);
int process_extern(char *, char *, int, int *, size_t *);
int process_label(int *, char *, char *, int, int *, size_t *, plw, plw);
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

/*Instruction and data counter are global variable to use in the Second Pass*/

/*TODO: maybe we need to add more checks to see variables are
not names after saved words?*/

/*Those arrays are saved words, better to have them globally,
so we can always check if a variable is trying to use those name*/

int first_pass(FILE *exp_file_handler, plw *h_I, plw *p_I, plw *h_D, plw *p_D)
{
  /*Variables*/
  int label_counter = 0;
  int line_number = 1;
  int error = FALSE;
  int g_error = FALSE;
  int is_code = TRUE;
  size_t array_size = 1;
  char line[MAX_LINE];
  char token[MAX_LINE];

  plw head_IC; /*need to be free in the end...*/
  plw prv_IC;
  plw head_DC;
  plw prv_DC;
  initialize_list(&head_DC, &prv_DC, 0);
  initialize_list(&head_IC, &prv_IC, BASE_STOCK);
  /*First initialize the symbols' table dynamic array and error flag*/

  label_init(0);
  /*TODO: use error flag to continue looking for errors in the program and
   * stop the program after that*/
  /*This loop will go through the entire expanded file, creating the labels
   * and adding every piece of data to it's correct location in the memory*/
  while (fgets(line, MAX_LINE, exp_file_handler))
  {
    get_first_token(line, token);
    is_code = TRUE;
    /*Case for extern labels*/
    if (!strcmp(token, ".extern"))
      error = process_extern(line, token, line_number, &label_counter, &array_size);
    /*Case for inner labels*/
    else if (token[strlen(token) - 1] == ':')
      error = process_label(&is_code, line, token, line_number, &label_counter, &array_size, prv_IC, prv_DC);

    /*in this point the line contain only code*/
    if (!error && !is_empty_line(line) && is_code == TRUE &&
        (strcmp(token, ".data") != 0 && strcmp(token, ".string") != 0) &&
        strcmp(token, ".entry") != 0 && strcmp(token, ".extern"))
      error = command_code_process(&prv_IC, line, line_number);
    else if (!error && !is_empty_line(line) && !strcmp(token, ".data"))
      error = command_data_process(&prv_DC, line, line_number);
    else if (!error && !is_empty_line(line) && !strcmp(token, ".string"))
      error = command_string_process(&prv_DC, line, line_number);

    if (get_current_address(prv_IC) + get_current_address(prv_DC) + 1 >
        MAX_ADDRESS)
    { /*checks if we didn't use too much memory*/
      fprintf(stderr, "Error: Out of memory\n");
      exit(EXIT_FAILURE);
    }
    line_number++;
    if (error)
    {
      g_error = error;
      error = FALSE;
    }
  }

  update_data_labels_address(prv_IC->stock_index);
  update_address(head_DC, prv_IC->stock_index);
  *h_I = head_IC;
  *p_I = prv_IC;
  *h_D = head_DC;
  *p_D = prv_DC;
  return g_error;
}

/***********************************************************************
 * The next functions are related to creating, defining and checking
 * validity of symbols
 **********************************************************************/
/*Function that accepts a name and checks if a symbol name is already
  definedand has "external" attribute with the same given name*/
int process_extern(char *line, char *token, int line_number, int *label_counter, size_t *array_size)
{
  int error = FALSE;
  get_next_token(line, token);
  if (!is_valid_label_name(token))
  {
    /*raises error flag but continue to search for more errors in
     * the first pass*/
    error = TRUE;
    fprintf(stderr, "Error at line %d: '%s' is an illegal label name\n",
            line_number, token);
  }
  else if (is_label_exists(token) && !is_defined_external(token))
  {
    /*the file is allowed to define the same
    external more than once without causing an error*/
    error = TRUE;
    fprintf(stderr, "Error at line %d: '%s' label already exists\n",
            line_number, token);
  }
  /*If we reached here the label is a valid external label, and we can
   * add it to the symbols array*/
  label_add((*label_counter)++, token, 0, "external", ++(*array_size));
  get_next_token(line, token);
  if (!is_empty_line(line))
  {
    error = TRUE;
    fprintf(stderr,
            "Error at line %d: Extended text after extern variable\n",
            line_number);
  }
  return error;
}
int is_defined_external(char *token)
{
  int i = 0;
  while (strcmp(symbols_table[i].name, ""))
  {
    if (!strcmp(symbols_table[i].name, token) &&
        !strcmp(symbols_table[i].attribute, "external"))
      return 1;
  }
  return 0;
}
int process_label(int *is_code, char *line, char *token, int line_number,
                  int *label_counter, size_t *array_size, plw prv_IC, plw prv_DC)
{
  int error = FALSE;
  char label_name[MAX_LINE];
  strcpy(label_name, token);
  remove_colon(label_name);
  if (!is_valid_label_name(label_name))
  {
    error = TRUE;
    fprintf(stderr, "Error at line %d: '%s' is an illegal label name\n",
            line_number, label_name);
  }
  else if (is_label_exists(label_name))
  {
    error = TRUE;
    fprintf(stderr, "Error at line %d: '%s' label already exists\n",
            line_number, label_name);
  }
  get_next_token(line, token);
  if (!strcmp(token, ".data") || !strcmp(token, ".string"))
  {
    *is_code = FALSE;
    label_add((*label_counter)++, label_name, get_current_address(prv_DC),
              "data", ++(*array_size));
  }
  else if (!strcmp(token, ".entry") || !strcmp(token, ".extern"))
    fprintf(stderr, "Warning at line %d: undefined attribute to label.\n",
            line_number);
  else if (is_operation_name(token))
    label_add((*label_counter)++, label_name, get_current_address(prv_IC),
              "code", ++(*array_size));
  else
  {
    error = TRUE;
    fprintf(stderr, "Error at line %d: undefined operation.\n",
            line_number);
  }
  return error;
}
/*Function that accept the location in the symbols array to initialize a new
 * symbol*/
void label_init(int count)
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
void label_add(int count, char *token, int address, char *attribute,
               size_t array_size)
{
  strcpy(symbols_table[count].name, token);
  strcpy(symbols_table[count].attribute, attribute);
  if (address != BASE_DC && address != BASE_IC)
    symbols_table[count].address = address + 1;
  else
    symbols_table[count].address = address;
  symbols_table[count].base_address = base_address(address);
  symbols_table[count].offset = (address % BASE_MODE);

  if ((symbols_table =
           realloc(symbols_table, array_size * sizeof(TypeLabel))) == NULL)
  {
    fprintf(stderr, "FAILED!\nError: Out of memory\n");
    exit(EXIT_FAILURE);
  }
  label_init(++count);
}
void add_entry_attribute(char *token)
{
  int i;
  char entry_attribute[] = ", entry";
  for (i = 0; strcmp(symbols_table[i].name, ""); i++)
    if (!strcmp(symbols_table[i].name, token))
      strcat(symbols_table[i].attribute, entry_attribute);
}
void update_data_labels_address(int last_address)
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

/*This function frees all the strings inside the symbol array*/
void free_symbols()
{
  int i = 0;
  while (strcmp(symbols_table[i].name, ""))
  {
    free(symbols_table[i].name);
    free(symbols_table[i++].attribute);
  }
  free(symbols_table[i].name);
  free(symbols_table[i].attribute);
  free(symbols_table);
}

/*Function that checks if a given name of label is a valid name*/
int is_valid_label_name(char *token)
{
  const char *registers_array[] = {"r0", "r1", "r2", "r3", "r4", "r5",
                                   "r6", "r7", "r8", "r9", "r10", "r11",
                                   "r12", "r13", "r14", "r15"};
  const char *operations_array[] = {"mov", "cmp", "add", "sub", "lea", "clr",
                                    "not", "inc", "dec", "jmp", "bne", "jsr",
                                    "red", "prn", "rts", "stop"};
  int i;
  int arrays_length = 16;

  if (!isalpha(token[0]))
    return 0;
  if (strlen(token) > MAX_LABEL_LENGTH)
    return 0;
  for (i = 0; i < arrays_length; i++)
    if (!strcmp(token, registers_array[i]) || !strcmp(token, operations_array[i]))
      return 0;
  if (!strcmp(token, "A") || !strcmp(token, "R") || !strcmp(token, "E"))
    return 0;
  return 1;
}

/*Function that checks if a given name of label is already exist in the symbols
 * array*/
int is_label_exists(char *label_name)
{
  int i;
  for (i = 0; strcmp(symbols_table[i].name, ""); i++)
    if (!strcmp(symbols_table[i].name, label_name))
      return 1;

  return 0;
}
int found_attribute(char *label_name, char *attribute)
{
  int i;
  for (i = 0; strcmp(symbols_table[i].name, ""); i++)
    if (!strcmp(symbols_table[i].name, label_name))
      if (strcmp(symbols_table[i].attribute, attribute))
        return 1;
  return 0;
}
int get_label_values(char *token, int *label_base_val, int *label_offset_val,
                     ARE *are)
{
  int i;
  for (i = 0; strcmp(symbols_table[i].name, ""); i++)
    if (!strcmp(symbols_table[i].name, token))
    {
      *label_base_val = symbols_table[i].base_address;
      *label_offset_val = symbols_table[i].offset;
      if (strstr(symbols_table[i].attribute, "external"))
        *are = E;
      else
        *are = R;
      return 0;
    }
  return 1;
}
int found_label(char *line, char *token)
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
/*Function that prints labels(just for show).*/
void print_labels()
{
  int i;
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

/*******************************************************************
 * The next functions related to finding and checking commands
 *******************************************************************/
/*Function that checks if a given token is an operation*/
int is_operation_name(char *token)
{
  const char *operations_array[] = {"mov", "cmp", "add", "sub", "lea", "clr",
                                    "not", "inc", "dec", "jmp", "bne", "jsr",
                                    "red", "prn", "rts", "stop"};
  int i = 0, found = 0;
  int arrays_length = 16;

  while (i < arrays_length)
    if (!strcmp(token, operations_array[i++]))
      found = 1;

  return found;
}