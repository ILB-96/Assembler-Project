#include "assembler.h"


int main(int argc, char *argv[]){
    FILE *fptr;
    char *file;
    argv++;
    while(--argc > 0){
        file = process(*argv++);
        firstProcess(file);
        
    }


    return 0;
}



