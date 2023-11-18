#include <stdio.h>

#include "word.h"

#define RED   1
#define BLACK 0

typedef struct word
{
    char* value;
    int occorrences;
} Word;

typedef struct node
{
    Word *word;
    int color;
    struct node *left;
    struct node *right;
} Node;

int isRed(Node *node)
{
    if(!node) return BLACK;
    return (node->color == RED);
}

int buscar (Node *n, char *v)
{
    if (n == NULL)
    {
        return 0;    /*Nao achou*/
    }
    else if (v < strcmp(n->word->occorrences, v))
    {
        return buscar (n->left, v);
    }
    else if (v > strcmp(n->word->occorrences, v))
    {
        return buscar (n->right, v);
    }
    else
    {
        return 1;    /*Achou*/
    }
}

//void in_order(Node *n)
//{
//    if(!n)
//        return;
//    in_order(n->esq);
//    printf("%s : %d",n->word->value, n->word->occorrences);
//    in_order(n->dir);
//}

//void print(Node *n,int spaces)
//{
//    int i;
//    for(i=0; i<spaces; i++) printf(" ");
//    if(!n)
//    {
//        printf("//\n");
//        return;
//    }
//
//    printf("%s : %d",n->word->value, n->word->occorrences);
//    print(n->esq,spaces+2);
//    print(n->dir,spaces+2);
//}

