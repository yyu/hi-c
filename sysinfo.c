#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <sys/resource.h>

int main(void)
{
	struct rlimit rlim;
	int ret = 0;

	ret = getrlimit(RLIMIT_FSIZE, &rlim);
	assert(ret == 0);
	printf("RLIMIT_FSIZE - cur:%lu, max:%lu\n", rlim.rlim_cur, rlim.rlim_max);

	ret = getrlimit(RLIMIT_CORE, &rlim);
	assert(ret == 0);
	printf("RLIMIT_CORE  - cur:%lu, max:%lu\n", rlim.rlim_cur, rlim.rlim_max);

	rlim.rlim_cur = rlim.rlim_max;
	printf("setting core file max size ...\n");
	ret = setrlimit(RLIMIT_CORE, &rlim);
	assert(ret == 0);

	ret = getrlimit(RLIMIT_CORE, &rlim);
	assert(ret == 0);
	printf("new value:\n");
	printf("RLIMIT_CORE  - cur:%lu, max:%lu\n", rlim.rlim_cur, rlim.rlim_max);

	printf("\n");

	char cpath[PATH_MAX];
	printf("%s\n", getcwd(cpath, PATH_MAX));

        return 0;
}
