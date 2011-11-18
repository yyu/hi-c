#include <stdio.h>

char * reverse_str(char * dst, char * src)
{
	char * d = dst;
        if (*src) {
                d = reverse_str(dst, src + 1);
                *d++ = *src;
        }
	*d = '\0';
	return d;
}

int main(int argc, char* argv[])
{
        {
                char dest[1024];
                char source[] = "";

                reverse_str(dest, source);

                printf("\"%s\" -> \"%s\"\n", source, dest);
        }
        {
                char dest[1024];
                char source[] = "a";

                reverse_str(dest, source);

                printf("\"%s\" -> \"%s\"\n", source, dest);
        }
        {
                char dest[1024];
                char source[] = "ab";

                reverse_str(dest, source);

                printf("\"%s\" -> \"%s\"\n", source, dest);
        }
        {
                char dest[1024];
                char source[] = "abcdefg";

                reverse_str(dest, source);

                printf("\"%s\" -> \"%s\"\n", source, dest);
        }
        return 0;
}
