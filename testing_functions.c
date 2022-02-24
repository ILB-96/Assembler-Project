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
int convertWord( int n)
{
    int masc = 15;
    int a = (n>>16)&masc;
    int b = (n>>12)&masc;
    int c = (n>>8)&masc;
    int d = (n>>4)&masc;
    int e = n&masc;

    printf("A%x-B%x-C%x-D%x-E%x/n",a,b,c,d,e);
}

int main(int argc, char const *argv[])
{
    convertWord(263105);
   
    return 0;
}
