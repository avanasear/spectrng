#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <immintrin.h>

pid_t recvpid;

int get_recv_pid(char * path);
void rng_send_char(char ltr);

int main() {
    char * pidpath = "/tmp/.recv.pid";

    recvpid = get_recv_pid(pidpath);
    printf("Receiver pid: %d\n", recvpid);

    rng_send_char('a');

    return(0);
}

int get_recv_pid(char * path) {
    FILE * pidfile = fopen(path, "r");
    pid_t receiver = 0;
    fscanf(pidfile, "%d", &receiver);

    return receiver;
}

void rng_send_char(char ltr) {

    int i, ret = 0;
    int x = 0x80;
    unsigned long long p;

    for (i = 0; i < 8; i++) {
        kill(recvpid, SIGUSR1);
        if (((ltr & x) >> (7 - i)) == 1) {
            printf("1");
            ret = _rdseed64_step(&p);
            usleep(1000);
        }
        else {
            printf("0");
            usleep(1250);
        }
        x /= 2;
    }
    printf("\n");
}
