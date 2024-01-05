#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main() {
	//raise(SIGSTOP);
	printf("Hello, World!\n");

	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	printf("%ld %ld\n", spec.tv_sec, spec.tv_nsec/1000);
}
