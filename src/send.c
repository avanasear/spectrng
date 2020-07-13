#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <immintrin.h>

pid_t recvpid;

int get_recv_pid(char * path);
void rng_send_char(char ltr);

// retrieves the PID from the get_recv_pid function
int main() {
    // known .recv.pid
    char * pidpath = "/tmp/.recv.pid";

    recvpid = get_recv_pid(pidpath);
    printf("Receiver pid: %d\n", recvpid);

    rng_send_char('a');

    return(0);
}


int get_recv_pid(char * path) {
    FILE * pidfile = fopen(path, "r");
    pid_t receiver = 0;
    // scans the pidfile for the process name and it's PID and then assigns
    // it to 'receiver'
    fscanf(pidfile, "%d", &receiver);

    return receiver;
}

// sends the specified letter to the terminal in binary form
void rng_send_char(char ltr) {

    int i, ret = 0;
    int x = 0x80;
    unsigned long long p;
    /*
        According to GNU.org SIGUSR1 is set aside for users to use in
        any way they want.
        This allows communication between './recv' and './send' processes
    */


// 7/13/2020 removed the always true statement of while(1)
    for (i = 0; i < 8; i++) {
        kill(recvpid, SIGUSR1);
        if (((ltr & x) >> (7 - i)) == 1) {
            printf("1");
            ret = _rdseed64_step(&p);
            usleep(150);
        }
        else {
            printf("0");
            usleep(300);
        }
        x /= 2;
    }
    printf("\n");
}
