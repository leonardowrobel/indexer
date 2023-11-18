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

    return theinput;
}
