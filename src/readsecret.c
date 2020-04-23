#include <stdio.h>
#include <unistd.h>
#include <immintrin.h>
#include <time.h>

int read_secret(char * output, int len) {
    // read from the conditioner buffer to see if it's empty

    char a;                 // char to add to output
    unsigned long long p;   // placeholder
    int i, j, k;            // counters
    int x, y;               // temp values
    struct timespec tp;     // time struct for synchronization

    for (i = 0; i < len; i++) {
        a = 0;
        for (j = 0; j < 8; j++) {
            x = y = 0;
            while (0 < 1) {
                // do something every 2 seconds
                clock_gettime(CLOCK_REALTIME, &tp);
                if (tp.tv_sec % 2 == 0) {
                    // wait for the other side to start clearing the buffer
                    usleep(100);
                    // see if we get any errors
                    for (k = 0; k < 4; k++) {
                        y = _rdseed64_step(&p);
                        if (y == 0) {
                            x = 1;
                        }
                    }
                    printf("%d", x);
                    // deserialize the data
                    a |= x;
                    if (j < 7) {
                        a = (a << 1);
                    }
                    x = y = 0;
                    sleep(1);
                    break;
                }
            }
        }
        // add the byte to our array/string
        output[i] = a;
        printf("\n");
    }
    return 0;
}

int main(int argc, char ** argv) {
    char possible_secret[41] = {0};
    read_secret(possible_secret, 40);
    printf("%s\n", possible_secret);
    return 0;
}
