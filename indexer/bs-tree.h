#ifndef BS-TREE_H_INCLUDED
#define BS-TREE_H_INCLUDED

#define RED   1
#define BLACK 0

typedef struct node {
  int occurrences;
  // TODO: pointer to linked list
  int color;
  struct node *left;
  struct node *right;
  struct node *parent;
} node;

int is_red(node * n);

int exists(node *n, int occurrence);

void in_order(node *n);

void print_tree(node *n,int spaces);

node* create_new_node(int occurrence);

node* insert_node(node *root, node *new_node);

void rotate_right(node *n);

void rotate_left(node *n);

void fixup(node *root, node *n);

#endif // BS-TREE_H_INCLUDED
