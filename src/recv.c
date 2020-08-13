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
void * pause_thread();

int main() {
    pthread_t thread0;
    pthread_t thread1;

    signal(SIGUSR1, read_from_conditioner);
    signal(SIGINT, abort_process);

    char * pidpath = "/tmp/.recv.pid";
    write_pid(pidpath);

    pthread_create(&thread0, NULL, read_thread, NULL);
    pthread_create(&thread1, NULL, pause_thread, NULL);
    pthread_join(thread1, NULL);

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
    exit(1);
}

void * read_thread() {
    int ret;
    unsigned long long p;

    while (stopno == 0) {
        ret = _rdseed64_step(&p);
        usleep(10);
    }

    pthread_exit(0);
}

void * pause_thread() {
    int x;

    pause();

    stopno++;
    pthread_exit(0);
}
