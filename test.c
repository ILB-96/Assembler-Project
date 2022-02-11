#include "acutest.h"
#include "assembler.h"
int isValidLabelName(char *word);
int nextWordIndex(char *line, int index);

void test_next_word_index(void)
{
    unsigned int actual = 0;
    TEST_CHECK_((actual = nextWordIndex("Hello World", 0)) == 6, "Second word index of 'Hello World', Expected: %d Actual: %d", 6, actual);
    TEST_CHECK_((actual = nextWordIndex("", 0)) == 0, "Second word index of '', Expected: %d, Actual: %d", 0, actual);
}

void test_is_valid_label_name(void)
{
    unsigned int expected = 0;
    unsigned int actual = 0;
    TEST_CHECK_((actual = isValidLabelName("HelloWorld")) == (expected = 1), "label name 'HelloWorld' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = isValidLabelName("jmp")) == (expected = 0), "label name 'jmp' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = isValidLabelName(".bas")) == (expected = 0), "label name '.bas' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = isValidLabelName("TRACK")) == (expected = 1), "label name 'TRACK' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = isValidLabelName("SQls")) == (expected = 1), "label name 'SQLs' Expected: %d Actual: %d", expected, actual);
}

void test_count_words(void)
{
    unsigned int expected = 0;
    unsigned int actual = 0;
    char *line;
    line = "HELL: .string \"asdasdq\"";
    TEST_CHECK_((actual = countWords(line)) == (expected = 2), "Number of Words In '%s' Expected: %d Actual: %d", line, expected, actual);
    line = "HELLO: add#1,r1";
    TEST_CHECK_((actual = countWords(line)) == (expected = 4), "Number of Words '%s' Expected: %d Actual: %d", line, expected, actual);
}

TEST_LIST = {
    {"int nextWordIndex(char *line, int index);", test_next_word_index}, {"int isValidLabelName(char *word)", test_is_valid_label_name}, {"int countWords(char *line);", test_count_words}, {0}};
