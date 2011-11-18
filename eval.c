#include <stdio.h>

int main(int argc, char* argv[])
{
        {
                int a, b, c, d;
                a = 9;
                b = (c = a);
                printf("a:%d, b:%d, c:%d, d=a:%d\n", a, b, c, d=a);
        }
        {
                int a = 1;
                int b = 2;
                int c = 3;
                a = b = c;
                printf("a:%d, b:%d, c:%d\n", a, b, c);
        }
        return 0;
}
