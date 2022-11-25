//word list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  char *value;
  struct node *next;
} node;

typedef struct {
  node *root;
  node *current;
  unsigned long length;
} word_list;

node* create_node(char *str,size_t str_len, node *next){
  node *n = calloc(sizeof(node),1);
  n->value=calloc(sizeof(char),str_len+1);// with \0
  strcpy(n->value,str);
  return n;
}
void list_append(word_list *l,node *n){
  if(l->root){
    l->current->next = n;
    l->current = n;
    ++l->length;
  } else{
    l->root = n;
    l->current = n;
    ++l->length;
  
  }
}
void list_free(word_list *l){
  node *n = l->root;
  node *n0;
  while(n){
    n0 = n;
    free(n->value);
    n=n->next;
    free(n0);
  }
}
void list_print(word_list *l){
  printf("\nlength: %ld\n",l->length);
  node *n = l->root;
  while(n){
    printf("%s ",n->value);
    n=n->next;
  }
}
void init_list(word_list *l){
  l->length = 0;
  l->current = NULL;
  l->root = NULL;
}
int count_word(word_list *l, char *wrd){
  node *p=l->root;
  int c =0;
  while(p){
    if(!strcmp(wrd,p->value)) 
      ++c;
    p=p->next;
  }
  return c;
}
char *get_popular_word(word_list *l){
  int popular_count=0;
  int buf;
  char *popular_word;
  node *p=l->root;
  node *e=l->current;
  while(p){
    buf = count_word(l,p->value);
    if(popular_count<buf){
      popular_count = buf;
      popular_word = p->value;
    } 
    p=p->next;
  }
  return popular_word;
}

int main(){
  char c = 32;
  char *s = calloc(sizeof(char), 255);
  unsigned long l;
  word_list wl;
  init_list(&wl);
  while( scanf(" %s%ln ",s,&l) !=EOF){

 
    list_append(&wl, create_node(s,l,NULL));

  }
  
  printf("--there are %d word %s\n",count_word(&wl,wl.root->value),wl.root->value);
  char *pw = get_popular_word(&wl);
  printf("--most popular word is %s it occurs %d times",pw,count_word(&wl,pw));
  list_print(&wl);
  list_free(&wl);
  free(s);
}
