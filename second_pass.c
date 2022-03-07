#include "assembler.h"
#define EXTEN_LEN 8
void create_file(FILE **, char *, char *);
int second_pass(FILE *exp_file_handler, char *file_name, plw head_IC) {
  int line_number = 1;
  int error = FALSE;
  int g_error = FALSE;
  int label_base_val = 0;
  int label_offset_val = 0;
  ARE label_are;
  char line[MAX_LINE];
  char word[MAX_LINE];
  FILE *obj_file_handler = NULL;
  FILE *ent_file_handler = NULL;
  FILE *ext_file_handler = NULL;

  create_file(&obj_file_handler, file_name, "_ps.ob");
  create_file(&ent_file_handler, file_name, "_ps.ent");
  create_file(&ext_file_handler, file_name, "_ps.ext");

  while (fgets(line, MAX_LINE, exp_file_handler) != NULL) {
    get_first_token(line, word);
    if (!strcmp(word, ".entry")) {
      get_next_token(line, word);
      error = process_entry_label(word, line_number);
      get_next_token(line, word);
      if (!is_empty_line(line)) {
        error = TRUE;
        fprintf(stderr,
                "Error at line %d: Extended text after entry variable\n",
                line_number);
      }
    } else if (word[strlen(word) - 1] == ':')
      get_next_token(line, word);
    else if (!strcmp(word, ".extern")) {
      get_next_token(line, word);
      get_next_token(line, word);
    }

    if (!is_empty_line(line) && !error && found_label(line, word)) 
    {
      error = get_label_values(word, &label_base_val, &label_offset_val,
                               &label_are);
      printf("at line: %d label: %s  base: %d off: %d are: %c\n", line_number,
             word, label_base_val, label_offset_val, label_are);
      set_next_empty(head_IC,label_are,label_base_val);
      set_next_empty(head_IC,label_are,label_offset_val);

      /*TODO: process ob.ext file
      if (!error &&label_are = 'E') {
        fprintf
      }*/
    }

    if (error) {
      g_error = TRUE;
      error = FALSE;
    }
    line_number++;
  }
  
  print_listNode(head_IC);
  puts("\n");

  print_labels();
  fclose(obj_file_handler);
  fclose(ent_file_handler);
  fclose(ext_file_handler);

  return g_error;
}
int process_entry_label(char *word, int line_number) {
  int error = FALSE;
  if (is_label_exists(word))
    add_entry_attribute(word);
  else {
    error = TRUE;
    fprintf(stderr, "Error at line %d: '%s' is an illegal label name\n",
            line_number, word);
  }
  return error;
}
void create_file(FILE **file_handler, char *name, char *exten) {
  char *file_name;
  if (!(file_name = (char *)malloc(strlen(name) + EXTEN_LEN))) {
    fprintf(stderr, "Error: Out of memory\n");
    exit(EXIT_FAILURE);
  }
  strcpy(file_name, name);
  strcat(file_name, exten);
  if (!(*file_handler = fopen(file_name, "w+"))) {
    free(file_name);
    fprintf(stderr, "FAILED!\nError: File '%s' open failed.\n\n", file_name);
    exit(EXIT_FAILURE);
  }
  free(file_name);
}