#define SUM_OPERATIONS 16

int command_code_pross(char *);
char ** split_line(char *);
int add_parameters(plw*,char **, opcode, Funct,Valid_operator);
int setSort_and_register(char *, registers *, sortType *, ARE *);
int add_word_by_target(plw *, char* , sortType, Valid_operator);
int add_word_by_source(plw *, char* , sortType, Valid_operator);
