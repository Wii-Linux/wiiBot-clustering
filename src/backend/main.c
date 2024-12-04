#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <backend/net.h>
static pthread_t netThread;
static int netId;

int main(int argc, char *argv[]) {
	(void)argc;
	puts("WiiBot Node v" NODE_VER " is starting up...");
	while (true) {
		pthread_create(&netThread, NULL, startNet, &netId);
		pthread_join(netThread, NULL);

		printf("\e[1;31mThe network thread exited, restarting...\e[0m\n");
	}
}
