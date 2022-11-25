#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFF_SIZE 256


typedef struct node {
  char *value;
  struct node *next;
} node;

typedef struct {
  node *root;
  node *current;
  unsigned long length;
} word_list;

node* create_node(char *str,size_t str_len, node *next);



void list_append(word_list *l,node *n);
void list_free(word_list *l);
void list_print(word_list *l);
void init_list(word_list *l);
int count_word(word_list *l, char *wrd);
char *get_popular_word(word_list *l);
///return number of passed 0s
size_t mystrncpy(char *dest, char* src, size_t n);


void list_append_spec_seq(word_list *wl, char *w,size_t l);

node * push_min(word_list *wl, int (*comp)(char*a,char*b));

void read_to_list_from_file(word_list *list,FILE *f);
int lexi_comp(char *a,char *b);
void insert_list_sorted(word_list *wl, node *n, int(*comp)(char *, char *));
void insert_lexi(word_list *wl, node *n);

void _insert_sorted_after(node *after, node *n, int(*comp)(char *,char*));

void sort(word_list *wl,int (*comp)(char *a,char *b));



size_t ceil_blocked(size_t i, size_t block_size){
  return i%block_size?i+(block_size-i%block_size):i;
}

node* create_node(char *str,size_t str_len, node *next){
  if(str[0]=='\0')
    return NULL;
  node *n = calloc(sizeof(node),1);
  n->value=calloc(sizeof(char),str_len+1);// with \0
  memmove(n->value,str,str_len);
  n->value[str_len]=0;
  //printf("\n %s  %ld \n ",n->value,strlen(n->value));
  return n;
}


void list_append(word_list *l,node *n){
  if(n==NULL)
    return;
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
node * list_push(word_list *l,node *n){
  if(n==NULL) return NULL;
  node *next = n->next; 
  n->next=l->root;
  l->root=n;
  ++l->length;
  if(!n->next)
    l->current = n;
  return next;
}
void list_free(word_list *l){
  if(l==NULL) return;
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
  printf("\n");
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
///return number of passed 0s
size_t mystrncpy(char *dest, char* src, size_t n){
  size_t zer_numb;
  for(int i =0; i<n;++i){
    if(src[i])
      src[i+zer_numb]=dest[i];
    else 
      ++zer_numb;
  }
  return zer_numb;
}


void list_append_spec_seq(word_list *wl, char *w,size_t l){
  char *real_word = calloc(2,sizeof(char));
  char c = *w;
  for(int i = 0;i<l&&c;++i){
    real_word[0]=c;
    if((c=='&'&&*(w+1)=='&')
        ||(c=='|'&&*(w+1)=='|')
        ||(c=='<'&&*(w+1)=='<')
        ||(c=='>'&&*(w+1)=='>')
        ){
      real_word[1]=c;
      insert_lexi(wl,create_node(real_word,2,NULL));
      ++w;
    } else{
      insert_lexi(wl,create_node(real_word,1,NULL));
    }

      ++w;
      c=*w;
  }
  free(real_word);

}


void read_to_list_from_file(word_list *list,FILE *f){
  //allocate buffer 
  //do read in empty part of buffer
  //  if there is at least 1 word
  //    append words from buffer to list
  //    align uncompleted word in buffer to left
  //    end it with '\0'
  //  else 
  //    allocate bigger buffer
  //
  //
  //while !eof
  char *s = NULL; //buffer
  size_t l = BUFF_SIZE; //actual size of buffer
  bool one_word;
  char *read_here = s; //ptr to empty part of buffer
  char c;
  do{
    s = realloc(s,ceil_blocked(l,BUFF_SIZE));
    l += fread(s,sizeof(char),BUFF_SIZE,f);
     
    c=*s;
    while(c) {
      one_word=true;
      //

    }
  
  }while(!feof(f));
  


}
int lexi_comp(char *a,char *b){
  return strcmp(a, b);
  char ac = *a;
  char bc = *b;

  while(ac&&bc){
    ac=*a;
    bc=*b;
    if(ac>bc)
      return 1;
    if(ac<bc)
      return -1;
    ++a;
    ++b; 
  }
  if(ac>bc)
    return 1;
  if(ac<bc)
    return -1;
  
  return 0;
}
node *pop_min(word_list *wl, int(*comp)(char *a,char*b)){
  if(wl ==NULL || wl->root==NULL) return NULL;
  node *np = wl->root;
  node *npp = np;

  node *nminp = np;
  node *nmin=np;

  while(np){
    if(comp(nmin->value,np->value)>=1) {
      nmin = np;
      nminp = npp;
    }
    npp = np;
    np=np->next;
  }
  if(nmin == wl->root) wl->root=wl->root->next;
  else nminp = nmin->next; 

  return nmin;

}

void insert_list_sorted(word_list *wl, node *n, int(*comp)(char *, char *)){
  ++wl->length;
  if(wl->length==1){
    wl->root=n;
    wl->current=n;
    return;
  }
  if(comp(wl->root->value,n->value)>=1){
    n->next=wl->root;
    wl->root=n;
    return;
  }

  node *np = wl->root;
  node *npp = np;
  do{
    npp = np;
    np=np->next;
  }while(np&&comp(np->value,n->value)<=-1);
  if(!np){
    npp->next = n;
    n->next=NULL;
    wl->current = n;
  } else{
    npp->next = n;
    n->next=np;
  }

}
void insert_lexi(word_list *wl, node *n){
  insert_list_sorted(wl,n,lexi_comp);
}


void sort(word_list *wl,int (*comp)(char *a,char *b)){
  node *p = wl->root;
  word_list *new_wl;
  word_list *to_free;
  node * n;
  while(n=pop_min(wl,comp)){
    list_append(new_wl,n);  
  }

  to_free = wl;
  wl = new_wl;
  free(to_free);

}
  
void  strange_reverse(word_list *wl){
  node *sep_start;
  node *sep_end;
  node *np = wl->root;
  node *npp =np;
  node ** buf = (node**)calloc(32,sizeof(node*));
  int c = 0;
  bool writing = false;
  while(np){
    if(np->value&&np->value[0] == '&'&&np->value[1]==0&&writing!=true){
      sep_start = np;
      writing = true;
      np=np->next;
      continue;
    }

    if(np->value&&np->value[0]=='>'&&np->value[1] ==0&&writing!=false){
      sep_end = np;
      //list_append(&new_wl,np);
      writing =false;
      np=np->next;
      continue;
    }
    if(writing){
      buf[c]=np;
      ++c;
    }
    np=np->next;
  }
  np = sep_start;
  for(--c; c>=0; --c){
    np->next=buf[c];
    np=np->next;
  }
  np->next = sep_end;
  free(buf);

}

