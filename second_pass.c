#include "assembler.h"

int second_pass(FILE *expanded_file_handler) {
  int line_number = 1;
  int error = FALSE;
  int g_error = FALSE;
  char line[MAX_LINE];
  char word[MAX_LINE];
  while (fgets(line, MAX_LINE, expanded_file_handler) != NULL) {
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
    }
    if (error) {
      g_error = TRUE;
      error = FALSE;
    }
    line_number++;
  }
  print_labels();
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
