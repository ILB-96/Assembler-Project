#include "assembler.h"
#define MAX_LABEL_LENGTH 31

/*Inner functions that are meant to use only inside this source*/
int is_valid_label_name(char *);
int base_address(int);
void label_init(int, TypeLabel **);
void label_add(int, char *, int, char *, size_t, TypeLabel **);
int add_binary_words(char *, char *, plw *, plw *, int);
void update_data_labels_address(int, TypeLabel *);
int process_extern(char *, char *, int, int *, size_t *, TypeLabel **);
int process_label(char *, char *, int, int *, size_t *, plw, plw, TypeLabel **);

/*Defining and declaring the symbols table*/
struct TypeLabel
{
  char *name;
  int address;
  int base_address;
  int offset;
  char *attribute;
};
/*First pass to process the labels and create the binary words image*/
int first_pass(FILE *exp_file_handler, plw *h_I, plw *p_I, plw *h_D, plw *p_D, TypeLabel **symbols_table)
{
  int label_counter = 0;
  int line_number = 1;
  int error = FALSE;
  int g_error = FALSE;
  size_t array_size = 1;
  char line[MAX_LINE];
  char token[MAX_LINE];

  /*Initialize labels, IC and DC*/
  plw head_IC;
  plw prv_IC;
  plw head_DC;
  plw prv_DC;
  initialize_list(&head_DC, &prv_DC, 0);
  initialize_list(&head_IC, &prv_IC, BASE_STOCK);
  label_init(0, symbols_table);

  /*This loop will go through the entire expanded file, creating the labels
   * and adding every piece of data to it's correct location in the memory*/
  while (fgets(line, MAX_LINE, exp_file_handler))
  {
    get_first_token(line, token);
    /*Case for extern labels*/
    if (!strcmp(token, ".extern"))
      error = process_extern(line, token, line_number, &label_counter, &array_size, symbols_table);
    /*Case for labels definition*/
    else if (token[strlen(token) - 1] == ':')
      error = process_label(line, token, line_number, &label_counter, &array_size, prv_IC, prv_DC, symbols_table);

    /*At this point the line doesn't contain a label definition*/
    if (!error && !is_empty_line(line))
      error = add_binary_words(line, token, &prv_IC, &prv_DC, line_number);

    /*Case for max memory usage*/
    if (get_current_address(prv_IC) + get_current_address(prv_DC) + 1 >
        MAX_ADDRESS)
    {
      fprintf(stderr, "Error: Out of memory\n");
      exit(EXIT_FAILURE);
    }
    line_number++;
    /*logic: we found error in line so the file is faulty but we continue to search for errors*/
    if (error)
    {
      g_error = error;
      error = FALSE;
    }
  }

  update_data_labels_address(prv_IC->stock_index, *symbols_table);
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
/*Function that handles new labels declartions(adds them to the symbols table*/
int process_label(char *line, char *token, int line_number,
                  int *label_counter, size_t *array_size, plw prv_IC, plw prv_DC, TypeLabel **symbols_table)
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
  else if (is_label_exists(label_name, *symbols_table))
  {
    error = TRUE;
    fprintf(stderr, "Error at line %d: '%s' label already exists\n",
            line_number, label_name);
  }
  get_next_token(line, token);
  if (!strcmp(token, ".data") || !strcmp(token, ".string"))
  {
    label_add((*label_counter)++, label_name, get_current_address(prv_DC),
              "data", ++(*array_size), symbols_table);
  }
  else if (!strcmp(token, ".entry") || !strcmp(token, ".extern"))
  {
    fprintf(stderr, "Warning at line %d: undefined attribute to label.\n",
            line_number);
    label_add((*label_counter)++, label_name, get_current_address(prv_DC),
              "undef", ++(*array_size), symbols_table);
  }
  else if (is_operation_name(token))
    label_add((*label_counter)++, label_name, get_current_address(prv_IC),
              "code", ++(*array_size), symbols_table);
  else
  {
    error = TRUE;
    fprintf(stderr, "Error at line %d: undefined operation.\n",
            line_number);
  }
  return error;
}

