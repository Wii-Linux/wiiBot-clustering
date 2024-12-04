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
		addr[i].sin_family = AF_INET;
		addr[i].sin_port = htons(nodes[i].port);
		addr[i].sin_addr.s_addr = inet_addr(nodes[i].ip);
		connfd[i] = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));

		if (connfd[i] < 0) {
			perror("connect");
			return NULL;
		}
	}

	while (true) {
		// check for new messages from nodes

		// respond to, or take note of, any messages recieved

		// if enough time has passed, ping each node to verify that they're still alive

		// if they're dead, remove them from the list of nodes available to execute commands, and close their connection

		// periodically check dead nodes to see if they're back

		// handle sending any data necessary back to the main thread, for sending and updating Discord messages
	}

	close(sockfd);

	return NULL;
}
