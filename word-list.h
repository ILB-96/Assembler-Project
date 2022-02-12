typedef enum
{
    A = 4,
    R = 2,
    E = 1
} ARE;

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