#include <stdio.h>

void strcp(char *str1, char *str2)

{
    while( *str1 != '\0'){
        *str2 = *str1;
        str1++;
        str2++;
    }
    *str2 = '\0';
}
int main(void)
{
    char str1[] = "abc";
    char str2[] = "1234";
    strcp(str1, str2);
    printf("%s\n", str2);
    return 0;
}
