//calculator
#include<stdio.h> 
#include<stdlib.h>
#include<stdbool.h>
#define TYPE int

typedef struct {
  TYPE as[32]; 
  char ops[32]; 
  short act_len;
  bool inited;
} d_queue;


TYPE pre_comp[sizeof(char)];
char pre_comp_len = 0;

TYPE get_pre_comp(char c){
  return pre_comp[c];
}

void init_dq(d_queue *dq, TYPE e){
  dq->ops[0]='f';
  dq->as[0]=e;
  dq->act_len=1;

}
void addTo_dq(d_queue *dq, TYPE e, char op){
  short l = dq->act_len;
  dq->ops[l]=op;
  dq->as[l]=e;
  dq->act_len = l+1;
}

TYPE read_number(char *str, bool to_right){
  char *n=str;
  char nc;
  TYPE result = 0;
  if(to_right){
    ++n;
    nc = *n;
    while((nc<='9' && nc>='0') || nc == '['){
      if (nc == '[')
        return get_pre_comp(*(n+1));
      result*=10;
      result+=nc-'0';
      ++n;
      nc = *n;
    }
  }
  else{
    --n;
    nc = *n;
    char k = 1;
    while((nc<='9' && nc>='0') || nc == ']'){
      if(nc==']')
        return get_pre_comp(*(n-1));
      result+=(nc-'0')*k;
      ++k; 
      --n;
      nc = *n;
    }
  }
  return result;
}
TYPE power(TYPE a, TYPE x){
  while(--x) a*=a;
  return a;  
}

TYPE compute_dq(d_queue *dq){
  TYPE result = 0;
  for(int i = 0; i<dq->act_len;++i){
    printf("---%d_%c\n",result,dq->ops[i]);
    switch(dq->ops[i]){
      case 'f':
        result+=dq->as[i];
        break;
      case '^':
        result = power(dq->as[i],result);
        break;
      case '*':
        result*=dq->as[i];
        break;
      case '+':
        result+=dq->as[i];
        break;
    } 
  }
  return result;

}

TYPE in_paranths_manager(char *s, unsigned int l){
  
  d_queue l_dq;
  bool inited = false;

  for(char *p= s+l-1; p>=s; --p){
    if(*p=='^'){
      if(!inited){
        init_dq(&l_dq,read_number(p,true));
        addTo_dq(&l_dq,read_number(p,false),*p);
        inited = true;
      }  
      else{
       addTo_dq(&l_dq,read_number(p,false),*p);
      }
    }
  }
  for(char *p = s; p<s+l;++p){
    if(*p=='*'){
      if(!inited){
        init_dq(&l_dq,read_number(p,false));
        addTo_dq(&l_dq,read_number(p,true),*p);
        inited = true;
      }  
      else{
       addTo_dq(&l_dq,read_number(p,true),*p);
      }
    }
  }
  for(char *p = s; p<s+l;++p){
    if(*p=='+'){
      if(!inited){
        init_dq(&l_dq,read_number(p,false));
        addTo_dq(&l_dq,read_number(p,true),*p);
        inited = true;
      }  
      else{
       addTo_dq(&l_dq,read_number(p,true),*p);
      }
    }
  }
  printf("--operations order changed\n");
  return compute_dq(&l_dq);
}

int main(){
  size_t buf_size = 1024;
  char *input = calloc(buf_size,sizeof(char));
  unsigned int input_length =  getline(&input,&buf_size,stdin);
  printf("\n%s\n",input);
  printf("--%d\n",in_paranths_manager(input,input_length));


  return EXIT_SUCCESS;
}
