#include <stdio.h>

int main(int argc, char* argv[]) {
	char s[] = "hello world";
	char* p = s + 5;
	int i;
	for (i = -5; i <= 5; i++)
		printf("p[%2d] : '%c'\n", i, p[i]);
	return 0;
}
