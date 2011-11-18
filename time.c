#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int main(void)
{
        {
                struct timeval tv1;
                struct timeval tv2;
                gettimeofday(&tv1, NULL);
                sleep(3);
                gettimeofday(&tv2, NULL);
                printf("time cost: %ld\n", ((tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec)));
        }
        {
                time_t thetime;
                time(&thetime);
                printf("ctime: %s.\n", ctime(&thetime));
        }
        {
                time_t now = time(NULL); /* see ``man 3 time'' */
                struct tm* current = localtime(&now);

                printf("localtime: %d-%02d-%02d %02d:%02d:%02d\n",
                       1900 + current->tm_year, current->tm_mon, current->tm_mday,
                       current->tm_hour, current->tm_min, current->tm_sec);
        }

        return 0;
}
