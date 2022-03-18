#include "assembler.h"
#define AS ".as"

int expand_macros(FILE *, FILE *);
int is_macro_name(char *, FILE *);
void insert_macro(FILE *, FILE *, char *);
void get_macro_name(char *, char *);
void process_macro(char *, char *, int *, FILE *);

/*pre assembler to expand and refactor the original file into a new file
to eas the assembly process*/
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

/*Function to load the macros into the new file
will also remove comment lines, space lines and leading spaces in line*/
int expand_macros(FILE *file_handler, FILE *exp_file_handler)
{
  FILE *macros_file_handler;
  char line[MAX_LINE] = "", word[MAX_LINE] = "";
  int is_part_of_macro = 0;
  if (load_file(&macros_file_handler, "macros_file", ".txt", "w+"))
    return 1;

  while (fgets(line, sizeof(line), file_handler))
  {
    get_first_token(line, word);
    if (!strcmp(word, "macro") || is_part_of_macro)
    {
      process_macro(line, word, &is_part_of_macro, macros_file_handler);
    }
    else
    {
      if (is_macro_name(word, macros_file_handler))
      {
        insert_macro(exp_file_handler, macros_file_handler, word);
      }
      else if (!is_empty_line(line) && !is_comment_line(line))
        fprintf(exp_file_handler, "%s", line);
      fseek(macros_file_handler, 0, SEEK_END);
    }
  }

  fclose(macros_file_handler);
  return 0;
}

/*Function that adds the macro and its content to the macros file*/
void process_macro(char *line, char *word, int *is_part_of_macro,
                   FILE *macros_file_handler)
{
  if (!strcmp(word, "endm"))
  {
    *is_part_of_macro = 0;
    fprintf(macros_file_handler, "%s\n", word);
  }
  else
  {
    if (!strcmp(word, "macro"))
    {
      *is_part_of_macro = 1;
      get_macro_name(line, word);
      fprintf(macros_file_handler, "%s\n", word);
    }
    else if (!is_empty_line(line) && !is_comment_line(line))
      fprintf(macros_file_handler, "%s", line);
  }
}
/*Function that saves the macro name*/
void get_macro_name(char *line, char *word)
{
  int i = 0, j = 0;
  while (isspace(line[i]) && line[i] != '\0')
  {
    i++;
  }
  i = get_next_token_index(line, i);
  while (!isspace(line[i]) && line[i] != '0')
  {
    word[j++] = line[i++];
  }

  word[j] = '\0';
}

/*Function that checks if we found a macro name in a line*/
int is_macro_name(char *word, FILE *macros_file_handler)
{
  char line[MAX_LINE] = "", *macro_name;
  int next_is_macro = 1;
  fseek(macros_file_handler, 0, SEEK_SET);
  while (fgets(line, MAX_LINE, macros_file_handler) != NULL)
  {
    macro_name = strtok(line, " \n");
    if (next_is_macro)
    {
      if (!strcmp(macro_name, word))
        return 1;
      else
        next_is_macro = 0;
    }
    if (!strcmp(macro_name, "endm"))
      next_is_macro = 1;
  }
  return 0;
}

/*Function that adds the macro content to expanded file in the necessary location*/
void insert_macro(FILE *exp_file_handler, FILE *macros_file_handler,
                  char *word)
{
  char line[MAX_LINE] = "", fword[MAX_LINE] = "";
  int inserted = 0, insert = 0;
  fseek(macros_file_handler, 0, SEEK_SET);
  while (fgets(line, MAX_LINE, macros_file_handler) != NULL && !inserted)
  {
    get_first_token(line, fword);
    if (!strcmp(fword, "endm") && insert)
    {
      insert = 0;
      inserted = 1;
    }
    if (insert)
    {
      fprintf(exp_file_handler, "%s", line);
    }
    if (!strcmp(fword, word) && !insert)
      insert = 1;
  }
}