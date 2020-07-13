#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <immintrin.h>

int stopno = 0;

char byte = 0;
int iter = 0;

int write_pid(char * path);
void read_from_conditioner();
void read_from_conditioner_new();
void abort_process();

int main() {
    signal(SIGUSR1, read_from_conditioner_new);
    signal(SIGINT, abort_process);

    char * pidpath = "/tmp/.recv.pid";
    write_pid(pidpath);

    while (stopno != 1) {
        pause();
    }

    remove(pidpath);

    return(0);
}

int write_pid(char * path) {
    pid_t recv_pid = getpid();

    FILE * pidfile = fopen(path, "wb");
    fprintf(pidfile, "%d", recv_pid);
    fclose(pidfile);

    return(0);
}

void read_from_conditioner() {
    char a = 0;
    unsigned long long p = 0;
    int i, j = 0;
    int x = 0;

    for (i = 0; i < 8; i++) {
        x = 0;
        while (1) {
            for (j = 0; j < 10; j++) {
                if (_rdseed64_step(&p) == 0) {
                    x = 1;
                }
            }
            printf("%d", x);
            a |= x;
            if (j < 7) {
                a = (a << 1);
            }
            usleep(10);
            break;
        }
    }
    printf("\n%c\n", a);
}

void read_from_conditioner_new() {
    unsigned long long p = 0;
    int i = 0;
    int x = 0;

    for (i = 0; i < 10; i++) {
        if (_rdseed64_step(&p) == 0) {
            x = 1;
        }
    }
    printf("%d", x);
    byte |= x;
    byte = (byte << 1);
    iter++;
    if (iter == 7) {
        printf("\n%c\n", byte);
        iter = 0;
        byte = 0;
    }
}

void abort_process() {
    printf("\nTerminating.\n");
    stopno++;
}

