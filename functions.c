#include "assembler.h"

int is_empty_line(char *line)
{
    unsigned int i = 0;
    while (line[i] != '\0')
        if (!isspace(line[i++]))
            return 0;
    return 1;
}

int is_comment_line(char *line)
{
    unsigned int i = 0;
    while (isspace(line[i++]))
        ;
    if (line[--i] == ';')
        return 1;
    return 0;
}
/*******************************************************************************
 * The next functions are used to change and alternate the words and the lines
 * from the file to meet our needs.
 * ****************************************************************************/
void get_first_token(char *line, char *word)
{
    unsigned int i = 0, j = 0;
    while (isspace(line[i]) && line[i] != '\0')
    {
        i++;
    }
    while (!isspace(line[i]) && line[i] != '0')
    {
        word[j++] = line[i++];
    }
    word[j] = '\0';
}

unsigned int get_next_token_index(char *line, int index)
{
    while (isspace(line[index]) && line[index] != '\0')
    {
        index++;
    }
    while (!isspace(line[index]) && line[index] != '\0')
    {
        index++;
    }
    while (isspace(line[index]) && line[index] != '\0')
    {
        index++;
    }
    return index;
}

/*Function that moves the line to the next word and insert the next word inside
 * a variable(word)*/
/*TODO: change the variable name "word" to not be confused with a binary word*/
void get_next_token(char *line, char *word, unsigned int i)
{
    unsigned int j = 0;
    while (line[i] != '\0' && line[i] != '\n')
        line[j++] = line[i++];

    line[j] = '\0';
    j = 0;
    i = 0;
    while (!isspace(line[i]))
        word[j++] = line[i++];
    word[j] = '\0';
}

/*Function that removes the ":" sign from a word*/
void remove_colon(char *word)
{
    unsigned int i = 0;
    while (word[i++] != ':')
        ;
    word[--i] = '\0';
}
