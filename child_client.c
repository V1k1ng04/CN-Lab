#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
	int sockfd = 0;
	struct sockaddr_in serv_addr;
	char buffer[BUFFER_SIZE];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("ERROR | Socket creation error.\n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		printf("ERROR | Invalid address.\n");
		return -1;
	}

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("ERROR | Connection failed.\n");
		return -1;
	}

	printf("INFO | Connected to the server...\n");

	while (1)
	{
		printf("INPUT | Enter message: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		send(sockfd, buffer, strlen(buffer), 0);

		int valread = read(sockfd, buffer, BUFFER_SIZE);
		buffer[valread] = '\0';
		printf("INFO | Server: %s\n", buffer);
	}

	close(sockfd);
	return 0;
}