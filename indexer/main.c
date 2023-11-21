#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

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

int cmpfunc (const void * a, const void * b)
{
    word word_a = *(word*)a;
    word word_b = *(word*)b;
    return ( word_a.occurrences - word_b.occurrences);
}

int main(int argc, char const *argv[])
{
    validateArgs(argc, argv);
    FILE *file;

    // HASHMAP
    struct hashmap *map = hashmap_new(sizeof(word), 0, 0, 0, word_hash, word_compare, NULL, NULL);

    unsigned long word_count = 0;
    word *get_word;
    int c, count = 0;
    char new_word[50];
    memset(new_word, '\0', sizeof(new_word));

    int array_length = 1;
    if(0 == strcmp(argv[1], "--freq"))
    {
        array_length = atoi(argv[2]);
    }
    int array_elements_qtd = 0;
    word word_counter[array_length];
    int found = 0;
    size_t word_size = sizeof(word);


    if(0 != strcmp(argv[1], "--search"))
    {
        printf("Processing\n");
        file = getFile(argc, argv);
        while (1)
        {
            c = fgetc(file);
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
                        // WordCounter
                        // printf("New word %s\n", text_to_add);
                        if(array_elements_qtd == 0)
                        {
                            word_counter[array_elements_qtd].occurrences = 1;
                            word_counter[array_elements_qtd].value= text_to_add;
                            // printf("Adding new word on empty list %s %d\n",word_counter[array_elements_qtd].value, word_counter[array_elements_qtd].occurrences);
                            array_elements_qtd++;
                            qsort(word_counter, array_elements_qtd, word_size, cmpfunc);
                        }
                        else if(array_elements_qtd < array_length)
                        {
                            word_counter[array_elements_qtd].occurrences = 1;
                            word_counter[array_elements_qtd].value= text_to_add;
                            // printf("Adding new word on list %s %d\n",word_counter[array_elements_qtd].value, word_counter[array_elements_qtd].occurrences);
                            array_elements_qtd++;
                            qsort(word_counter, array_elements_qtd, word_size, cmpfunc);
                        }
                        else
                        {
                            /*
                            printf("List is full [%d/%d]", array_elements_qtd, array_length);
                            printf("new word %s occours %d times, less than or equal than %s :%d\n",
                            text_to_add, 1, word_counter[0].value, word_counter[0].occurrences);
                            */
                        }
                    }
                    else // Word already exists
                    {
                        int word_occurrences = get_word->occurrences + 1;
                        hashmap_set(map, &(word)
                        {
                            .value=text_to_add, .occurrences= word_occurrences
                        });
                        // WordCounter
                        // printf("Word %s exists\n", text_to_add);
                        // Lista não tá cheia, só busca e substitui, pois apalavra já existe
                        if(array_elements_qtd < array_length)
                        {
                            // printf("List is not full %d/%d\n", array_elements_qtd, array_length);
                            for(int count = 0 ; count < array_elements_qtd ; count++)
                            {
                                if(strcmp(word_counter[count].value, text_to_add) == 0)
                                {
                                    word_counter[count].occurrences = word_occurrences;
                                    // printf("Updating word %s %d in a not full list\n",word_counter[count].value, word_counter[count].occurrences);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            // printf("List is full %d/%d\n", array_elements_qtd, array_length);
                            // Lista tá cheia, verifica se o ultimo elemento tem menos que o atual, se não só ignora
                            // ai pode ser um item que já existe E busca e substitui
                            //, ou pode ser um novo item, ai removemos o ultimo e inserimos no lugar
                            if(word_counter[0].occurrences < word_occurrences)
                            {
                                for(int count = 0 ; count < array_elements_qtd ; count++)
                                {
                                    if(strcmp(word_counter[count].value, text_to_add) == 0)
                                    {
                                        found = count;
                                        break;
                                    }
                                }
                                if(found)
                                {
                                    word_counter[found].occurrences = word_occurrences;
                                    // printf("Updating word %s %d\n in a full list",word_counter[found].value, word_counter[found].occurrences);
                                }
                                else
                                {
                                    word_counter[0].occurrences = word_occurrences;
                                    word_counter[0].value= text_to_add;
                                    // printf("replacing first word %s %d\n in a full list",word_counter[0].value, word_counter[0].occurrences);
                                }
                                found = 0;
                                qsort(word_counter, array_elements_qtd, word_size, cmpfunc);
                            }
                            else
                            {

                            }
                        }
                    }
                }
                word_count++;
                // TODO: Quick sort array
                // Reset word builder
                memset(new_word, '\0', sizeof(new_word));
                count = 0;
            }
            else
            {
                new_word[count] = tolower(c);
                count++;
            }
            if((c) == EOF)
            {
                break;
            }
        };
        fclose(file);
    }

    // ===================================================================================================
    // OPERATIONS
    // --freq N ARQUIVO [WORKING]
    printf("Selecting operation\n");
    if (0 == strcmp(argv[1], "--freq"))
    {
        printf("Printing most frequent words:\n");
        for(int count = 0 ; count < array_elements_qtd ; count++)
        {
            printf("%s QTD: %d\n", word_counter[count].value, word_counter[count].occurrences);
        }
    }
    // --freq-word WORD [WORKING]
    if (0 == strcmp(argv[1], "--freq-word"))
    {
        // Exibe o número de ocorrências de PALAVRA em ARQUIVO.
        char occurring_word[50];
        strcpy(occurring_word, argv[2]);
        tolower(occurring_word);
        // char *occurring_word = tolower(argv[2]);
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
    }
    // --search
    if (0 == strcmp(argv[1], "--search"))
    {
        int files_qtd = argc - 3;
        int files_containing_word = 0;
        file_search_word files_to_search[files_qtd];

        int c, char_count = 0;
        char word_builder[50];
        memset(word_builder, '\0', sizeof(word_builder)); // Reset word builder
        for(int counter = 0; counter < files_qtd ; counter++)
        {
            files_to_search[counter].file_name = argv[counter+3];
            files_to_search[counter].word = argv[2];
            files_to_search[counter].file_total_words = 0;
            files_to_search[counter].word_occurrences = 0;

            FILE *file = fopen(files_to_search[counter].file_name, "r");

            char occurring_word[50];
            strcpy(occurring_word, argv[2]);
            tolower(occurring_word);
            // char *word_to_search = tolower(argv[2]);

            unsigned long freq_word = 0;
            unsigned long total_words = 0;
            while (1)
            {
                c = fgetc(file);
                if(!isalpha(c) || char_count > 47)
                {
                    word_builder[char_count] = '\0';
                    if(strlen(word_builder)>2)  // word is valid
                    {
                        if(strcmp(word_builder, occurring_word) == 0)
                        {
                            freq_word++;
                        }
                    }
                    memset(word_builder, '\0', sizeof(word_builder)); // Reset word builder
                    char_count = 0;
                    total_words++;
                }
                else
                {
                    word_builder[char_count] = tolower(c);
                    char_count++;
                }
                if((c) == EOF)
                {

                    break;
                }
            }
            if(freq_word > 0){
                files_containing_word++;
            }
            files_to_search[counter].file_total_words = total_words;
            files_to_search[counter].word_occurrences = freq_word;
            if(freq_word != 0 && total_words != 0){
                files_to_search[counter].term_frequency = (float)freq_word/(float)total_words;
            } else {
                files_to_search[counter].term_frequency = 0;
            }
            printf("File: %s | %s (%d)", files_to_search[counter].file_name, files_to_search[counter].word, files_to_search[counter].word_occurrences);
            printf("| Tot: %u | TF: %f\n", files_to_search[counter].file_total_words, files_to_search[counter].term_frequency);
            fclose(file);
        }

        // Calculate IDF
        double doc_qtd_doc_qtd_word = (double)files_qtd / (double)files_containing_word;
        printf("doc_qtd_doc_qtd_word: %f\n", doc_qtd_doc_qtd_word);
        double idf = log10(doc_qtd_doc_qtd_word);
        printf("idf: %f\n", idf);

        for(int counter = 0; counter < files_qtd ; counter++)
        {
            files_to_search[counter].tfidf= (double)files_to_search[counter].term_frequency * idf;
            printf("%s | TFIDF: %f\n", files_to_search[counter].file_name, files_to_search[counter].tfidf);
        }
    }

    // For iteration porpouses
    // printf("\nNumber of words on hashmap: %lu\n", hashmap_count(map));
    // size_t iter = 0;
    // void *item;
    // while (hashmap_iter(map, &iter, &item))
    // {
    //     word *iter_word = item;
    //     printf("- %s :%d\n", iter_word->value, iter_word->occurrences);
    // }

    // Clear everything and get the hell out of here!
    hashmap_free(map);
    // fclose(file);
    printf("\nThis is the end!\n");
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
https://www.geeksforgeeks.org/comparator-function-of-qsort-in-c/
https://cplusplus.com/reference/cstdlib/qsort/
https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
https://stackoverflow.com/questions/27284185/how-does-the-compare-function-in-qsort-work
https://stackoverflow.com/questions/11554262/converting-char-to-lower-case-throws-segfault-error
https://www.tutorialspoint.com/c_standard_library/c_function_log10.htm
https://stackoverflow.com/questions/9457225/codeblocks-verbose-build
https://www.geeksforgeeks.org/gcc-command-in-linux-with-examples/
*/
