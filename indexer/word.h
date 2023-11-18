#ifndef WORD_H_INCLUDED
#define WORD_H_INCLUDED

typedef struct word word;

word* create(char* value);
void put(word** head, char* value);


#endif // WORD_H_INCLUDED
