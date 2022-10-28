#include <stdio.h>

int main(void)
  
{
    int N, f1 = 1, f2 = 1;
    scanf("%d", &N);
    if( N<1)
    {
      return 0;
    }
    printf("%d ", f1);
    while(N > 1)
    {
      printf("%d ", f2);
      f2 += f1;
      f1 -= f2;
      f1 = -f1;
      N--;
    }
    printf("\n");
    return 0;
}
