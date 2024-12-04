#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <backend/net.h>
static pthread_t netThread;

int main() {
	puts("WiiBot Node v" NODE_VER " is starting up...");
	while (true) {
		pthread_create(&netThread, NULL, startNet, NULL);
		pthread_join(netThread, NULL);

		printf("\e[1;31mThe network thread exited, restarting...\e[0m\n");
	}
}
