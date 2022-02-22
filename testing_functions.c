#include "assembler.h"
#define CLEAN_STRING_SPACE(x,i)\
        i = 7; \
        while(x[i] == ' ') i++;\
    

int isOnlyDigits(char *num)
{
    int i;
    int result = 1;
    for (i = 0; num[i] != '\0'; i++)
    {
        if(isdigit(num[i]) == 0) result = 0;
        if(i == 0 && (num[i] == '+' || num[i] == '-') ) result = 1;
    }
    return result;
}

char * strPross(char *s)
{
    int i;
    CLEAN_STRING_SPACE(s,i);

    while(s[i] != '\0')
    {
        printf("%c\n",s[i++]);
    }
    return s+i;
}

int main(int argc, char const *argv[])
{
    char str[] = ".string    \"abfff  cd\"  ";
    puts(strPross(str));
    
   
    return 0;
}
