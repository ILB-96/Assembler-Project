#include "acutest.h"
#include "assembler.h"

void test_next_word_index(void)
{
    int actual = 0;
    TEST_CHECK_( (actual = nextWordIndex("Hello World", 0)) == 6, "Second word index of 'Hello World', Expected: %d Actual: %d", 6, actual);
    TEST_CHECK_((actual = nextWordIndex("", 0)) == 0, "Second word index of '', Expected: %d, Actual: %d", 0, actual);
}


TEST_LIST = {
    {"int nextWordIndex(char *line, int index);", test_next_word_index},
    {0}};
