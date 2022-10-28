#include <stdio.h>
#include <math.h>

int sumDigit(int n)

{
    int s = 0;
    while (n != 0){
 s += n % 10;
 n /= 10;
    }
    return s;
}
int main(void)
{
    int n;
    printf("vvedite chislo:  ");
    scanf("%d", &n);
    if (pow(n, 2) == pow(sumDigit(n), 3)){
        printf("da \n");
        0 0;
    }
    printf("net \n");
    return 0;
}
