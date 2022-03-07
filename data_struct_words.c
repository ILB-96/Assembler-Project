#include "assembler.h"

void initialize_list(plw *h, plw *p, int stock)
{
    *h = (plw)malloc(sizeof(wordsNode));
    *p = *h;
    (*p)->word = -1;
    (*p)->stock_index = stock;
}
int create_stdnum(ARE are, int n)
{
    int word;
    SET_ARE(are);
    int mask = 0;
    mask = ~mask;
    mask <<= 16;
    mask = ~mask;
    return (mask & n) | are;
}

void add_num_to_list(plw *prv, ARE are, int n)
{
    int word = create_stdnum(are, n);
    add_to_list(prv, word);
}
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
        temp->word = n;
        temp->next = NULL;
        temp->stock_index = (*prv)->stock_index + 1;

        (*prv)->next = temp;
        *prv = temp;
    }
}

int get_word(plw h, int index)
{
    int i;
    IS_NULL(h)
    for (i = 0; i < index; i++, h = h->next)
        IS_NULL(h)

    IS_NULL(h)
    return h->word;
}
int add_base_word(plw *p, ARE are, opcode o)
{
    int word = 1 << o;
    SET_ARE(are);
    word |= are;

    add_to_list(p, word);
    return word;
}
int add_std_word(plw *prv, ARE are, Funct funct, registers source_r, sortType source_sort, registers target_r, sortType target_sort)
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
void print_listNode(plw h)
{
    while (h != NULL)
    {
        print_node(h);
        h = h->next;
    }
}
void print_node(plw p)
{
    printf("\n%d\t", p->stock_index);
    print_word(p->word);
}

void print_word(int word)
{
    int mask = WORD_SIZE;

    while (mask != 0)
    {
        printf("%d", (mask & word) > 0);
        mask >>= 1;
    }
}
int get_current_address(plw prv)
{
    return prv->stock_index;
}
void update_address(plw head, int n)
{
    while (head != NULL)
    {
        head->stock_index = n + 1;
        n++;
        head = head->next;
    }
}
int set_next_empty(plw p, ARE are, int num)
{
    int std_word = create_stdnum(are , num);
    while (p->word != 0 && p->next != NULL) p = p->next;
    if(p != NULL)
    {
        p->word = std_word;
    }
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
