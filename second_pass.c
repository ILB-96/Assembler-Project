#include "assembler.h"
int process_entry_label(char *, char *, int, TypeLabel *, FILE **, int *, char *);

/*
 *Second Pass to process entry labels and add the labels to their place in the binary words img
 *@return 1 if found an error in the process
 */
int second_pass(FILE *exp_file_handler, char *file_name, plw head_IC,
                plw head_DC, TypeLabel *symbols_table)
{
  int line_number = FIRST_LINE; /*count lines in file.am*/
  int error = FALSE;            /*error flag that resets every line*/
  int g_error = FALSE;          /*TRUE if found error in atleast one line*/
  int label_base_val = 0;       /*stores the base value of label*/
  int label_offset_val = 0;     /*stores the offset value of label*/
  int ext_file_created = FALSE; /*stores if extern file already exists*/
  int ent_file_created = FALSE; /*stores if extern file already exists*/
  char line[MAX_LINE] = "\0";
  char token[MAX_LINE] = "\0";
  FILE *obj_file_handler = NULL;
  FILE *ent_file_handler = NULL;
  FILE *ext_file_handler = NULL;

  while (fgets(line, MAX_LINE, exp_file_handler))
  {
    get_first_token(line, token);

    /*case for entry instruction line*/
    if (!strcmp(token, ".entry"))
    {
      get_next_token(line, token);
      error = process_entry_label(line, token, line_number, symbols_table, &ent_file_handler, &ent_file_created, file_name);
    }
    else if (token[strlen(token) - 1] == ':')
      /*skips label declartion(was dealt with in the first pass)*/
      get_next_token(line, token);
    else if (!strcmp(token, ".extern"))
    {
      /*skips extern instruction(was dealt with in the first pass)*/
      get_next_token(line, token);
      get_next_token(line, token);
    }

    if (!is_empty_line(line) && !error && found_label(line, token, symbols_table))
    {
      /*we found a label and need to store its base and offset values in
      the next two empty data binary word*/
      error = get_label_values(token, &label_base_val, &label_offset_val, line_number, symbols_table);
      if (!error && !label_base_val) /*case for an extern label*/
      {
        /*we need to create an extern file if one doesn't exists*/
        if (!ext_file_created)
        {
          if (load_file(&ext_file_handler, file_name, ".ext", "w"))
            return 1;
          ext_file_created = TRUE;
        }
        /*Here we output to file.ext the values of an extern label
        and add extern values to the next empty couple of binary words*/
        fprintf(ext_file_handler, "%s BASE %d\n", token,
                set_next_empty(head_IC, E, label_base_val));
        fprintf(ext_file_handler, "%s OFFSET %d\n\n", token,
                set_next_empty(head_IC, E, label_offset_val));
      }
      else if (!error)
      {
        set_next_empty(head_IC, R, label_base_val);
        set_next_empty(head_IC, R, label_offset_val);
      }
    }
    if (error)
    {
      g_error = TRUE;
      error = FALSE;
    }
    line_number++;
  }
  if (!g_error)
  {
    /*creates an objects file*/
    if (load_file(&obj_file_handler, file_name, ".ob", "w"))
      return 1;
    fprintf(obj_file_handler, "\t%d %d\n", get_length(head_IC),
            get_length(head_DC));
    load_obj_file(head_IC, obj_file_handler);
    load_obj_file(head_DC, obj_file_handler);
    fclose(obj_file_handler);
  }
  if (ext_file_created)
  {
    fclose(ext_file_handler);
    if (g_error)
      remove_file(file_name, ".ext");
  }
  if (ent_file_created)
  {
    fclose(ent_file_handler);
    if (g_error)
      remove_file(file_name, ".ent");
  }

  return g_error;
}

/*
Adds entry attribute to label and adds the label to file.ent
*return 1 if found an error in the process
*/
int process_entry_label(char *line, char *token, int line_number, TypeLabel *symbols_table, FILE **ent_file_handler, int *is_file_created, char *file_name)
{
  int error = FALSE;
  int label_base_val = 0;
  int label_offset_val = 0;
  if (is_label_exists(token, symbols_table))
  {
    if (is_defined_ext(token, symbols_table))
    {
      error = TRUE;
      fprintf(stdout,
              "Error at line %d: label cannot be defined as external and as entry\n",
              line_number);
    }
    else if (!is_defined_ent(token, symbols_table))
    {

      add_entry_attribute(token, symbols_table);
      if (!(*is_file_created))
      {
        if (load_file(ent_file_handler, file_name, ".ent", "w"))
          return 1;
        *is_file_created = TRUE;
      }
      error = get_label_values(token, &label_base_val, &label_offset_val, line_number, symbols_table);
      if (!error)
        fprintf(*ent_file_handler, "%s, %d, %d\n", token, label_base_val,
                label_offset_val);

      get_next_token(line, token);
      if (!is_empty_line(line))
      {
        error = TRUE;
        fprintf(stdout,
                "Error at line %d: Extended text after entry variable\n",
                line_number);
      }
    }
  }
  else
  {
    error = TRUE;
    fprintf(stdout, "Error at line %d: '%s' label not found\n",
            line_number, token);
  }
  return error;
}
