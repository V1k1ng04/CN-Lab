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
	char buffer[25];
	struct sockaddr_in saddr, caddr;

	sd = socket(AF_INET, SOCK_DGRAM, 0);

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("172.16.56.10");
	saddr.sin_port = htons(PORT);

	int result = bind(sd, (struct sockaddr *) &saddr, sizeof(saddr));
	int len = sizeof(caddr);
	int m = recvfrom(sd, buffer, sizeof(buffer), 0, (struct sockaddr *) &caddr, &len);

	printf("SENT FROM SERVER:\n");
	puts(buffer);

	int n = sendto(sd, buffer, sizeof(buffer), 0, (struct sockaddr *) &caddr, len);

	return 0;
}