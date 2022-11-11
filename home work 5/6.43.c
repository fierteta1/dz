#include "stack43.h"
void pushst(stack *st, void *value)
{
    stack new = (stack)malloc(sizeof(inode));
    new->data = value;
    new->next = *st;
    *st = new;
}
void *popst(stack *st)
{
    if (*st == NULL){
        return NULL;
    }
    stack first = *st;
    void *value = first->data;
    *st = (*st)->next;
    free(first);
    return value;
}
void clearst(stack *st)
{
    stack buf = *st;
    *st = (*st)->next;
    for (; *st != NULL; *st = (*st)->next){
        free(buf);
        buf = *st;
    }
}
