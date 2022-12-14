#include "turn42.h"

void puttu(turn *t, void *value)
{
    turn new = (turn)malloc(sizeof(inode));
    new->data = value;
    new->next = NULL;
    if (*t == NULL){
        *t = new;
        return;
    }
    for (; (*t)->next != NULL; t = &((*t)->next));
    (*t)->next = new;
}

void *gettu(turn *t)
{
    if (*t == NULL){
        return NULL;
    }
    turn fo = *t;
    void *value = fo->data;
    *t = (*t)->next;
    free(fo);
    return value;
}

int sizetu(turn t)
{
    int n = 0;
    for(; t != NULL; t = t->next){
        ++n;
    }
    return n;
}

void cleartu(turn *t)
{
    turn del;
    for(del = *t; del != NULL; del = *t){
        *t = del->next;
        free(del);
    }
}
