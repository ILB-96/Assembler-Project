#include "acutest.h"
#include "assembler.h"
int is_valid_label_name(char *word);

void test_next_word_index(void)
{
    unsigned int actual = 0;
    TEST_CHECK_((actual = get_next_token_index("Hello World", 0)) == 6,
                "Second word index of 'Hello World', Expected: %d Actual: %d", 6,
                actual);
    TEST_CHECK_((actual = get_next_token_index("", 0)) == 0,
                "Second word index of '', Expected: %d, Actual: %d", 0, actual);
}

void test_is_valid_label_name(void)
{
    unsigned int expected = 0;
    unsigned int actual = 0;
    TEST_CHECK_((actual = is_valid_label_name("HelloWorld")) == (expected = 1),
                "label name 'HelloWorld' Expected: %d Actual: %d", expected,
                actual);
    TEST_CHECK_((actual = is_valid_label_name("jmp")) == (expected = 0),
                "label name 'jmp' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name(".bas")) == (expected = 0),
                "label name '.bas' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name("TRACK")) == (expected = 1),
                "label name 'TRACK' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name("SQls")) == (expected = 1),
                "label name 'SQLs' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name("1Qls")) == (expected = 0),
                "label name '1Qls' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name("#Ql1s")) == (expected = 0),
                "label name '#Ql1s' Expected: %d Actual: %d", expected, actual);
}
/*
void test_count_words(void)
{
    unsigned int expected = 0;
    unsigned int actual = 0;
    char *line;
    char *word;
    line = ".string \"asdf\"";
    firstWord(line, word);
    TEST_CHECK_((actual = count_binarywords(line, "data")) == (expected = 5),
"'%s' Expected: %d Actual: %d", line, expected, actual); line = "add#1,r1";
    firstWord(line, word);
    TEST_CHECK_((actual = count_binary_words(line, "code")) == (expected = 4),
"Number of Words '%s' Expected: %d Actual: %d", line, expected, actual); line =
".data 6,-11"; firstWord(line, word); TEST_CHECK_((actual =
countBinaryWords(line, "data")) == (expected = 4), "Number of Words '%s'
Expected: %d Actual: %d", line, expected, actual);
}*/
void test_command_code_process(void)
{
    unsigned int expected = 0;
    unsigned int actual = 0;
    char *line;
    int line_number = 1;
    plw head_IC;
    plw prv_IC;
    plw head_DC;
    plw prv_DC;
    initialize_list(&head_DC, &prv_DC, 0);
    initialize_list(&head_IC, &prv_IC, BASE_STOCK);

    line = "add r3, LIST";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 0),
                "'%s' inserted Expected: %d Actual: %d", line, expected, actual);
    TEST_CHECK_((actual = get_current_address(prv_IC) - BASE_STOCK) ==
                    (expected = 3),
                "'%s' number of binary words Expected: %d Actual: %d", line,
                expected, actual);
    initialize_list(&head_IC, &prv_IC, BASE_STOCK);

    /*this test wrong because of the command_code_process assumes to get the line without the label
    line = "MAIN : add r3, LIST";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s' inserted Expected: %d Actual: %d", line, expected, actual);
      */
    line = "prn #48";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 0),
                "'%s' inserted Expected: %d Actual: %d", line, expected, actual);
    line = "jsr\tmiami";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 0),
                "'%s' Unknown Error, Expected: %d Actual: %d", line, expected, actual);

    line = "inc r6";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 0),
                "'%s' inserted Expected: %d Actual: %d", line, expected, actual);

    line = "cmp #48,str[r3]";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 0),
                "'%s' inserted Expected: %d Actual: %d", line, expected, actual);
    line = "add array[r2],r1";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 0),
                "'%s' inserted Expected: %d Actual: %d", line, expected, actual);

    line = "cmp #123,array[r1]";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 0),
                "'%s' invalid index Expected: %d Actual: %d", line, expected, actual);

    line = "clr array(5)";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s' invalid character Expected: %d Actual: %d", line, expected, actual);

    line = "add #zero, array ";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s' invalid first operand Expected: %d Actual: %d", line, expected, actual);

    line = "add  #start,array";
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s' invalid first operand Expected: %d Actual: %d", line, expected, actual);

    line = "add mov, LIST"; /*double command*/
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s', double command Expected: %d Actual: %d", line, expected,
                actual);

    line = "prn #!48"; /*illegal number format*/
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s', illegal number format. Expected: %d Actual: %d", line,
                expected, actual);

    line = "prn !#48"; /*illegal number format*/
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s', illegal number format. Expected: %d Actual: %d", line,
                expected, actual);

    line = "add r3 LIST"; /*no comma*/
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s', no comma. Expected: %d Actual: %d", line, expected,
                actual);

    line = "add r14 ,,LIST"; /*double comma*/
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s', double comma. Expected: %d Actual: %d", line, expected,
                actual);

    line = "add ,r17 ,LIST"; /*illegal comma location*/
    TEST_CHECK_((actual = command_code_process(&prv_IC, line, line_number)) ==
                    (expected = 1),
                "'%s', illegal comma location. Expected: %d Actual: %d", line,
                expected, actual);
}
void test_command_data_process(void)
{
    unsigned int expected = 0;
    unsigned int actual = 0;
    char *line;
    int line_number = 1;
    plw head_IC;
    plw prv_IC;
    plw head_DC;
    plw prv_DC;
    initialize_list(&head_DC, &prv_DC, 0);
    initialize_list(&head_IC, &prv_IC, BASE_STOCK);

    line = ".data 6, -9";
    TEST_CHECK_((actual = command_data_process(&prv_DC, line, line_number)) ==
                    (expected = 0),
                "'%s' inserted Expected: %d Actual: %d", line, expected, actual);
    TEST_CHECK_((actual = get_current_address(prv_DC) + 1) == (expected = 2),
                "'%s' number of binary words Expected: %d Actual: %d", line,
                expected, actual);
    initialize_list(&head_DC, &prv_DC, 0);

    line = ".data 66666";
    TEST_CHECK_((actual = command_data_process(&prv_DC, line, line_number)) ==
                    (expected = 1),
                "'%s', data overflow. Expected: %d Actual: %d", line, expected,
                actual);
    line = ".data 6, -9";
    TEST_CHECK_((actual = command_data_process(&prv_DC, line, line_number)) ==
                    (expected = 0),
                "'%s' inserted Expected: %d Actual: %d", line, expected, actual);
    line = ".data 6, -9 ,";
    TEST_CHECK_((actual = command_data_process(&prv_DC, line, line_number)) ==
                    (expected = 1),
                "'%s', illegal comma. inserted Expected: %d Actual: %d", line,
                expected, actual);

    line = ".data 6,, -9";
    TEST_CHECK_((actual = command_data_process(&prv_DC, line, line_number)) ==
                    (expected = 1),
                "'%s', double comma. inserted Expected: %d Actual: %d", line,
                expected, actual);

    line = ".data !6, c-9";
    TEST_CHECK_((actual = command_data_process(&prv_DC, line, line_number)) ==
                    (expected = 1),
                "'%s', illegal number format. inserted Expected: %d Actual: %d",
                line, expected, actual);
    line = ".data 9.3";
    TEST_CHECK_((actual = command_data_process(&prv_DC, line, line_number)) ==
                    (expected = 1),
                "'%s', illegal floating number. inserted Expected: %d Actual: %d",
                line, expected, actual);
}
void test_command_string_process(void)
{
    unsigned int expected = 0;
    unsigned int actual = 0;
    char *line;
    int line_number = 1;
    plw head_IC;
    plw prv_IC;
    plw head_DC;
    plw prv_DC;
    initialize_list(&head_DC, &prv_DC, 0);
    initialize_list(&head_IC, &prv_IC, BASE_STOCK);

    line = ".string \"abcd\"";
    TEST_CHECK_((actual = command_string_process(&prv_DC, line, line_number)) ==
                    (expected = 0),
                "'%s' inserted Expected: %d Actual: %d", line, expected, actual);
    TEST_CHECK_((actual = get_current_address(prv_DC) + 1) == (expected = 5),
                "'%s' number of binary words Expected: %d Actual: %d", line,
                expected, actual);

    line = ".string ,\"abcd\"";
    TEST_CHECK_((actual = command_string_process(&prv_DC, line, line_number)) ==
                    (expected = 1),
                "'%s' illegal comma Expected: %d Actual: %d", line, expected,
                actual);

    line = ".string \"abcd\",  ,";
    TEST_CHECK_((actual = command_string_process(&prv_DC, line, line_number)) ==
                    (expected = 1),
                "'%s' illegal comma Expected: %d Actual: %d", line, expected,
                actual);
}
TEST_LIST = {
    {"int get_next_token_index(char *line, int index);", test_next_word_index},
    {"int is_valid_label_name(char *word)", test_is_valid_label_name},
    {"int command_code_process(plw *prv_IC, char *line, int line_number)",
     test_command_code_process},
    {"int command_data_proces(plw *prv_DC, char *line, int line_number)",
     test_command_data_process},
    {"int command_string_proces(plw *prv_DC, char *line, int line_number)",
     test_command_string_process},
    {0}};