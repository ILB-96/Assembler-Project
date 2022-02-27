#include "acutest.h"
#include "assembler.h"
int is_valid_label_name(char *word);

void test_next_word_index(void) {
    unsigned int actual = 0;
    TEST_CHECK_((actual = get_next_token_index("Hello World", 0)) == 6, "Second word index of 'Hello World', Expected: %d Actual: %d", 6, actual);
    TEST_CHECK_((actual = get_next_token_index("", 0)) == 0, "Second word index of '', Expected: %d, Actual: %d", 0, actual);
}

void test_is_valid_label_name(void) {
    unsigned int expected = 0;
    unsigned int actual = 0;
    TEST_CHECK_((actual = is_valid_label_name("HelloWorld")) == (expected = 1), "label name 'HelloWorld' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name("jmp")) == (expected = 0), "label name 'jmp' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name(".bas")) == (expected = 0), "label name '.bas' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name("TRACK")) == (expected = 1), "label name 'TRACK' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name("SQls")) == (expected = 1), "label name 'SQLs' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name("1Qls")) == (expected = 0), "label name '1Qls' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = is_valid_label_name("#Ql1s")) == (expected = 0), "label name '#Ql1s' Expected: %d Actual: %d", expected, actual);
}

void test_count_words(void) {
    unsigned int expected = 0;
    unsigned int actual = 0;
    char *line;
    char *word;
    line = ".string \"asdf\"";
    firstWord(line, word);
    TEST_CHECK_((actual = count_binarywords(line, "data")) == (expected = 5), "Number of Words In '%s' Expected: %d Actual: %d", line, expected, actual);
    line = "add#1,r1";
    firstWord(line, word);
    TEST_CHECK_((actual = count_binary_words(line, "code")) == (expected = 4), "Number of Words '%s' Expected: %d Actual: %d", line, expected, actual);
    line = ".data 6,-11";
    firstWord(line, word);
    TEST_CHECK_((actual = countBinaryWords(line, "data")) == (expected = 4), "Number of Words '%s' Expected: %d Actual: %d", line, expected, actual);
}

TEST_LIST = {
    {"int get_next_token_index(char *line, int index);", test_next_word_index}, {"int is_valid_label_name(char *word)", test_is_valid_label_name}, {"int countBinaryWords(char *line, char *word);", test_count_words}, {0}};
