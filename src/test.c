#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	raise(SIGSTOP);
	printf("Hello, World~\n");
}
