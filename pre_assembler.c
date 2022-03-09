#include "assembler.h"
#define AS ".as"

void expand_macros(FILE *, FILE *);
int is_macro_name(char *, FILE *);
void insert_macro(FILE *, FILE *, char *);
void get_macro_name(char *, char *);
void process_macro(char *, char *, int *, FILE *);

int pre_assembler(FILE **exp_file_handler, char *file_name)
{
  FILE *file_handler;
  int error = FALSE;
  error = load_file(&file_handler, file_name, ".as", "r");

  if (!error)
  {
    error = load_file(&*exp_file_handler, file_name, "_exp.as", "w+");
    expand_macros(file_handler, *exp_file_handler);
    fclose(file_handler);
    remove("macros_file.txt");
  }
  return error;
}

void expand_macros(FILE *file_handler, FILE *exp_file_handler)
{
  FILE *macros_file_handler;
  char line[MAX_LINE] = "", word[MAX_LINE] = "";
  int is_part_of_macro = 0;
  load_file(&macros_file_handler, "macros_file", ".txt", "w+");

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
}
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