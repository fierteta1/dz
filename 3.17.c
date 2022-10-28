#include <stdio.h>
#include <math.h>
int main(void)
{
    double a, b, c;
    printf("enter odds (a, b, c):\n");
    scanf("%lf%lf%lf", &a, &b, &c);
    if (a == 0)
    {
        if ( b == 0)
	{
	    if (c == 0)
	    {
	        printf("equation has infinitely many solutions\n");
	    }
	    else
	    {
	        printf("equation has not solution\n");
	    }
	}
	else
	{
           printf("equation has one solution: %.3f\n", -c / b);
	}
        return 0;
    }
 // discriminant    
    double d = b * b - 4 * a * c;
    if (d == 0)
    {
        printf("equation has one solution: %.3f\n", -b / (2 * a));
    }
    else
    {
        if ( d > 0)
        {
	    d = sqrt(d);
            printf("equation has two solution: %.3f %.3f\n", (-b + d) / (2 * a), (-b - d) / (2 * a));
	}
	else
	{
	   d = sqrt(-d);
	   printf("equation has two solution: %.3f + i%.3f %.3f - i%.3f\n", -b / (2 * a), d / (2 * a), -b / (2 * a), d / (2 * a));
	}
        return 0;
    }
}
