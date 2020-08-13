#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <immintrin.h>

int main() {
    char * pidpath = "/tmp/.recv.pid";
    pid_t recvpid = 0;
    unsigned long long p;

    FILE * pidfile = fopen(pidpath, "r");
    pid_t receiver = 0;
    fscanf(pidfile, "%d", &receiver);
    fclose(pidfile);

    recvpid = receiver;

    kill(recvpid, SIGUSR1);
    _rdseed64_step(&p);

    return(0);
}