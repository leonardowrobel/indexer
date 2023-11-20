#ifndef WORD_H_INCLUDED
#define WORD_H_INCLUDED

typedef struct word {
    char* value;
    int occurrences;
} word;

typedef struct file_search_word {
    char* word;
    int word_occurrences;
    char* file_name;
    unsigned long file_total_words;
    float term_frequency;
    double tfidf;
} file_search_word;

//typedef struct words {
//    word *words_list;
//    int words_length;
//    int words_max_length;
//} words;

#endif // WORD_H_INCLUDED
