#include <stdio.h>
#include <stdlib.h>

typedef struct t_word
{
    char* value;
    struct t_word* next;
} word;

word* create(char* value)
{
    word* newWord = (word*) malloc(sizeof(word));
    newWord->value = value;
    return newWord;
    return NULL;
}

void put(word** head, char* value)
{
    if(*head == NULL)
    {
        word* newWord = create(value);
        newWord ->next = NULL;
        *head = newWord ;
    }
    else
    {

        word* w = *head;
        while(w->next != NULL)
        {
            w = w->next;
        };

        word* newWord = create(value);
        newWord ->next = NULL;
        w->next = newWord ;

    }
}

void print_list(word* head)
{
    if(head == NULL)
    {
        printf("==============================\n");
        printf("List is empty!\n");
        printf("==============================\n");
    }
    else
    {
        word* w = head;
        printf("==============================\n");
        while(w->next != NULL)
        {
            printf("%s", w->value);
            w = w->next;
        };
        printf("%s", w->value);
        printf("\n");
    }
};

// TODO:
char* get(word* head, word key)
{
    if(head != NULL)
    {
        word* w = head;
        while( w != NULL)
        {
//            if(time_cmp(&c->key, &key) == 0)
            {
//                return c->value;
            }
//            c = c->next;
        }
        return 0;
    }
}


int main()
{
    printf("POC 1 - read file.\n");
    // Need to create file pointer
    FILE *fptr;
    int MAX_TEXT_SIZE = 100;

    word* list = NULL;

    // In C, you can create, open, read, and write to files by declaring a pointer of type FILE, and use the fopen() function:
    fptr = fopen("text4testing.txt", "r");

    char myText[MAX_TEXT_SIZE];

//    while(fgets(myText, MAX_TEXT_SIZE, fptr)/* This function only saves the first line*/){
//        printf("%s", myText);
//    }

    char ch;
    char newWord[MAX_TEXT_SIZE];
    int c = 0;
    while ((ch = fgetc(fptr)) != EOF)
    {
//        printf("ch = %i\n", ch);
        if(ch == 10 || ch == 32)
        {
            c = -1;
            printf("%s\n", newWord);
        }
        else
        {
//            printf("else\n");
            newWord[c] = ch;
            newWord[c+1] = '\n';
        }
        c++;
//        printf("Read = %c - ASCII: %i\n", ch, ch);
        printf("\n");
    }


    fclose(fptr);

    printf("End of the program.\n");
    return 0;
}
