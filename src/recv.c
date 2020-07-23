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
    signal(SIGUSR1, read_from_conditioner);
    signal(SIGINT, abort_process);

    char * pidpath = "/tmp/.recv.pid";
    write_pid(pidpath);

    pause();

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
    int ret = 0;
    unsigned long long p = 0;

    while (1) {
        ret = _rdseed64_step(&p);
        printf("%d\n", ret);
        usleep(250);
    }
}

void abort_process() {
    printf("\nTerminating.\n");
    exit(0);
}

