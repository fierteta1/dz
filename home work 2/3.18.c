#include <stdio.h>

int main(void)

{
    int count = 0;
    for (int n = 111; n < 1000; ++n){
        if ((n % 10 == n / 100)
           || (n % 10 == (n / 10) % 10)
           || (n / 100 == (n / 10) % 10)){
            count++;
           }
    }
    printf("two equal digits: %d\n", count);
    return 0;
}
