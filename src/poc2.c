#include <stdio.h>
#include <unistd.h>
#include <immintrin.h>

int main(void)
{
    int i, j = 0;
    int ret;

    printf("run 1 starting...\n");
    for (i = 0; i < 99; i++) {
        j = 0;
        asm volatile(
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;" : "=r" (ret)
        );
        if (ret == 0) {
            j++;
            printf("fail %d\n", j);
        }
        sleep(2);
    }
    printf("run 2 starting...\n");
    for (i = 0; i < 99; i++) {
        j = 0;
        asm volatile(
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;" : "=r" (ret)
        );
        if (ret == 0) {
            j++;
            printf("fail %d\n", j);
        }
        sleep(2);
    }
    printf("run 3 starting...\n");
    for (i = 0; i < 99; i++) {
        j = 0;
        asm volatile(
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;" : "=r" (ret)
        );
        if (ret == 0) {
            j++;
            printf("fail %d\n", j);
        }
        sleep(2);
    }
    printf("run 4 starting...\n");
    for (i = 0; i < 99; i++) {
        j = 0;
        asm volatile(
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;" : "=r" (ret)
        );
        if (ret == 0) {
            j++;
            printf("fail %d\n", j);
        }
        sleep(2);
    }
    printf("run 5 starting...\n");
    for (i = 0; i < 99; i++) {
        j = 0;
        asm volatile(
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;"
            "rdseed %%rax;" : "=r" (ret)
        );
        if (ret == 0) {
            j++;
            printf("fail %d\n", j);
        }
        sleep(2);
    }
}
