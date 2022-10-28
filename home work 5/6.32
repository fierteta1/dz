#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data {
    double f;
    char *s[2];
}data;

typedef struct inode {
    data *p;
    struct inode *next;
}inode;

typedef inode *list;
list cplist(list l)
{   
    list l1, l2;
    if (l == NULL){
        l1 = NULL;
        return l1;
    }
    l1 = (list)malloc(sizeof(inode));
    l2 = l1;
    for (; l -> next != NULL; l = l -> next){
        memcpy(l2 -> p, l -> p, sizeof(data));
        l2 -> next = (list)malloc(sizeof(inode));
        l2 = l2 -> next;
    }
    memcpy(l2 -> p, l -> p, sizeof(data));
    l2 -> next = NULL;
    return l1;
}
