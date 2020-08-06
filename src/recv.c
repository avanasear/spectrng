#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <immintrin.h>

int stopno = 0;

int write_pid(char * path);
void read_from_conditioner();
void abort_process();
void * read_thread();

int main() {
    int x = 0;
    pthread_t thread0;

    signal(SIGUSR1, read_from_conditioner);
    signal(SIGINT, abort_process);

    char * pidpath = "/tmp/.recv.pid";
    write_pid(pidpath);

    pthread_create(&thread0, NULL, read_thread, NULL);
    pthread_join(thread0, NULL);

    for (x=0; x<8; x++) {
        pause();
    }

    remove(pidpath);

    stopno++;

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

    ret = _rdseed64_step(&p);
    printf("%d\n", ret);
}

void abort_process() {
    printf("\nTerminating.\n");
    exit(1);
}

void * read_thread(){
    int ret;
    unsigned long long p;

    while (stopno == 0) {
        //ret = _rdseed64_step(&p);
        usleep(10000);
    }

    pthread_exit(0);
}
