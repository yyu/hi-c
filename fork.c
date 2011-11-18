#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>

static const char* filename = "/tmp/forktest.log";
static void writelog(const char* s) {
        static FILE* fp = NULL;

        if (!fp)
                fp = fopen(filename, "a");
        assert(fp);

        time_t now = time(NULL); /* see ``man 3 time'' */
        struct tm* current = localtime(&now);
        fprintf(fp, "[%d-%02d-%02d %02d:%02d:%02d] %s\n",
                1900 + current->tm_year, current->tm_mon, current->tm_mday,
                current->tm_hour, current->tm_min, current->tm_sec,
                s);
        fflush(fp);
}

int main(void)
{
        writelog("--------------------");

        /* process ID and Session ID */
        pid_t pid, sid;

        pid = fork();
        assert(pid >= 0);

        /* If we got a good PID, then we can exit the parent process. */
        if (pid > 0) {
                fprintf(stderr, "child pid: %d\n", pid);
                writelog("Parent exits.");
                exit(EXIT_SUCCESS);
        }

        /* Create a new SID for the child process */
        sid = setsid();
        assert(sid != -1);
        fprintf(stderr, "new sid: %d\n", sid);
        writelog("new SID created");

        writelog("done");

        /* The Big Loop */
        int i = 100;
        while (--i) {
                writelog("inside loop");
                sleep(2); /* wait 2 seconds */
        }

        exit(EXIT_SUCCESS);
}
