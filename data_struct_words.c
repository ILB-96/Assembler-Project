#include "assembler.h"

/*initialize data struct befor use*/
void initialize_list(plw *h, plw *p, int stock)
{
  *h = (plw)malloc(sizeof(wordsNode));
  IS_NULL(*h)
  *p = *h;
  (*p)->word = -1;
  (*p)->stock_index = stock;
}
/*get ARE val and number n and combine them to 'word'*/
int create_stdnum(ARE are, int n)
{
  int mask = 0;
  SET_ARE(are);
  mask = ~mask;
  mask <<= BASE_MODE;
  mask = ~mask;
  return (mask & n) | are;
}
/*get prv of stract and ARE , int parameters to add to the stract*/
void add_num_to_list(plw *prv, ARE are, int n)
{
  int word = create_stdnum(are, n);
  add_to_list(prv, word);
}
/*add simpal number to the stract*/
void add_to_list(plw *prv, int n)
{
  if ((*prv)->word == -1)
  {
    (*prv)->word = n;
    (*prv)->next = NULL;
  }
  else
  {
    plw temp = (plw)malloc(sizeof(wordsNode));
    IS_NULL(temp)
    temp->word = n;
    temp->next = NULL;
    temp->stock_index = (*prv)->stock_index + 1;

    (*prv)->next = temp;
    *prv = temp;
  }
}
/*geting function to word in index*/
int get_word(plw h, int index)
{
  int i;
  IS_NULL(h)
  for (i = 0; i < index; i++, h = h->next)
    IS_NULL(h)

  IS_NULL(h)
  return h->word;
}
/*adding word to the stract in base format ARE and opcode*/
int add_base_word(plw *p, ARE are, opcode o)
{
  int word = 1 << o;
  SET_ARE(are);
  word |= are;

  add_to_list(p, word);
  return word;
}
/*add standart word*/
int add_std_word(plw *prv, ARE are, Funct funct, registers source_r,
                 sortType source_sort, registers target_r,
                 sortType target_sort)
{
  int word = 0;

  SET_TARG_R(target_r);
  SET_SOUR_SORT(source_sort);
  SET_SOUR_R(source_r);
  SET_FUNCT(funct);
  SET_ARE(are);

  word = target_sort | target_r | source_sort | source_r | funct | are;

  add_to_list(prv, word);
  return word;
}
/*convert word to spacial format*/
void convert_word(int n, FILE *file_handler)
{
  int masc = 15;
  int a = (n >> convert_A) & masc;
  int b = (n >> convert_B) & masc;
  int c = (n >> convert_C) & masc;
  int d = (n >> convert_D) & masc;
  int e = n & masc;
  fprintf(file_handler, " A%x-B%x-C%x-D%x-E%x\n", a, b, c, d, e);
}
/*load the converted words to the obj file*/
void load_obj_file(plw h, FILE *file_handler)
{
  while (h != NULL)
  {
    fprintf(file_handler, "%04d", get_current_address(h));
    convert_word(h->word, file_handler);
    h = h->next;
  }
}

/*get the length of the stract*/
int get_length(plw h)
{
  int length = 0;
  while (h != NULL)
  {
    length++;
    h = h->next;
  }
  return length;
}

/*returns the value of the last IC or DC address*/
int get_current_address(plw prv) { return prv->stock_index; }
/*Updates the address section by the num n*/
void update_address(plw head, int n)
{
  if(get_length(head) != 0)
  {
    while (head != NULL)
    {
      head->stock_index = n + 1;
      n++;
      head = head->next;
    }
  }
}
/*Finds the next 0 node and set the ARE and num n*/
int set_next_empty(plw p, ARE are, int num)
{
  int result = -1;
  int std_word = create_stdnum(are, num);
  while (p->word != 0 && p->next != NULL)
    p = p->next;
  if (p != NULL)
  {
    p->word = std_word;
    result = p->stock_index;
  }
  return result;
}

void free_list(plw h)
{
  plw p = h->next;
  while (p != NULL)
  {
    free(h);
    h = p;
    p = h->next;
  }
}
