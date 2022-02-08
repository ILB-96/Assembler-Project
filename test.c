#include "acutest.h"
#include "assembler.h"

void test_next_word_index(void)
{
    TEST_CHECK_(nextWordIndex("Hello World", 0) == 6, "Second word index of 'Hello World', Expected: %d", 6);
    TEST_CHECK_(nextWordIndex("", 0) == 0, "Second word index of '', Expected: %d", 0);
}
void test_is_valid_macro_name(void)
{
    TEST_CHECK(isValidMacroName("hijmp") == 1);
    TEST_CHECK(isValidMacroName("hithere") == 1);
    TEST_CHECK(isValidMacroName("jmp") == 0);
    TEST_CHECK(isValidMacroName("inc") == 0);
}

TEST_LIST = {
    {"int nextWordIndex(char *line, int index);", test_next_word_index},
    {"int isValidMacroName(char *word);", test_is_valid_macro_name},
    {0}};
