#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <common/packet.h>
#include <common/config.h>

void startNet() {
	puts("Hello from network thread");

	// initialize a socket so the central server can connect to us
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return;
	}

	int option = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	// set up the server address
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(DEFAULT_PORT);

	// bind the socket to the server address
	if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("bind");
		return;
	}

	// listen for incoming connections
	if (listen(sockfd, 1) < 0) {
		perror("listen");
		return;
	}

	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);

	uint8_t *recvbuf = malloc(sizeof(packet_t));
	uint8_t *sendbuf = malloc(sizeof(packet_t));
	packet_t *incomingPacket = (packet_t *)recvbuf;
	packet_t *outgoingPacket = (packet_t *)sendbuf;

	int connfd = accept(sockfd, (struct sockaddr *)&client, &client_len);
	// accept a connection
	if (connfd < 0) {
		perror("accept");
		return;
	}
	puts("accepted");

	while (true) {
		// recieve a message from the client into recvbuf
		int ret = recv(connfd, recvbuf, sizeof(packet_t), 0);
		printf("recv returned %d\n", ret);

		if (ret == -1) {
			perror("recv");
			free(recvbuf);
			free(sendbuf);
			return;
		}


		// validate that the message is a valid packet
		if (ret != -1 && incomingPacket->magic != PACKET_MAGIC) {
			// invalid packet, return error
			memcpy(outgoingPacket->magic, PACKET_MAGIC, 4);
			outgoingPacket->type = PACKET_TYPE_ERROR;
			send(connfd, sendbuf, sizeof(packet_t), 0);
		}
	}

	return;
}
