#ifndef TURN42
#define TURN42
#include <stdlib.h>

typedef struct inode {
    void *data;
    struct inode *next;
} inode;

typedef inode *turn;

void puttu(turn *t, void *data);

void *gettu(turn *t);

int sizetu(turn t);

void cleartu(turn *t);
#endif
