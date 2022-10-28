#include <tdio.h>

int main(void)

{
    int n;
    scanf("%d", &n);
    if ( n < 1){
        printf ("wrong number ");
        return 0;
    }
    n--;
    printf("2 ");
    int p = 3;
    while (n > 0){
        int k;
        for (k = 3; k * k <= p; k += 2){
            if (p % k == 0){
                k = 1;
                break;
            }
        }
        if ( k != 1){
            printf("%d ", p);
            n--;
        }
        p += 2;
    }
    printf("\n");
    return 0;
}
