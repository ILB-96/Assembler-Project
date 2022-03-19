#include "assembler.h"
/*
 *Creates a file with a given extension and points a file handler to it
 *@exten char pointer to the name of the extension of the file
 *@name char pointer to the original file name.
 *@mode char pointer to the type of open needed for the file
 *@file_handler pointer to a file pointer that reference to the file.
 *@return 1 if could not command fopen on the file.
 */
int load_file(FILE **file_handler, char *name, char *exten, char *mode)
{
  char *file_name;
  if (!(file_name = (char *)malloc(strlen(name) + MAX_EXTEN_LEN)))
  {
    fprintf(stderr, "Error: Out of memory\nAbort...\n");
    exit(EXIT_FAILURE);
  }
  strcpy(file_name, name);
  strcat(file_name, exten);
  if (!(*file_handler = fopen(file_name, mode)))
  {
    fprintf(stderr, "Error: File '%s' open failed.\n", file_name);
    free(file_name);
    return 1;
  }
  free(file_name);
  return 0;
}
/********************************************************************
 * The next functions check the state of a current line or token.
 * *****************************************************************/

/*
 *Checks if current line is an empty line(can contain spaces)
 *@return 1 if line is
 */
int is_empty_line(char *line)
{
  int i = 0;
  while (line[i] != '\0')
    if (!isspace(line[i++]))
      return 0;
  return 1;
}

/*
*Checks if current line is a comment line
@return 1 if line is a comment line
*/
int is_comment_line(char *line)
{
  int i = 0;
  while (isspace(line[i++]))
    ;
  if (line[--i] == ';')
    return 1;
  return 0;
}

/*
 *Checks if a given token is an operation command
 *@return 1 if the token is an operation command.
 */
int is_operation_name(char *token)
{
  const char *operations_array[] = {"mov", "cmp", "add", "sub", "lea", "clr",
                                    "not", "inc", "dec", "jmp", "bne", "jsr",
                                    "red", "prn", "rts", "stop"};
  int i = 0, found = 0;
  int arrays_length = 16;

  while (i < arrays_length)
    if (!strcmp(token, operations_array[i++]))
      found = 1;

  return found;
}

/*******************************************************************************
 * The next functions are used to change and alternate the tokens and the lines
 * from the file to meet our needs.
 * ****************************************************************************/
/*r
 *Removes leading spaces in line and gets first token in line
 */
void get_first_token(char *line, char *token)
{
  int i = 0, j = 0;
  /*Here we remove leading spaces in line*/
  if (isspace(line[0]))
  {
    while (isspace(line[i]))
      i++;
    while (line[i] != '\0')
      line[j++] = line[i++];
    line[j] = '\0';
    i = 0;
    j = 0;
  }
  /*Here we get the first token from the line*/
  while (!isspace(line[i]) && line[i] != '\0')
    token[j++] = line[i++];

  token[j] = '\0';
}
/*
 *Returns the index of the next token in line
 */
int get_next_token_index(char *line, int index)
{
  while (isspace(line[index]) && line[index] != '\0')
  {
    index++;
  }
  while (!isspace(line[index]) && line[index] != '\0' && line[index] != ',')
  {
    index++;
  }
  if (line[index] == ',')
    index++;
  while (isspace(line[index]) && line[index] != '\0')
  {
    index++;
  }
  return index;
}

/*
 *Moves the line to the next token and gets the next token
 */
void get_next_token(char *line, char *token)
{

  int j = 0;
  int i = get_next_token_index(line, 0);
  while (line[i] != '\0' && line[i] != '\n')
    line[j++] = line[i++];

  line[j] = '\0';
  j = 0;
  i = 0;
  while (!isspace(line[i]))
    token[j++] = line[i++];
  token[j] = '\0';
}

/*
 *Removes the ":" sign from a token
 */
void remove_colon(char *token)
{
  int i = 0;
  while (token[i++] != ':')
    ;
  token[--i] = '\0';
}

/*
 *remove all excessive signs("',','[',']'") from a given token
 *needed to find labels in line
 */
void remove_signs(char *token)
{
  int i = 0;
  while (isalnum(token[i++]))
    ;
  token[--i] = '\0';
}
