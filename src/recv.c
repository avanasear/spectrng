#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <immintrin.h>

int stopno = 0;

int write_pid(char * path);
void read_from_conditioner();
void abort_process();

int main() {
    signal(SIGUSR1, read_from_conditioner);
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
    //similar to 'send', we read the temp file belonging to 'recv'
    FILE * pidfile = fopen(path, "wb");
    fprintf(pidfile, "%d", recv_pid);
    fclose(pidfile);

    return(0);
}

void read_from_conditioner() {
    // initialize variables to 0.
    char a = 0;
    unsigned long long p = 0;
    int i, j = 0;
    int x = 0;

    for (i = 0; i < 8; i++) {
        x = 0;
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
    printf("\n", a);
}

void abort_process() {
    printf("\nTerminating.\n");
    stopno++;
}

