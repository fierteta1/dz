#include <stdio.h>
#include <math.h>

int main(void)

{
    double eps, x1=0, x2=1;
    int i =0;
    printf("vvedite chislo:  ");
    scanf("%lf", &eps);
    while (x2 - x1 >= eps){
 i++;
 x1 = x2;
 x2 = pow((1 + 1 / (double)i), i);
    }
    printf("raven priblizitelno ~%.5lf\n", x2);
    return 0;
}
