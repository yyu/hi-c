#include <stdio.h>

int main(void)
{
        printf("file:%s, base_file:%s, line:%d\n", __FILE__, __BASE_FILE__, __LINE__);
        return 0;
}
