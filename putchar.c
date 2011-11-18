#include <stdio.h>

int main(int argc, char* argv[])
{
        int i;

        {
                for (i = 0; i < 80; i++)
                        putchar('-');
                putchar('\n');

                printf("123456789");

                for (i = 0; i < 3; i++)
                        putchar('\b');

                for (i = 0; i < 2; i++)
                        printf("\b");

                printf("ABC");

                putchar('\n');
        }
        {
                for (i = 0; i < 80; i++)
                        putchar('-');
                putchar('\n');

                printf("123456789");

                for (i = 0; i < 3; i++)
                        putchar('\b');

                for (i = 0; i < 2; i++)
                        printf("\b");

                printf("ABCDEFG");

                putchar('\n');
        }
        {
                for (i = 0; i < 80; i++)
                        putchar('-');
                putchar('\n');

                printf("123456789\n");

                putchar('\b');

                printf("\b");

                printf("ABC");

                putchar('\n');
        }

        return 0;
}
