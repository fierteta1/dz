#ifndef TREE6P45
#define TREE6P45
#include <stdio.h>
#include "turn42.h"

typedef struct tnode {
    int data;
    struct tnode *left;
    struct tnode *right;
} tnode;

typedef tnode *tree;

int count_level_tree(tree tr, int n);

void printtr(tree tr);

void addtr(tree *tr, int data);

void cleartr(tree *tr);

#endif
