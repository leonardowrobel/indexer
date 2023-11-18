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

int main(int argc, char const *argv[])
{
    FILE *file = getFile(argc, argv);

    // --freq N ARQUIVO
    if (0 == strcmp(argv[1], "--freq"))
    {
//        printf("FREQ\n");
    }

    // --freq-word
    if (0 == strcmp(argv[1], "--freq-word"))
    {
//        printf("FREQ-WORD\n");
        // do something
    }

    // --search
    if (0 == strcmp(argv[1], "--search "))
    {
//        printf("SEARCH\n");
        // do something
    }

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
                printf("Adding %s\n", text_to_add);
                get_word = hashmap_get(map, &(word){.value=new_word});
                if(get_word == NULL)
                {
                hashmap_set(map, &(word){.value=text_to_add, .occorrences=1});
//                    hashmap_set(map, &(word){.value=new_word, .occorrences=1});
//                    // printf("New word: %s\n", new_word);
                }
                else
                {
                    int word_occorrences = get_word->occorrences + 1;
                    hashmap_set(map, &(word){.value=text_to_add, .occorrences= word_occorrences});
//                    // printf("Word: %s, QTD: %d\n", new_word, word_occorrences);
                }
                // printf("- %s \n", word);
//                free(text_to_add);
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
    {
        char *text_to_add = malloc(count+1);
                strcpy(text_to_add,new_word);
                printf("Adding %s\n", text_to_add);
                get_word = hashmap_get(map, &(word){.value=new_word});
                if(get_word == NULL)
                {
                hashmap_set(map, &(word){.value=text_to_add, .occorrences=1});
//                    hashmap_set(map, &(word){.value=new_word, .occorrences=1});
//                    // printf("New word: %s\n", new_word);
                }
                else
                {
                    int word_occorrences = get_word->occorrences + 1;
                    hashmap_set(map, &(word){.value=text_to_add, .occorrences= word_occorrences});
//                    // printf("Word: %s, QTD: %d\n", new_word, word_occorrences);
                }
                // printf("- %s \n", word);
//                free(text_to_add);
                word_count++;
    }
    // TODO: The last word is ignored

    printf("Number of words on hashmap: %lu\n", hashmap_count(map));
    printf("Number of valid words: %lu\n", word_count);
    printf("Find some word....\n");
    char *word_test = "tres";
    get_word = hashmap_get(map, &(word){.value=word_test});
    printf("%s %s\n",word_test ,get_word == NULL ? "Not found" : "found");
    printf("\n");
    size_t iter = 0;
    void *item;
    while (hashmap_iter(map, &iter, &item)) {
        word *iter_word = item;
        printf("- %s :%d\n", iter_word->value, iter_word->occorrences);
    }
    // printf("%s ocorrences=%d\n", get_word->value, get_word->occorrences);

    hashmap_free(map);
    fclose(file);
    printf("End of the program.\n");
    return 0;
}

/*
REF:
https://codereview.stackexchange.com/questions/166085/creating-index-to-a-file-in-c
https://github.com/tidwall/hashmap.c
*/
