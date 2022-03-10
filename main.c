#include "assembler.h"
/*"expanded-.as\0" = 13 chars"*/

/*Private functions meant to be used only inside this source*/
void assembler(char *);
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
  plw head_IC; /*need to be free in the end...*/
  plw prv_IC;
  plw head_DC;
  plw prv_DC;

  /*variables to handle the expanded file that we will use*/
  FILE *exp_file_handler;
  printf("Pre Assembly process at work...\n");
  /*go to pre-assembler.c for more info*/
  if (pre_assembler(&exp_file_handler, file_name))
  {
    return;
  }

  fseek(exp_file_handler, 0, SEEK_SET);
  printf("===Assembly process for %s.as started===\n", file_name);
  printf("Assembly First Pass at work...\n");
  /*Go to first-pass.c for more info*/
  if (first_pass(exp_file_handler, &head_IC, &prv_IC, &head_DC, &prv_DC))
  {
    fclose(exp_file_handler);
    free_symbols();
    return;
  }

  fseek(exp_file_handler, 0, SEEK_SET);
  printf("Assembly Second Pass at work...\n");
  /*Go to second-pass.c for more info*/
  if (second_pass(exp_file_handler, file_name, head_IC, head_DC))
  {
    fclose(exp_file_handler);
    free_symbols();
    return;
  }

  printf("===Assembly process for %s.as finished===\n\n", file_name);

  /*Frees every allocated memory and close the file handler*/
  free_symbols();
  fclose(exp_file_handler);
  free_list(head_IC);
  free_list(head_DC);
}