/*Function that handles external labels(adds them to the symbols table)*/
int process_extern(char *line, char *token, int line_number, int *label_counter, size_t *array_size, TypeLabel **symbols_table)
{
  int error = FALSE;
  get_next_token(line, token);
  if (!is_valid_label_name(token))
  {
    error = TRUE;
    fprintf(stderr, "Error at line %d: '%s' is an illegal label name\n",
            line_number, token);
  }
  else if (is_label_exists(token, *symbols_table))
  {
    if (!is_defined_ext(token, *symbols_table))
    {
      /*the file is allowed to define the same
      external more than once without causing an error*/
      error = TRUE;
      fprintf(stderr, "Error at line %d: '%s' label already exists\n",
              line_number, token);
    }
  }
  else
  {
    /*If we reached here the label is a valid external label, and we can
     * add it to the symbols array*/
    label_add((*label_counter)++, token, 0, "external", ++(*array_size), symbols_table);
  }
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

/*Function that checks if inserted label name already have external attribute*/
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

/*Functions that checks if inserted label already have entry attribute*/
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

/*Function that accept the location in the symbols array to initialize a new
 * symbol*/
void label_init(int count, TypeLabel **symbols_table)
{
  if (count == 0)
    if ((*symbols_table = calloc(1, sizeof(TypeLabel))) == NULL)
    {
      fprintf(stderr, "FAILED!\nError: Out of memory\n");
      exit(EXIT_FAILURE);
    }

  if (((*symbols_table)[count].name = (char *)malloc(MAX_LABEL_LENGTH)) == NULL)
  {
    fprintf(stderr, "Error: Out of memory\n");
    exit(EXIT_FAILURE);
  }
  if (((*symbols_table)[count].attribute = (char *)malloc(MAX_LABEL_LENGTH)) ==
      NULL)
  {
    fprintf(stderr, "Error: Out of memory\n");
    exit(EXIT_FAILURE);
  }
  strcpy((*symbols_table)[count].name, "");
  strcpy((*symbols_table)[count].attribute, "");
  (*symbols_table)[count].address = 0;
  (*symbols_table)[count].base_address = 0;
  (*symbols_table)[count].offset = 0;
}

/*Function to add new symbol and initialize a new symbol in the next address*/
void label_add(int count, char *token, int address, char *attribute,
               size_t array_size, TypeLabel **symbols_table)
{
  strcpy((*symbols_table)[count].name, token);
  strcpy((*symbols_table)[count].attribute, attribute);
  if (address != BASE_DC && address != BASE_IC)
    (*symbols_table)[count].address = address + 1;
  else
    (*symbols_table)[count].address = address;
  (*symbols_table)[count].base_address = base_address(address);
  (*symbols_table)[count].offset = ((*symbols_table)[count].address % BASE_MODE);

  if (!(*symbols_table =
            realloc(*symbols_table, array_size * sizeof(TypeLabel))))
  {
    fprintf(stderr, "Error: Out of memory\n");
    exit(EXIT_FAILURE);
  }
  label_init(++count, symbols_table);
}
/*Function that adds entry attribute to a given valid label*/
void add_entry_attribute(char *token, TypeLabel *symbols_table)
{
  int i;
  char entry_attribute[] = ", entry";
  for (i = 0; strcmp(symbols_table[i].name, ""); i++)
    if (!strcmp(symbols_table[i].name, token))
      strcat(symbols_table[i].attribute, entry_attribute);
}

/*Function that update the address of the data labels
to the end of the the IC address so that they will have a valid address*/
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

/*This function frees all the strings inside the symbol array(and the array itself)*/
void free_symbols(TypeLabel *symbols_table)
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
  int result = 1;
  if (!isalpha(token[0]) || strlen(token) > MAX_LABEL_LENGTH || !strcmp(token, "A") ||
      !strcmp(token, "R") || !strcmp(token, "E") || !strcmp(token, "macro") || !strcmp(token, "endm") || !strcmp(token, "data") ||
      !strcmp(token, "string") || !strcmp(token, "entry") || !strcmp(token, "extern"))
    result = 0;
  for (i = 0; result && i < arrays_length; i++)
    if (!strcmp(token, registers_array[i]) || !strcmp(token, operations_array[i]))
      result = 0;
  return result;
}

/*Function that checks if a given name of label is already exist in the symbols
 * array*/
int is_label_exists(char *label_name, TypeLabel *symbols_table)
{
  int i;
  for (i = 0; strcmp(symbols_table[i].name, ""); i++)
    if (!strcmp(symbols_table[i].name, label_name))
      return 1;

  return 0;
}

/*Function that finds and return the base and offset values of a given label*/
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
  fprintf(stderr, "Error at line %d: '%s' label not found\n",
          line_number, token);
  return 1;
}

/*Function that return TRUE if a given line contains a label*/
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

/*Function that prints labels(just for show).*/
void print_labels(TypeLabel *symbols_table)
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

/*Function that creates the data image of IC and DC*/
int add_binary_words(char *line, char *token, plw *prv_IC, plw *prv_DC, int line_number)
{
  int error = FALSE;
  if (!strcmp(token, ".data"))
    error = command_data_process(prv_DC, line, line_number);
  else if (!strcmp(token, ".string"))
    error = command_string_process(prv_DC, line, line_number);
  else if (strcmp(token, ".entry") != 0)
    error = command_code_process(prv_IC, line, line_number);
  return error;
}