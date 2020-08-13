#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <immintrin.h>

int stopno = 0;
char * pidpath = "/tmp/.recv.pid";

int write_pid(char * path);
void read_from_conditioner();
void abort_process();
void * read_thread();

int main() {
    int thread_ct = 1;
    pthread_t threads[thread_ct];

    signal(SIGUSR1, read_from_conditioner);
    signal(SIGINT, abort_process);

    write_pid(pidpath);

    for (int i = 0; i < thread_ct; i++) {
        pthread_create(&threads[i], NULL, read_thread, NULL);
    }

    for (int i = 0; i < 8; i++) {
        pause();
    }
    stopno++;

    for (int i = 0; i < thread_ct; i++) {
        pthread_join(threads[i], NULL);
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
    int ret = 0;
    unsigned long long p = 0;

    ret = _rdseed64_step(&p);
    printf("%d\n", ret);
}

void abort_process() {
    printf("\nTerminating.\n");
    stopno++;

    remove(pidpath);

    exit(1);
}

void * read_thread() {
    int ret;
    unsigned long long p;

    while (stopno == 0) {
        ret = _rdseed64_step(&p);
        ret = _rdseed64_step(&p);
        ret = _rdseed64_step(&p);
        ret = _rdseed64_step(&p);
        ret = _rdseed64_step(&p);
        ret = _rdseed64_step(&p);
        ret = _rdseed64_step(&p);
        ret = _rdseed64_step(&p);
    }

    pthread_exit(0);
}
