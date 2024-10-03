#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket)
{
	char buffer[BUFFER_SIZE];
	int valread;
	while ((valread = read(client_socket, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[valread] = '\0';
		printf("Client: %s\n", buffer);
		send(client_socket, buffer, strlen(buffer), 0);
	}
	close(client_socket);
	exit(0);
}

int main()
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	socklen_t addr_len = sizeof(address);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("ERROR | Bind failed.");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 10) < 0)
	{
		perror("ERROR | Listen failed.");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
	while (1)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len)) < 0)
		{
			perror("ERROR | Accept failed.");
			close(server_fd);
			exit(EXIT_FAILURE);
		}

		printf("INFO | New connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
		pid_t pid = fork();

		if (pid == 0)
		{
			close(server_fd);
			handle_client(new_socket);
		}
		else if (pid > 0)
		{
			close(new_socket);
		}
		else
		{
			perror("ERROR | Fork failed.");
			close(new_socket);
		}
	}

	close(server_fd);
	return 0;
}