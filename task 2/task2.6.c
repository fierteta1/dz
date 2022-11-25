//дерево поиска
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TYPE unsigned long

typedef struct node {
  TYPE value;
  struct node *l_child; //less
  struct node *g_child; //greater
} node;
typedef struct {
  node *root;
  unsigned int height;
  unsigned long size; //number of nodes
} search_tree;

void search_tree_add(search_tree *t,TYPE e){
  node *n = t->root;
  node *n0;
  bool g = false;
  unsigned int _height = 0;
  while(n){
    ++_height;
    TYPE _value = n->value;
    if (e == _value) return;
    else if(e > _value){
      n0 = n;
      n = n->g_child;
      g = true;
    }
    else if(e < _value){
      n0 = n;
      n = n->l_child;
      g = false;
    } 
  }
  n = calloc(sizeof(node),1);
  n->g_child = NULL;
  n->l_child = NULL;
  n->value = e;
  t->size++;
  unsigned int h = t->height;
  ++_height;
  t->height = h>_height?h:_height;
  if(t->size==1) t->root = n;
  else
    if(g) n0->g_child = n;
    else  n0->l_child = n;
  return;
}
bool search_tree_in(search_tree *t, TYPE e){
  node *n = t->root;
  while(n){
    TYPE _value = n->value;
    if (e == _value) return true;
    else if(e > _value) n = n->g_child;
    else if(e < _value) n = n->l_child;
  }
  return false; 
}
void search_tree_rm(search_tree *t,TYPE e){
  node *n = t->root;
  node *n0 = NULL;
  bool g = false;
  unsigned int _height = 0;
  while(n){
    TYPE _value = n->value;
    if (e == _value){
      --t->size;
      if(n->g_child == NULL && n->l_child == NULL) {
        free(n);
        if (n0){ 
          if(g) n0->g_child = NULL;
          else  n0->l_child = NULL; 
        }
        else t->root=NULL;
        return;
      }
      if(n->g_child == NULL) {
        if (n0){ 
          if(g) n0->g_child = n->l_child;
          else  n0->l_child = n->l_child; 
        } else 
          t->root = n->l_child;
        free(n);
        return;
      }
      if(n->l_child == NULL) {
        if (n0){ 
          if(g) n0->g_child = n->g_child;
          else  n0->l_child = n->g_child; 
        } else 
          t->root = n->g_child;
        free(n);
        return;
      }
      
    node *n1 = n->l_child;
    node *n2;
    while (n1->g_child){
      n2 = n1;
      n1 = n1->g_child;
    }
    n2->g_child=n1->l_child;
    if(g) 
      n0->g_child=n1;
    else
      n0->l_child=n1;
   free(n); 
   return;

      // place (multiple g child of l child of *n) value into *n value
      // if multiple g child has own l child
      // place it's value upper
      // delete it(l child of multiple g child of ...)
      // else 
      // delete multiple g child of l child of *n

    }
    else if(e > _value){
      n0 = n;
      n = n->g_child;
      g = true;
    }
    else if(e < _value){
      n0 = n;
      n = n->l_child;
      g = false;
    } 
  }
  return;
}
void pr_sub_search_tree(node *n){
  if(n){
  pr_sub_search_tree(n->l_child);
  printf("%ld", n->value);
  pr_sub_search_tree(n->g_child);
  }
}
void print_search_tree(search_tree *t){
  printf("\ntree    o \n");
  printf("      ooOoo\n");
  printf("     oOO0OOo\n");
  printf("      oO0Oo\n");
  printf("       \\Y/\n");
  printf("        Y\n");
  printf("height: %d | size: %ld\n",t->height,t->size);
  pr_sub_search_tree(t->root);
  printf("\n");
}
void search_tree_free(node *n){
  if(n){
    search_tree_free(n->l_child);
    search_tree_free(n->g_child);
    free(n);
    }

}

int main(){
  search_tree cur_tree;
  cur_tree.size = 0;
  cur_tree.height = 0;
  TYPE a;
  char op;
  while((scanf("\t%c%ld",&op,&a)) != EOF){
    switch(op){
      case '+':
        search_tree_add(&cur_tree,a);
        break;
      case '?':
        printf("\n%ld\n",a);
        if(search_tree_in(&cur_tree,a))
          printf("yes\n");
        else
          printf("no\n");
        break;
      case '-':
        search_tree_rm(&cur_tree,a);
        printf("\ndel %ld done\n",a);
    }
  
  }

  print_search_tree(&cur_tree);

  search_tree_free(cur_tree.root);
  return 0;
}
