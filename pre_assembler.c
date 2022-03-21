#include "assembler.h"

/*inner functions in use only at pre_assembler.c*/
void expand_macros(FILE *, FILE *);

/*
 *Pre Assembler to expand and refactor the original file into a new file
 *to eas the assembly process
 *@return 1 if filed to process the inserted file.
 */
int pre_assembler(FILE **exp_file_handler, char *file_name)
{
  FILE *file_handler;
  if (load_file(&file_handler, file_name, ".as", "r") || load_file(&*exp_file_handler, file_name, ".am", "w+"))
    return 1;

  expand_macros(file_handler, *exp_file_handler);
  fclose(file_handler);

  return 0;
}

/*
 *Expands the macros in a new file
 *will also remove comment lines, space lines and leading spaces in line
 *@return 1 if failed to create a macro file
 */
void expand_macros(FILE *file_handler, FILE *exp_file_handler)
{
  TypeMacro *macros_table;
  int macros_counter = 0;
  size_t array_size = BASE_ARR_SIZE;
  char line[MAX_LINE] = "\0", token[MAX_LINE] = "\0";
  int macro_number = 0;
  macros_init(0, &macros_table);

  while (fgets(line, sizeof(line), file_handler))
  {
    get_first_token(line, token);
    if (!strcmp(token, "macro"))
    {
      get_next_token(line, token);
      macro_add(line, token, macros_counter++, &array_size, file_handler, &macros_table);
    }
    else
    {
      if ((macro_number = is_macro_name(token, macros_table)) >= 0)
      {
        insert_macro(exp_file_handler, macros_table, token, macro_number);
      }
      else if (!is_empty_line(line) && !is_comment_line(line))
        fprintf(exp_file_handler, "%s", line);
    }
  }
  free(macros_table);
}
