#include <stdio.h>

int main(void)

{
    int n;
    double a;
    printf("write number:  ");
    scanf("%lf", &a);
    for (n = 1; a * n >= 1; n++){
 a -= 1 / (double)n;
    }
    printf("n=%d\n", n);
    return 0;
}
