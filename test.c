#include "acutest.h"
#include "assembler.h"

void test_next_word_index(void)
{
    int actual = 0;
    TEST_CHECK_((actual = nextWordIndex("Hello World", 0)) == 6, "Second word index of 'Hello World', Expected: %d Actual: %d", 6, actual);
    TEST_CHECK_((actual = nextWordIndex("", 0)) == 0, "Second word index of '', Expected: %d, Actual: %d", 0, actual);
}
void test_is_valid_label_name(void)
{
    int expected = 0;
    int actual = 0;
    TEST_CHECK_((actual = isValidLabel("HelloWorld:")) == (expected = 0), "label name 'HelloWorld:' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = isValidLabel("jmp:")) == (expected = 0), "label name 'jmp:' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = isValidLabel(".bas:")) == (expected = 0), "label name 'jmp:' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = isValidLabel("TRACK:")) == (expected = 1), "label name 'HelloWorld:' Expected: %d Actual: %d", expected, actual);
    TEST_CHECK_((actual = isValidLabel("SQls:")) == (expected = 1), "label name 'HelloWorld:' Expected: %d Actual: %d", expected, actual);
}

TEST_LIST = {
    {"int nextWordIndex(char *line, int index);", test_next_word_index}, {"int isValidLabelName(char *word)", test_is_valid_label_name}, {0}};
