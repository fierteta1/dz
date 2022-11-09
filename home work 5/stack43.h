#ifndef STACK6P43
#define STACK6P43
#include <stdlib.h>

typedef struct inode {
    void *data;
    struct inode *next;
} inode;

typedef inode *stack;

void pushst(stack *st, void *data);

void *popst(stack *st);

void clearst(stack *st);

#endif
