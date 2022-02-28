#ifndef ASSEMBLER_DOT_H
#define ASSEMBLER_DOT_H
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <string.h>
=======
>>>>>>> 4c1522b3209ce145f637e1a320c288e717cd4f7e

#define MAX_LINE 81
#define MAX_ADDRESS 8191
#define BASE_MODE 16
#define BASE_IC 100
#define BASE_DC 0
#define TRUE 1
#define FALSE 0

typedef struct TypeLabel TypeLabel;
extern unsigned int g_error;
extern TypeLabel *symbols_table;

/*Functions from pre-assembler.c*/
int pre_assembler(char *, char *);
/*Functions from first-pass.c*/
int first_pass(FILE *);
void free_symbols_str();
int get_binary_words(char *, char *);
int is_operation_name(char *);
/*Functions from second-pass.c*/
int second_pass(FILE *expanded_file_handler);

int is_comment_line(char *);
int is_empty_line(char *);
void get_first_token(char *, char *);
unsigned int get_next_token_index(char *, int);
void get_next_token(char *line, char *word, unsigned int i);
void remove_colon(char *word);

/*WORD-LIST*/
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
    if (x == NULL)                \
    {                             \
        puts("index not exists"); \
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
    full_two_op,
    two_op,
    min_two_op,
    target_op,
    min_target_op,
    full_target_op,
    no_op
} Valid_operator;

typedef enum
{
    f_mov = 0,
    f_cmp = 0,
    f_add = 10,
    f_sub = 11,
    f_lea = 0,
    f_clr = 10,
    f_not = 11,
    f_inc = 12,
    f_dec = 13,
    f_jmp = 10,
    f_bne = 11,
    f_jsr = 12,
    f_red = 0,
    f_prn = 0

} Funct;

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
    rts = 14,
    stop = 15
} opcode;

typedef enum
{
    immediate,
    direct,
    index_sort,
    register_direct
} sortType;
typedef enum
{
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15
} registers;

void initialize_list(plw *h, plw *p, int);
void add_to_list(plw *prv, int n);
int get_word(plw h, int index);
int add_std_word(plw *, ARE are, Funct funct, registers source_r, sortType source_sort, registers target_r, sortType target_sort);
void print_listNode(plw h);
void print_node(plw p);
void print_word(int word);
void free_list(plw);
int add_base_word(plw *, ARE, opcode);
void add_num_to_list(plw *, ARE, int);
void update_address(plw, int);

/*command_process*/

#define SUM_OPERATIONS 16

<<<<<<< HEAD
int is_only_digits(char *);
=======
<<<<<<< HEAD
int isOnlyDigits(char *);
>>>>>>> 2b8056912a358f759b382029ef4a882e910b22ed
int command_string_process(plw *, char *);
int command_data_process(plw *, char *);
int command_code_process(plw *, char *);
char **split_line(char *);
int add_parameters(plw *, char **, opcode, Funct, Valid_operator);
int set_sort_and_register(char *, registers *, sortType *, ARE *);
int add_word_by_target(plw *, char *, sortType, Valid_operator);
int add_word_by_source(plw *, char *, sortType, Valid_operator);
int get_current_address(plw);
#endif
=======
#endif





/*WORD-LIST*/
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
    if (x == NULL)                \
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
    full_two_op,
    two_op,
    min_two_op,
    target_op,
    min_target_op,
    full_target_op,
    no_op
}Valid_operator;

typedef enum
{
    f_mov = 0,
    f_cmp = 0,
    f_add = 10,
    f_sub = 11,
    f_lea = 0,
    f_clr = 10,
    f_not = 11,
    f_inc = 12,
    f_dec = 13,
    f_jmp = 10,
    f_bne = 11,
    f_jsr = 12,
    f_red = 0,
    f_prn = 0
   
}Funct;

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
    
    rts = 14,
    stop = 15
} opcode;

typedef enum{immediate, direct, index_sort,register_direct}sortType;
typedef enum{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15}registers;


void initialize_list(plw *h, plw *p,int);
void addTo_list(plw *prv, int n);
int getWord(plw h, int index);
int addStd_word(plw *, ARE are, Funct funct, registers source_r, sortType source_sort, registers target_r, sortType target_sort);
void print_listNode(plw h);
void print_node(plw p);
void print_word(int word);
void free_list(plw);
int addBase_word(plw*, ARE, opcode);
void add_numTo_list(plw *,ARE, int);
void update_address(plw, int);


/*command_process*/

#define SUM_OPERATIONS 16


int isOnlyDigits(char *);
int command_string_pross(plw*,char *);
int command_data_pross(plw*,char *);
int command_code_pross(plw*,char *);
char ** split_line(char *);
int add_parameters(plw*,char **, opcode, Funct,Valid_operator);
int setSort_and_register(char *, registers *, sortType *, ARE *);
int add_word_by_target(plw *, char* , sortType, Valid_operator);
int add_word_by_source(plw *, char* , sortType, Valid_operator);
>>>>>>> 4c1522b3209ce145f637e1a320c288e717cd4f7e
