#include <stdio.h>
#include "word.h"

/* A function to get the file including a input check */
FILE *getFile (int argc, char const *argv[])
{
    FILE *theinput;

    if (argc < 3)
    {
        fprintf(stderr, "There must be at least three arguments!\n");
        exit(1);
    }

    theinput = fopen(argv[3], "r");

    if (!theinput)
    {
        fprintf(stderr, "Can't read %s\n", argv[3]);
        exit(1);
    }

    return theinput;
}
