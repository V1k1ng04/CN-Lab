#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9704

int main() {
	int sd;
	struct sockaddr_in address;

	sd = socket(AF_INET, SOCK_DGRAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("172.16.56.10");
	address.sin_port = htons(PORT);

	char clientBuf[24], serverBuf[24];

	printf("ENTER BUFFER:\n");
	gets(clientBuf);

	int len = sizeof(address);
	int m = sendto(sd, clientBuf, sizeof(clientBuf), 0, (struct sockaddr *) &address, len);
	int n = recvfrom(sd, serverBuf, sizeof(serverBuf), 0, (struct sockaddr *) &address, &len);

	printf("SERVER ECHO:\n");
	puts(serverBuf);

	return 0;
}