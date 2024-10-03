#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
	int status, valread, clientfd;
	struct sockaddr_in servaddr;
	char *hello = "Hello from client";
	char buffer[1024] = { 0 };

	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("ERROR | Couldn't create socket.\n");
		return -1;
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary.
	if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0)
	{
		printf("ERROR | Invalid/Unsupported address.\n");
		return -1;
	}

	if ((status = connect(clientfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
	{
		printf("ERROR | Connection failed.\n");
		return -1;
	}

	send(clientfd, hello, strlen(hello), 0);
	printf("INFO | Message sent.\n");
	valread = read(clientfd, buffer, 1024 - 1);
	printf("%s\n", buffer);

	// Close connection socket.
	close(clientfd);

	return 0;
}