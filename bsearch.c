#include <stdio.h>
#include <string.h>

/*
 * insert a number x in A[N]
 * Note:
 *     1. A[0]..A[N-1] is sorted
 *     2. sizeof(A) >= N + 1, such that A[N] does not overflow
 */
void ins(int A[], int N, int x) {
        int l = 0;
        int r = N;
        while (l < r) {
                int m = (l + r) / 2;
                if (A[m] == x) {
                        r = m;
                        break;
                } else if (x < A[m]) {
                        r = m;
                } else {        /* A[m] < x */
                        l = m + 1;
                }
        }
        memmove(A + r + 1, A + r, sizeof(int) * (N - r));
        A[r] = x;
}

void test(int A[], int N, int x) {
        int i;
        for (i = 0; i < N; i++)
                printf("\033[32m%3d\033[0m", A[i]);
        printf("\n");

        ins(A, N, x);

        for (i = 0; i < N + 1; i++)
                printf("\033[31m%3d\033[0m", A[i]);
        printf("\n");
}

int main(int argc, char* argv[]) {
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 0);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 1);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 2);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 3);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 8);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 9);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 10);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 13);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 15);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 16);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 18);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 19);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 25);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 28);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 30);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 40);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 50);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 72);
        }
        {
                int a[11]={1,3,9,9,13,16,19,28,40,72};
                test(a, 10, 80);
        }
        return 0;
}
