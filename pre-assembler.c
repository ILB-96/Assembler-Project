#include "assembler.h"


FILE * process(char *file){
    char as[] =  ".as";
    FILE * fptr;
    FILE *fptr;
    strcat(file, as);
    fptr = fopen(file, "r");
    findMacros(fptr);
}

void findMacros(FILE *fptr){

}