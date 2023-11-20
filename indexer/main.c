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

void insert(word* words, word new_word,int max_length){
    int i, k;

    int words_length = sizeof(words) / sizeof(word);
    if(words_length == 0){
        (*words)[words_length++] = new_word;
    } else if(words_length < max_length){//need this check
        //x->length = x->length + 1;//Do not increment before the loop. because x->numbers[i] occurs out of bounds
        int words_i_occurrence;
        for(i = 0; i < words_length; i++){   //descending order
            words_i_occurrence = (*words)[i]->occurrences;
            if(*((*words)[i])->occurrences < new_word->occurrences){//descending order
                //insert position is i
                for(k = words_length-1; k >= i; k--){//k > i --> k >= i
                    (*words)[k+1] = (*words)[k];
                }
                break;//exit loop
            }
        }
        (*words)[i] = new_word;
        words_length++;
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
    int array_length = atoi(argv[2]);
    word* words[array_length];

    printf("Processing\n");
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
                if(get_word == NULL) // NEW WORD
                {
                    hashmap_set(map, &(word)
                    {
                        .value=text_to_add, .occurrences=1
                    });

//                    node* this_node = get_node(root, 1);
//                    if(this_node == NULL){
//                        node *new_node = create_new_node(1);
//                        push_new_item(new_node, text_to_add);
//                        root = insert_node(root, new_node);
//                    /*
//                        fixup(root, new_node);
//                        root->color = BLACK;
//                    */
//                    } else {
//                        push_new_item(this_node->words, create_new_item(text_to_add));
//                    }
                }
                else // Word already exists
                {
                    int word_occurrences = get_word->occurrences + 1;
                    hashmap_set(map, &(word)
                    {
                        .value=text_to_add, .occurrences= word_occurrences
                    });

                    // TREE part
                    /*
                    // remove from old node
                    node* old_node = get_node(root, get_word->occurrences);
                    delete_item_by_key(old_node, text_to_add);

                    node* new_node = get_node(root, word_occurrences);
                    if(new_node == NULL)
                    {
                        new_node = create_new_node(word_occurrences);
                        push_new_item(new_node->words, create_new_item(text_to_add));
                        root = insert_node(root, new_node);
                        fixup(root, new_node);
                        root->color = BLACK;
                    }
                    else {
                        push_new_item(new_node->words, create_new_item(text_to_add));
                    }
                    */
                }
                word_count++;
            }
            // Reset word builder
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
        // printf("Adding %s\n", text_to_add);
        get_word = hashmap_get(map, &(word)
        {
            .value=new_word
        });
        if(get_word == NULL) // New word
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
//            if(!exists(root, word_occurrences))
//                    {
//                        node *new_node = create_new_node(word_occurrences);
//                        root = insert_node(root, new_node);
//                        fixup(root, new_node);
//                        root->color = BLACK;
//                    }
            word_count++;
        }
    };





    printf("RB BS-Tree\n");
    // RB BS-Tree
    // Testing
//    int n = 11;
//    int a[11] = { 7, 6, 5, 4, 3, 0, 7 , 8, 11, 45, 12};
//
//    for (int i = 0; i < n; i++)
//    {
//        node *new_node = create_new_node(a[i]);
//        // printf("-%d \n", new_node->occurrences);
//        root = insert_node(root, new_node);
//        fixup(root, new_node);
//        root->color = BLACK;
//    }

    // Testing
    int test_find = 3;
    node *node_to_find = search_occurrence(root, test_find);
    printf("%d was %s\n", test_find, node_to_find == NULL ? "not found" : "found");
    in_order(root);

    // ===================================================================================================

    // OPERATIONS
    // --freq N ARQUIVO
    printf("\n");
    if (0 == strcmp(argv[1], "--freq"))
    {






//        printf("FREQ\n");

    }
    // --freq-word [WORKING]
    if (0 == strcmp(argv[1], "--freq-word"))
    {
        // Exibe o número de ocorrências de PALAVRA em ARQUIVO.
        char *occurring_word = tolower(argv[2]);
        get_word = hashmap_get(map, &(word)
        {
            .value=occurring_word
        });
        if(get_word == NULL)
        {
            printf("There are no occurrences for word \"%s\" in this file!\n", occurring_word);
        }
        else
        {
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

    // For iteration porpouses
//    printf("\nNumber of words on hashmap: %lu\n", hashmap_count(map));
//    size_t iter = 0;
//    void *item;
//    while (hashmap_iter(map, &iter, &item))
//    {
//        word *iter_word = item;
//        printf("- %s :%d\n", iter_word->value, iter_word->occurrences);
//    }

    // ===================================================================================================
    // Clear everything and get the hell out of here!
    deallocate(root);
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
