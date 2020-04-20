#include <stdio.h>
#include <unistd.h>
#include <immintrin.h>

int read_secret(char * output, int len) {
    // read from the conditioner buffer to see if it's empty

    char a;                 // char to add to output
    unsigned long long p;   // placeholder
    int i, j, k;            // counters
    int x, y;               // temp values

    for (i = 0; i < len; i++) {
        a = 0;
        for (j = 0; j < 8; j++) {
            x = y = 0;
            for (k = 0; k < 10; k++) {
                y = _rdseed64_step(&p);
                if (y == 0) {
                    x = 1;
                }
            }
            usleep(1000000);
            a = (a | x);
            a = (a << 1);
        }
        output[i] = a;
    }
    return 0;
}

int main(int argc, char ** argv) {
    char possible_secret[41] = {0};
    read_secret(possible_secret, 40);
    printf("%s\n", possible_secret);
    return 0;
}
