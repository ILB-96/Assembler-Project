#include "assembler.h"

int main(int argc, char *argv[])
{
    /* FILE *fptr;
     char *file;*/
    argv++;
    while (--argc > 0)
    {
        process(*argv++);
    }
    return 0;
}