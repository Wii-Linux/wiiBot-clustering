#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <common/packet.h>
#include <frontend/config.h>
#include <unistd.h>

static pthread_t connThread[NUM_NODES];

typedef struct param {
	struct sockaddr_in *addr;
	int sockfd;
	int *connfd;
	int i;
} param_t;


static void *tryConnect(void *arg) {
	param_t *info = arg;
	int sockfd = info->sockfd;
	int *connfd = info->connfd;
	int i = info->i;
	struct sockaddr_in *addr = info->addr;

	// now free the info struct, we've copied the data we need to the stack
	free(info);

	printf("Trying to connect to %s (%s)...\n", nodes[i].description, nodes[i].ip);

	*connfd = connect(sockfd, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
	if (*connfd < 0) {
		perror("connect");
		return NULL;
	}

	return NULL;
}

void *startNet(void *arg) {
	(void)arg;
	puts("Hello from network thread");

	// initialize a socket so the central server can connect to us
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return NULL;
	}

	struct sockaddr_in addr[NUM_NODES];
	int connfd[NUM_NODES];
	for (int i = 0; i < NUM_NODES; i++) {
		param_t *info = malloc(sizeof(param_t));
		info->addr = &addr[i];
		info->sockfd = sockfd;
		info->connfd = &connfd[i];
		info->i = i;

		// we do this in it's own thread because connecting could take ages if the node is down,
		// and we don't want to block the entire network thread if a node is down.
		pthread_create(&connThread[i], NULL, tryConnect, info);

		// don't free info here, the thread will do that after it has the info it needs
	}

	while (true) {
		// check for new messages from nodes

		// respond to, or take note of, any messages recieved

		// if enough time has passed, ping each node to verify that they're still alive

		// if they're dead, remove them from the list of nodes available to execute commands, and close their connection

		// periodically check dead nodes to see if they're back

		// handle sending any data necessary back to the main thread, for sending and updating Discord messages
	}

	for (int i = 0; i < NUM_NODES; i++) {
		pthread_kill(connThread[i], SIGINT);
		pthread_join(connThread[i], NULL);
	}
	close(sockfd);

	return NULL;
}
