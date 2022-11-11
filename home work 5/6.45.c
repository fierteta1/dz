#include "tree45.h"
#include <time.h>

void printtu(turn t);

int count_level_tree(tree tr, int number)
{
    if ((number < 0) || (tr == NULL)){
        return 0;
    }
    int n, last = 1, next = 0;
    turn t = NULL;
    puttu(&t, tr);
    for(n = 0; (q != NULL) && (n < number); ++n){
        tree buf;

        for(; last > 0; --last){
            buf = (tree)gettu(&q);
            if (buf->left != NULL){
                ++next;
                puttu(&t, buf->left);
            }
            if (buf->right != NULL){
                ++next;
                puttu(&t, buf->right);
            }
        }
        if (next == 0){
            break;
        }
        last = next;
        next = 0;
    }
       cleartu(&t);
    return last;
}

void printtu(turn t)
{
    for(; t!= NULL; t = t -> next){
        printf("%d ", ((tree)(t->data))->data);
    }
    printf("\n");
}

void printtr(tree tr)
{
    int last = 1, next = 0;
    turn t = NULL;
    puttu(&t, tr);
    for(;;){
        tree buf;
        for(; last > 0; --last){
            buf = (tree)gettu(&t);
            printf("%d ", buf->data);
            if (buf->left != NULL){
                ++next;
                puttu(&t, buf->left);
            }
            if (buf->right != NULL){
                ++next;
                puttu(&t, buf->right);
            }
        }
        if (next == 0){
            printf("%d ", buf->data);
            break;
        }
        printf("\n");
        last = next;
        next = 0; 
    }
    printf("\n");
    cleartu(&t);
}

void addtr(tree *tr, int value)
{
    if (*tr == NULL){
        *tr = (tree)malloc(sizeof(tnode));
        (*tr)->data = value;
        (*tr)->left = NULL;
        (*tr)->right = NULL;
        return;
    }
    if ((*tr)->data > value){
        addtr(&((*tr)->left), value);
    } else {
        addtr(&((*tr)->right), value);
    }
}

void cleartr(tree *tr)
{
    if (*tr == NULL){
        return;
    }
    cleartr(&((*tr)->left));
    cleartr(&((*tr)->right));
    free(*tr);
}

int main(void)
{
    tree tr = NULL;
    int n;
    scanf("%d", &n);
    printf("\n");
    srand(time(NULL));
    for (; n > 0; --n){
        addtr(&tr, 1+(int)(100.0*rand()/(RAND_MAX+1.0)));
    }
    printtr(tr);
    printf("----------\n");
    n = count_level_tree(tr, 2);
    printf("%d\n", n);
    cleartr(&tr);
    return 0;
}
