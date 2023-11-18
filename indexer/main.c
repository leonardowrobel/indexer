#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "hashmap.h"

#include "word.h"
#include "file_operations.h"

int word_compare(const void *a, const void *b, void *udata)
{
    const word *ua = a;
    const word *ub = b;
    return strcmp(ua->value, ub->value);
}

bool word_iter(const void *item, void *udata)
{
    const word *word = item;
    return true;
}

uint64_t word_hash(const void *item, uint64_t seed0, uint64_t seed1)
{
    const word *word = item;
    return hashmap_sip(word->value, strlen(word->value), seed0, seed1);
}

void validateArgs(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "There must be at least three arguments!\n");
        exit(1);
    }
    if(0 != strcmp(argv[1], "--freq") && 0 != strcmp(argv[1], "--freq-word") && 0 != strcmp(argv[1], "--search"))
    {
        fprintf(stderr, "No valid option specified!\n");
        exit(1);
    }
}

int main(int argc, char const *argv[])
{
    validateArgs(argc, argv);

    FILE *file = getFile(argc, argv);

    // HASHMAP
    struct hashmap *map = hashmap_new(sizeof(word), 0, 0, 0, word_hash, word_compare, NULL, NULL);

    unsigned long word_count = 0;
    word *get_word;
    int c, count = 0;
    char new_word[50];
    memset(new_word, '\0', sizeof(new_word));

    while ((c = fgetc(file)) != EOF)
    {
        if(!isalpha(c) || count > 47)
        {
            new_word[count] = '\0';
            if(strlen(new_word)>2)  // word is valid
            {
                char *text_to_add = malloc(count+1);
                strcpy(text_to_add,new_word);
                // printf("Adding %s\n", text_to_add);
                get_word = hashmap_get(map, &(word)
                {
                    .value=new_word
                });
                if(get_word == NULL)
                {
                    hashmap_set(map, &(word)
                    {
                        .value=text_to_add, .occurrences=1
                    });
                }
                else
                {
                    int word_occurrences = get_word->occurrences + 1;
                    hashmap_set(map, &(word)
                    {
                        .value=text_to_add, .occurrences= word_occurrences
                    });
                }
                word_count++;
            }
            // Zera o montador de palavrar
            memset(new_word, '\0', sizeof(new_word));
            count = 0;
        }
        else
        {
            new_word[count] = tolower(c);
            count++;
        }
    };
    // The last word is ignored
    new_word[count] = '\0';
    if(strlen(new_word)>2)
    {
        char *text_to_add = malloc(count+1);
        strcpy(text_to_add,new_word);
        printf("Adding %s\n", text_to_add);
        get_word = hashmap_get(map, &(word)
        {
            .value=new_word
        });
        if(get_word == NULL)
        {
            hashmap_set(map, &(word)
            {
                .value=text_to_add, .occurrences=1
            });
//                    hashmap_set(map, &(word){.value=new_word, .occurrences=1});
//                    // printf("New word: %s\n", new_word);
        }
        else
        {
            int word_occurrences = get_word->occurrences + 1;
            hashmap_set(map, &(word)
            {
                .value=text_to_add, .occurrences= word_occurrences
            });
//                    // printf("Word: %s, QTD: %d\n", new_word, word_occorrences);
        }
        // printf("- %s \n", word);
//                free(text_to_add);
        word_count++;
    }

    // --freq N ARQUIVO
        printf("\n");
    if (0 == strcmp(argv[1], "--freq"))
    {
//        printf("FREQ\n");
    }

    // --freq-word
    if (0 == strcmp(argv[1], "--freq-word"))
    {
        // Exibe o número de ocorrências de PALAVRA em ARQUIVO.
        char *occurring_word = tolower(argv[2]);
        get_word = hashmap_get(map, &(word)
        {
            .value=occurring_word
        });
        if(get_word == NULL){
            printf("There are no occurrences for word \"%s\" in this file!\n", occurring_word);
        } else {
            printf("The word \"%s\" appears %d times in this file.\n", get_word->value, get_word->occurrences);
        }
        // do something
    }

    // --search
    if (0 == strcmp(argv[1], "--search "))
    {
//        printf("SEARCH\n");
        // do something
    }
        printf("\n");

    printf("Number of words on hashmap: %lu\n", hashmap_count(map));
    printf("\n");

    // For iteration porpouses
//    size_t iter = 0;
//    void *item;
//    while (hashmap_iter(map, &iter, &item))
//    {
//        word *iter_word = item;
//        printf("- %s :%d\n", iter_word->value, iter_word->occurrences);
//    }

    // Clear everything and get the hell out of here!
    hashmap_free(map);
    fclose(file);
    printf("This is the end!\n");
    return 0;
}

/*
REF:
https://codereview.stackexchange.com/questions/166085/creating-index-to-a-file-in-c
https://github.com/tidwall/hashmap.c
https://stackoverflow.com/questions/28087519/how-to-copy-from-character-array-to-character-pointer
https://stackoverflow.com/questions/38561/what-is-the-conversion-specifier-for-printf-that-formats-a-long
https://www.learnc.net/c-tutorial/c-read-text-file/
https://www.educative.io/answers/resolving-the-dereferencing-pointer-to-incomplete-type-error
https://stackoverflow.com/questions/2700646/dereferencing-pointer-to-incomplete-type
https://stackoverflow.com/questions/20120833/how-do-i-refer-to-a-typedef-in-a-header-file
https://stackoverflow.com/questions/5547131/c-question-const-void-vs-void
*/
