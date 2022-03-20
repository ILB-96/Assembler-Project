#include "assembler.h"
/*"expanded-.as\0" = 13 chars"*/

/*Private functions meant to be used only inside this source*/
void assembler(char *);
void compfiles();
int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    printf("Error: Not enough arguments, please insert files names.\n");
    return 1;
  }
  while (--argc > 0)
  {
    assembler(*++argv); /*assembler process for each file starts here*/
  }
  return 0;
}

/*Primary Function to process all the steps of the assembler*/
void assembler(char *file_name)
{
  plw head_IC;
  plw prv_IC;
  plw head_DC;
  plw prv_DC;
  TypeLabel *symbols_table;
  /*variables to handle the expanded file that we will use*/
  FILE *exp_file_handler;
  printf("===Assembly process for %s.as started===\n", file_name);
  printf("Pre Assembly process at work...\n");
  /*go to pre-assembler.c for more info*/
  if (pre_assembler(&exp_file_handler, file_name))
  {
    puts("Abort...\n");
    return;
  }

  fseek(exp_file_handler, 0, SEEK_SET);

  printf("Assembly First Pass at work...\n");
  /*Go to first-pass.c for more info*/
  if (first_pass(exp_file_handler, &head_IC, &prv_IC, &head_DC, &prv_DC, &symbols_table))
  {
    puts("Abort...\n");
    fclose(exp_file_handler);
    free_list(head_IC);
    free_list(head_DC);
    free_symbols(symbols_table);
    return;
  }

  fseek(exp_file_handler, 0, SEEK_SET);

  printf("Assembly Second Pass at work...\n");
  /*Go to second-pass.c for more info*/
  if (second_pass(exp_file_handler, file_name, head_IC, head_DC, symbols_table))
  {
    puts("Abort...\n");
    fclose(exp_file_handler);
    free_list(head_IC);
    free_list(head_DC);
    free_symbols(symbols_table);
    return;
  }

  printf("===Assembly process for %s.as finished===\n\n", file_name);

  /*Frees every allocated memory and close the file handler*/
  free_symbols(symbols_table);
  fclose(exp_file_handler);
  free_list(head_IC);
  free_list(head_DC);
}

void compfiles()
{
  char line1[MAX_LINE] = "\0";
  char line2[MAX_LINE] = "\0";
  char token1[MAX_LINE] = "\0";
  char token2[MAX_LINE] = "\0";
  int line_num = 1;
  FILE *before = fopen("beforemacro.ob", "r");
  FILE *after = fopen("inpuTest.ob", "r");
  while (fgets(line1, MAX_LINE, before))
  {
    if (fgets(line2, MAX_LINE, after))
    {
      get_first_token(line1, token1);
      get_first_token(line2, token2);
      if (strcmp(token1, token2) != 0)
        printf("not matching at line %d\n", line_num);
      else
      {
        get_next_token(line1, token1);
        get_next_token(line2, token2);
        if (strcmp(token1, token2) != 0)
          printf("not matching at line %d\n", line_num);
      }
    }
    line_num++;
  }
}