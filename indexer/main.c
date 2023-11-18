#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "word.h"
#include "file_operations.h"

int main(int argc, char const *argv[])
{
    FILE *file = getFile(argc, argv);
    printf("Argument: %s\n", argv[1]);

    // --freq N ARQUIVO
    if (0 == strcmp(argv[1], "--freq"))
    {
        printf("FREQ\n");
    }

    // --freq-word
    if (0 == strcmp(argv[1], "--freq-word"))
    {
        printf("FREQ-WORD\n");
        // do something
    }

    // --search
    if (0 == strcmp(argv[1], "--search "))
    {
        printf("SEARCH\n");
        // do something
    }


    int c, count = 0;
    char word[50];
    while ((c = fgetc(file)) != EOF)
    {
        if(!isalpha(c) || count > 48){
            word[count] = '\0';
            if(strlen(word)>2){ // word is valid
                printf("- %s \n", word);
            }
                word[0] = '\0';
                count = 0;
        }else {
            word[count] = tolower(c);
            count++;
        }
    }
    fclose(file);
    printf("End of the program.\n");
    return 0;
}

/*
REF:
https://codereview.stackexchange.com/questions/166085/creating-index-to-a-file-in-c
*/
