#include <stdio.h>
#include <immintrin.h>
#include <unistd.h>
#include <time.h>

void find_time_time();
void find_seed_time();
void find_fail_time();

int main() {
    find_time_time();
    find_seed_time();
    find_fail_time();

    return 0;
}

void find_time_time(){
    struct timespec tp1;
    struct timespec tp2;

    clock_gettime(CLOCK_REALTIME, &tp1);
    clock_gettime(CLOCK_REALTIME, &tp2);

    printf("%10ld.%07ld\n%10ld.%07ld\n", tp1.tv_sec, tp1.tv_nsec, tp2.tv_sec, tp2.tv_nsec);
}

void find_seed_time(){
    unsigned long long p;
    struct timespec tp1;
    struct timespec tp2;

    clock_gettime(CLOCK_REALTIME, &tp1);
    _rdseed64_step(&p);
    clock_gettime(CLOCK_REALTIME, &tp2);

    printf("%10ld.%07ld\n%10ld.%07ld\n", tp1.tv_sec, tp1.tv_nsec, tp2.tv_sec, tp2.tv_nsec);
}

void find_fail_time(){
    int x;
    unsigned long long p;
    struct timespec tp1;
    struct timespec tp2;

    for (x = 0; x < 10000000; x++){
        clock_gettime(CLOCK_REALTIME, &tp1);
        if (_rdseed64_step(&p) == 0){
            clock_gettime(CLOCK_REALTIME, &tp2);
            printf("%10ld.%07ld\n%10ld.%07ld\n", tp1.tv_sec, tp1.tv_nsec, tp2.tv_sec, tp2.tv_nsec);
            break;
        }
    }
}

