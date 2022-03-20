#include "assembler.h"
#define AS ".as"

/*inner functions in use only at pre_assembler.c*/
int expand_macros(FILE *, FILE *);
int is_macro_name(char *, FILE *);
void insert_macro(FILE *, FILE *, char *);
void get_macro_name(char *, char *);
void process_macro(char *, char *, int *, FILE *);

/*
 *Pre Assembler to expand and refactor the original file into a new file
 *to eas the assembly process
 *@return 1 if filed to process the inserted file.
 */
int pre_assembler(FILE **exp_file_handler, char *file_name)
{
  FILE *file_handler;
  int error = FALSE;
  if (load_file(&file_handler, file_name, ".as", "r") || load_file(&*exp_file_handler, file_name, ".am", "w+"))
    return 1;

  error = expand_macros(file_handler, *exp_file_handler);
  fclose(file_handler);
  remove("macros_file.txt");

  return error;
}

/*
 *Expands the macros in a new file
 *will also remove comment lines, space lines and leading spaces in line
 *@return 1 if failed to create a macro file
 */
int expand_macros(FILE *file_handler, FILE *exp_file_handler)
{
  FILE *macros_file_handler;
  char line[MAX_LINE] = "\0", token[MAX_LINE] = "\0";
  int is_part_of_macro = 0;
  if (load_file(&macros_file_handler, "macros_file", ".txt", "w+"))
    return 1;

  while (fgets(line, sizeof(line), file_handler))
  {
    get_first_token(line, token);
    if (!strcmp(token, "macro") || is_part_of_macro)
    {
      process_macro(line, token, &is_part_of_macro, macros_file_handler);
    }
    else
    {
      if (is_macro_name(token, macros_file_handler))
      {
        insert_macro(exp_file_handler, macros_file_handler, token);
      }
      else if (!is_empty_line(line) && !is_comment_line(line))
        fprintf(exp_file_handler, "%s", line);
      fseek(macros_file_handler, 0, SEEK_END);
    }
  }

  fclose(macros_file_handler);
  return 0;
}

/*
 *Adds the macro and its content to the macros file
 */
void process_macro(char *line, char *token, int *is_part_of_macro,
                   FILE *macros_file_handler)
{
  if (!strcmp(token, "endm"))
  {
    *is_part_of_macro = 0;
    fprintf(macros_file_handler, "%s\n", token);
  }
  else
  {
    if (!strcmp(token, "macro"))
    {
      *is_part_of_macro = 1;
      get_macro_name(line, token);
      fprintf(macros_file_handler, "%s\n", token);
    }
    else if (!is_empty_line(line) && !is_comment_line(line))
      fprintf(macros_file_handler, "%s", line);
  }
}

/*
 *Saves the macro name
 */
void get_macro_name(char *line, char *token)
{
  int i = 0, j = 0;
  while (isspace(line[i]) && line[i] != '\0')
  {
    i++;
  }
  i = get_next_token_index(line, i);
  while (!isspace(line[i]) && line[i] != '0')
  {
    token[j++] = line[i++];
  }

  token[j] = '\0';
}

/*
 *Checks if we found a macro name in a line
 *return 1 if found a macro name.
 */
int is_macro_name(char *token, FILE *macros_file_handler)
{
  char line[MAX_LINE] = "\0", *macro_name;
  int next_is_macro = 1;
  fseek(macros_file_handler, 0, SEEK_SET);
  while (fgets(line, MAX_LINE, macros_file_handler) != NULL)
  {
    macro_name = strtok(line, " \n");
    if (next_is_macro)
    {
      if (!strcmp(macro_name, token))
        return 1;
      else
        next_is_macro = 0;
    }
    if (!strcmp(macro_name, "endm"))
      next_is_macro = 1;
  }
  return 0;
}

/*
 *Adds macro content to expanded file in the correct location
 */
void insert_macro(FILE *exp_file_handler, FILE *macros_file_handler,
                  char *token)
{
  char line[MAX_LINE] = "\0", file_token[MAX_LINE] = "\0";
  int inserted = 0, insert = 0;
  fseek(macros_file_handler, 0, SEEK_SET);
  while (fgets(line, MAX_LINE, macros_file_handler) != NULL && !inserted)
  {
    get_first_token(line, file_token);
    if (!strcmp(file_token, "endm") && insert)
    {
      insert = 0;
      inserted = 1;
    }
    if (insert)
    {
      fprintf(exp_file_handler, "%s", line);
    }
    if (!strcmp(file_token, token) && !insert)
      insert = 1;
  }
}