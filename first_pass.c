#include "assembler.h"

/*Inner functions that are meant to use only inside this source*/
int add_binary_words(char *, char *, plw *, plw *, int);
int process_extern(char *, char *, int, int *, size_t *, TypeLabel **);
int process_label(char *, char *, int, int *, size_t *, plw, plw, TypeLabel **);

/*
 *First Pass process the labels and create the binary words image.
 *@return 1 if found an error in the process
 */
int first_pass(FILE *exp_file_handler, plw *h_I, plw *p_I, plw *h_D, plw *p_D, TypeLabel **symbols_table)
{
  int label_counter = 0;
  int line_number = FIRST_LINE;
  int error = FALSE;
  int g_error = FALSE;
  size_t array_size = BASE_ARR_SIZE;
  char line[MAX_LINE] = "\0";
  char token[MAX_LINE] = "\0";

  /*Initialize labels, IC and DC*/
  plw head_IC;
  plw prv_IC;
  plw head_DC;
  plw prv_DC;
  initialize_list(&head_DC, &prv_DC, 0);
  initialize_list(&head_IC, &prv_IC, BASE_IC);
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
      fprintf(stdout, "Error: Out of memory\n");
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
/*
 *Handles new labels declartions(adds them to the symbols table
 *@return 1 if found an error in the declartion
 */
int process_label(char *line, char *token, int line_number,
                  int *label_counter, size_t *array_size, plw prv_IC, plw prv_DC, TypeLabel **symbols_table)
{
  int error = FALSE;
  char label_name[MAX_LINE] = "\0";
  strcpy(label_name, token);
  remove_colon(label_name);
  if (!is_valid_label_name(label_name))
  {
    error = TRUE;
    fprintf(stdout, "Error at line %d: '%s' is an illegal label name\n",
            line_number, label_name);
  }
  else if (is_label_exists(label_name, *symbols_table))
  {
    error = TRUE;
    fprintf(stdout, "Error at line %d: '%s' label already exists\n",
            line_number, label_name);
  }
  get_next_token(line, token);
  if (!strcmp(token, ".data") || !strcmp(token, ".string"))
  {
    label_add((*label_counter)++, label_name, get_current_address(prv_DC),
              "data", array_size, symbols_table);
  }
  else if (!strcmp(token, ".entry") || !strcmp(token, ".extern"))
  {
    fprintf(stdout, "Warning at line %d: undefined attribute to label.\n",
            line_number);
    label_add((*label_counter)++, label_name, get_current_address(prv_DC),
              "undef", array_size, symbols_table);
  }
  else if (is_operation_name(token))
    label_add((*label_counter)++, label_name, get_current_address(prv_IC),
              "code", array_size, symbols_table);
  else
  {
    error = TRUE;
    fprintf(stdout, "Error at line %d: undefined operation.\n",
            line_number);
  }
  return error;
}

/*
 *Handles external labels(adds them to the symbols table)
 *@return 1 if found an error in the declartion of the label.
 */
int process_extern(char *line, char *token, int line_number, int *label_counter, size_t *array_size, TypeLabel **symbols_table)
{
  int error = FALSE;
  get_next_token(line, token);
  if (!is_valid_label_name(token))
  {
    error = TRUE;
    fprintf(stdout, "Error at line %d: '%s' is an illegal label name\n",
            line_number, token);
  }
  else if (is_label_exists(token, *symbols_table))
  {
    if (!is_defined_ext(token, *symbols_table))
    {
      /*the file is allowed to define the same
      external more than once without causing an error*/
      error = TRUE;
      fprintf(stdout, "Error at line %d: '%s' label already exists\n",
              line_number, token);
    }
  }
  else
  {
    /*If we reached here the label is a valid external label, and we can
     * add it to the symbols array*/
    label_add((*label_counter)++, token, EXTERN_VAL, "external", array_size, symbols_table);
  }
  get_next_token(line, token);
  if (!is_empty_line(line))
  {
    error = TRUE;
    fprintf(stdout,
            "Error at line %d: Extended text after extern variable\n",
            line_number);
  }
  return error;
}

/*
 *Creates the data image of IC and DC
 *@return 1 if found an error
 */
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