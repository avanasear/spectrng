#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <immintrin.h>

/*
 This is essentially a proof of concept for the proof of concept
 We're interested to see the shortest amount of time it takes to have it read and not fail
*/
int main(int argc, char ** argv)
{
	int x = 0;
	unsigned long long p;
	int arr[1] = {0};

	for (x = 0; x < 1; x++)
	{
		arr[x] = _rdseed64_step(&p);
		usleep(100);
	}

	for (x=0; x < 1; x++) {
		printf("%d", arr[x]);
	}
	return 0;
}


