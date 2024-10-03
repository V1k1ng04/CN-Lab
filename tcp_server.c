#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080


int main(int argc, char const *argv[])
{
	int serverfd, newsockfd;
	ssize_t valread;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);

	char buffer[1024] = { 0 };
	char *hello = "Hello from server";

	// Create server file descriptor
	if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("ERROR | Socket failed.\n");
		exit(EXIT_FAILURE);
	}

	// Forcefully attach socket to 8080
	if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("ERROR | Couldn't set socket options.\n");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(serverfd, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		perror("ERROR | Bind failed.\n");
		exit(EXIT_FAILURE);
	}

	if (listen(serverfd, 3) < 0)
	{
		perror("ERROR | Couldn't listen on port.\n");
		exit(EXIT_FAILURE);
	}

	if ((newsockfd = accept(serverfd, (struct sockaddr *) &address, &addrlen)) < 0)
	{
		perror("ERROR | Couldn't accept request.\n");
		exit(EXIT_FAILURE);
	}

	valread = read(newsockfd, buffer, 1024 - 1);
	printf("%s\n", buffer);
	send(newsockfd, hello, strlen(hello), 0);
	printf("INFO | Sent message.\n");

	// Close connected socket.
	close(newsockfd);
	// Close listening socket.
	close(serverfd);

	return 0;
}