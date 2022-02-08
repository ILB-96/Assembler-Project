#include "assembler.h"

typedef struct link_words *plw;
typedef struct link_words
{
    plw next;
    unsigned int word;
    int stock_index;
} wordsNode;

#define IS_NULL(x)                \
    if (h == NULL)                \
    {                             \
        puts("index not exsist"); \
        exit(0);                  \
    }

#define WORD_SIZE 1 << 19
#define BASE_STOCK 100

#define SET_TARG_R(x) x = x << 2
#define SET_SOUR_SORT(x) x = x << 6
#define SET_SOUR_R(x) x = x << 8
#define SET_FUNCT(x) x = x << 12
#define SET_ARE(x) x = x << 16
typedef enum
{
    A = 4,
    R = 2,
    E = 1
} ARE;

typedef enum
{
    o_mov = 0,
    o_cmp = 1,
    o_add = 2,
    o_sub = 2,
    o_lea = 4,
    o_clr = 5,
    o_not = 5,
    o_inc = 5,
    o_dec = 5,
    o_jmp = 9,
    o_bne = 9,
    o_jsr = 9,
    o_red = 12,
    o_prn = 13,
    o_rts = 14,
    stop = 15
} opcode;

void initialize_list(plw *h, plw *p);
void addTo_list(plw *prv, int n);
int getWord(plw h, int index);
int addStd_word(plw *, ARE are, int funct, int source_r, int source_sort, int target_r, int target_sort);
void print_listNode(plw h);
void print_node(plw p);
void print_word(int word);

/*________________code________________*/

void initialize_list(plw *h, plw *p)
{
    *h = (plw)malloc(sizeof(wordsNode));
    *p = *h;
    (*p)->word = -1;
    (*p)->stock_index = BASE_STOCK - 1;
}

void addTo_list(plw *prv, int n)
{
    if ((*prv)->word == -1)
    {
        (*prv)->word = n;
        (*prv)->next = NULL;
        (*prv)->stock_index = BASE_STOCK;
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

int getWord(plw h, int index)
{
    int i;
    IS_NULL(h)
    for (i = 0; i < index; i++, h = h->next)
        IS_NULL(h)

    IS_NULL(h)
    return h->word;
}
int addBase_word(plw *p, ARE are, opcode o)
{
    int word = 1 << o;
    SET_ARE(are);
    word |= are;

    addTo_list(p, word);
    return word;
}
int addStd_word(plw *prv, ARE are, int funct, int source_r, int source_sort, int target_r, int target_sort)
{
    int word = 0;

    SET_TARG_R(target_r);
    SET_SOUR_SORT(source_sort);
    SET_SOUR_R(source_r);
    SET_FUNCT(funct);
    SET_ARE(are);

    word = target_sort | target_r | source_sort | source_r | funct | are;

    addTo_list(prv, word);
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

int main()
{

    plw head;
    plw prv;
    initialize_list(&head, &prv);

    addBase_word(&prv, A, o_add);
    addStd_word(&prv, A, 11, 1, 3, 4, 3);

    print_listNode(head);
}