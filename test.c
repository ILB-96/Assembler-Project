#include "acutest.h"
#include "assembler.h"

void test_next_word_index(void)
{
    TEST_CHECK_(nextWordIndex("Hello World", 0) == 6, "Second word index of 'Hello World', Expected: %d", 6);
    TEST_CHECK_(nextWordIndex("", 0) == 0, "Second word index of '', Expected: %d", 0);
}


TEST_LIST = {
    {"int nextWordIndex(char *line, int index);", test_next_word_index},
    {0}};
