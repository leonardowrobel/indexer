#include <stdio.h>
#include "word.h"

/* A function to get the file including a input check */
FILE *getFile (int argc, char const *argv[])
{
    FILE *theinput;

    theinput = fopen(argv[3], "r");

    if (!theinput)
    {
        fprintf(stderr, "Can't read %s\n", argv[3]);
        exit(1);
    }

    if(0 == strcmp(argv[1], "--freq")){
        int qtd = atoi(argv[2]);
        if(qtd < 1 || qtd > 256){
            fprintf(stderr, "Quantity '%d' of words is invalid.\n", qtd);
            exit(1);
        }
    }

    return theinput;
}
